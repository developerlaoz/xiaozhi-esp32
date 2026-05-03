/*
 * @Description: XIAO ESP32-S3 Sense Board Implementation
 * @Author: Custom
 * @Date: 2025
 * @License: GPL 3.0
 */
#include "wifi_board.h"
#include "xiao_audio_codec.h"
#include "display/lcd_display.h"
#include "application.h"
#include "config.h"
#include "esp_lcd_gc9a01.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_commands.h"

#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>

#define TAG "XIAO-ESP32S3-SENSE"

class NoBacklight : public Backlight {
public:
    NoBacklight() : Backlight() {}
    ~NoBacklight() {}

protected:
    void SetBrightnessImpl(uint8_t brightness) override {
        ESP_LOGI(TAG, "SetBacklight brightness: %d", brightness);
    }
};

class XIAOESp32S3SenseBoard : public WifiBoard {
private:
    LcdDisplay *display_ = nullptr;

    void InitSpi() {
        ESP_LOGI(TAG, "=== InitSpi START ===");
        ESP_LOGI(TAG, "DISPLAY_MOSI=%d, DISPLAY_SCLK=%d, DISPLAY_WIDTH=%d, DISPLAY_HEIGHT=%d",
                 DISPLAY_MOSI, DISPLAY_SCLK, DISPLAY_WIDTH, DISPLAY_HEIGHT);
        
        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = DISPLAY_MOSI;
        buscfg.miso_io_num = GPIO_NUM_NC;
        buscfg.sclk_io_num = DISPLAY_SCLK;
        buscfg.quadwp_io_num = GPIO_NUM_NC;
        buscfg.quadhd_io_num = GPIO_NUM_NC;
        buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t);
        
        ESP_LOGI(TAG, "spi_bus_initialize SPI3_HOST...");
        esp_err_t ret = spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "spi_bus_initialize FAILED: %s", esp_err_to_name(ret));
            return;
        }
        ESP_LOGI(TAG, "spi_bus_initialize OK");
        ESP_LOGI(TAG, "=== InitSpi END ===");
    }

    void InitGc9a01Display() {
        ESP_LOGI(TAG, "=== InitGc9a01Display START ===");
        ESP_LOGI(TAG, "DISPLAY_CS=%d, DISPLAY_DC=%d, DISPLAY_RST=%d",
                 DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);

        esp_lcd_panel_io_handle_t panel_io = nullptr;
        esp_lcd_panel_handle_t panel = nullptr;

        // Panel IO configuration
        ESP_LOGI(TAG, "Configuring panel IO...");
        esp_lcd_panel_io_spi_config_t io_config = {};
        io_config.cs_gpio_num = DISPLAY_CS;
        io_config.dc_gpio_num = DISPLAY_DC;
        io_config.spi_mode = 0;
        io_config.pclk_hz = 40 * 1000 * 1000;  // 40MHz
        io_config.trans_queue_depth = 10;
        io_config.lcd_cmd_bits = 8;
        io_config.lcd_param_bits = 8;
        
        ESP_LOGI(TAG, "esp_lcd_new_panel_io_spi...");
        esp_err_t ret = esp_lcd_new_panel_io_spi(SPI3_HOST, &io_config, &panel_io);
        if (ret != ESP_OK || panel_io == nullptr) {
            ESP_LOGE(TAG, "esp_lcd_new_panel_io_spi FAILED: %s", esp_err_to_name(ret));
            return;
        }
        ESP_LOGI(TAG, "esp_lcd_new_panel_io_spi OK, panel_io=%p", panel_io);

        // Panel configuration
        ESP_LOGI(TAG, "Configuring panel device...");
        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = DISPLAY_RST;
        panel_config.rgb_endian = LCD_RGB_ENDIAN_RGB;
        panel_config.bits_per_pixel = 16;
        
        ESP_LOGI(TAG, "esp_lcd_new_panel_gc9a01...");
        ret = esp_lcd_new_panel_gc9a01(panel_io, &panel_config, &panel);
        if (ret != ESP_OK || panel == nullptr) {
            ESP_LOGE(TAG, "esp_lcd_new_panel_gc9a01 FAILED: %s", esp_err_to_name(ret));
            return;
        }
        ESP_LOGI(TAG, "esp_lcd_new_panel_gc9a01 OK, panel=%p", panel);

        // Initialize panel
        ESP_LOGI(TAG, "esp_lcd_panel_reset...");
        ret = esp_lcd_panel_reset(panel);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "esp_lcd_panel_reset FAILED: %s", esp_err_to_name(ret));
            return;
        }
        ESP_LOGI(TAG, "esp_lcd_panel_reset OK");

        ESP_LOGI(TAG, "esp_lcd_panel_init...");
        ret = esp_lcd_panel_init(panel);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "esp_lcd_panel_init FAILED: %s", esp_err_to_name(ret));
            return;
        }
        ESP_LOGI(TAG, "esp_lcd_panel_init OK");

        ESP_LOGI(TAG, "esp_lcd_panel_invert_color(false)...");
        ret = esp_lcd_panel_invert_color(panel, false);
        ESP_LOGI(TAG, "esp_lcd_panel_invert_color: %s", esp_err_to_name(ret));

        ESP_LOGI(TAG, "esp_lcd_panel_swap_xy(%s)...", DISPLAY_SWAP_XY ? "true" : "false");
        ret = esp_lcd_panel_swap_xy(panel, DISPLAY_SWAP_XY);
        ESP_LOGI(TAG, "esp_lcd_panel_swap_xy: %s", esp_err_to_name(ret));

        ESP_LOGI(TAG, "esp_lcd_panel_mirror_x(%s)...", DISPLAY_MIRROR_X ? "true" : "false");
        ret = esp_lcd_panel_mirror(panel, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y);
        ESP_LOGI(TAG, "esp_lcd_panel_mirror: %s", esp_err_to_name(ret));

        ESP_LOGI(TAG, "esp_lcd_panel_disp_on_off(true)...");
        ret = esp_lcd_panel_disp_on_off(panel, true);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "esp_lcd_panel_disp_on_off FAILED: %s", esp_err_to_name(ret));
            return;
        }
        ESP_LOGI(TAG, "esp_lcd_panel_disp_on_off OK");

        // Create display object
        ESP_LOGI(TAG, "Creating SpiLcdDisplay object...");
        display_ = new SpiLcdDisplay(panel_io, panel,
                                    DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                    DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y,
                                    DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y,
                                    DISPLAY_SWAP_XY);
        
        if (display_ == nullptr) {
            ESP_LOGE(TAG, "SpiLcdDisplay creation FAILED!");
            return;
        }
        
        ESP_LOGI(TAG, "=== InitGc9a01Display END - SUCCESS ===");
        ESP_LOGI(TAG, "GC9A01 LCD initialized: %dx%d", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    }

public:
    XIAOESp32S3SenseBoard() {
        ESP_LOGI(TAG, "=== XIAOESp32S3SenseBoard Constructor START ===");
        
        // Initialize SPI and Display
        ESP_LOGI(TAG, "Calling InitSpi()...");
        InitSpi();
        
        ESP_LOGI(TAG, "Calling InitGc9a01Display()...");
        InitGc9a01Display();
        
        ESP_LOGI(TAG, "=== XIAOESp32S3SenseBoard Constructor END ===");
    }

    virtual ~XIAOESp32S3SenseBoard() {
        ESP_LOGI(TAG, "~XIAOESp32S3SenseBoard destructor");
        if (display_) {
            delete display_;
            display_ = nullptr;
        }
    }

    virtual AudioCodec *GetAudioCodec() override {
        ESP_LOGI(TAG, "GetAudioCodec called");
        static XIAOAudioCodec audio_codec(
            AUDIO_INPUT_SAMPLE_RATE,
            AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_SPKR_I2S_GPIO_BCLK,
            AUDIO_SPKR_I2S_GPIO_LRCLK,
            AUDIO_SPKR_I2S_GPIO_DATA);
        return &audio_codec;
    }

    virtual Display *GetDisplay() override {
        ESP_LOGI(TAG, "GetDisplay called, display_=%p", display_);
        return display_;
    }

    virtual Backlight* GetBacklight() override {
        ESP_LOGI(TAG, "GetBacklight called");
        static NoBacklight backlight;
        return &backlight;
    }
};

DECLARE_BOARD(XIAOESp32S3SenseBoard);

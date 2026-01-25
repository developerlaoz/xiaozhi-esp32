#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

#define AUDIO_INPUT_SAMPLE_RATE  24000
#define AUDIO_OUTPUT_SAMPLE_RATE 24000

// I2S output for MAX98357A amplifier
#define AUDIO_SPKR_I2S_GPIO_BCLK GPIO_NUM_15   // D2
#define AUDIO_SPKR_I2S_GPIO_LRCLK GPIO_NUM_13  // D3
#define AUDIO_SPKR_I2S_GPIO_DATA GPIO_NUM_12   // D4
#define AUDIO_SPKR_ENABLE GPIO_NUM_14          // D5

// PDM microphone input (built-in on Xiao ESP32S3 Sense)
#define AUDIO_MIC_I2S_GPIO_BCLK GPIO_NUM_NC    // Not used for PDM
#define AUDIO_MIC_I2S_GPIO_WS GPIO_NUM_4       // PDM clock
#define AUDIO_MIC_I2S_GPIO_DATA GPIO_NUM_3     // PDM data
#define AUDIO_MIC_ENABLE GPIO_NUM_NC

// Display configuration for GC9A01A
#define DISPLAY_SPI_SCLK_PIN    GPIO_NUM_27    // D6
#define DISPLAY_SPI_MOSI_PIN    GPIO_NUM_33    // D7
#define DISPLAY_SPI_CS_PIN      GPIO_NUM_32    // D8
#define DISPLAY_SPI_DC_PIN      GPIO_NUM_35    // D9
#define DISPLAY_SPI_RESET_PIN   GPIO_NUM_34    // D10
#define DISPLAY_BACKLIGHT_PIN   GPIO_NUM_25    // D1
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false

#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  240
#define DISPLAY_MIRROR_X true
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false

#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0

// Built-in LED on Xiao ESP32S3 Sense (GPIO21)
#define BUILTIN_LED_GPIO        GPIO_NUM_21
// Boot button (GPIO0)
#define BOOT_BUTTON_GPIO        GPIO_NUM_0

// Audio input reference (true for internal reference, false for external)
#define AUDIO_INPUT_REFERENCE   true

#endif // _BOARD_CONFIG_H_
#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>
#include "pin_config.h"

#define AUDIO_INPUT_REFERENCE true
#define AUDIO_INPUT_SAMPLE_RATE 16000
#define AUDIO_OUTPUT_SAMPLE_RATE 16000

// MAX98357A Speaker I2S
#define AUDIO_SPKR_I2S_GPIO_BCLK static_cast<gpio_num_t>(MAX98357A_BCLK)
#define AUDIO_SPKR_I2S_GPIO_LRCLK static_cast<gpio_num_t>(MAX98357A_LRCLK)
#define AUDIO_SPKR_I2S_GPIO_DATA static_cast<gpio_num_t>(MAX98357A_DATA)
#define AUDIO_SPKR_ENABLE static_cast<gpio_num_t>(MAX98357A_SD_MODE)

// PDM Microphone (onboard ES7210)
#define AUDIO_MIC_I2S_GPIO_BCLK static_cast<gpio_num_t>(MIC_PDM_CLK)
#define AUDIO_MIC_I2S_GPIO_DATA static_cast<gpio_num_t>(MIC_PDM_DATA)

#define TOUCH_I2C_SDA_PIN static_cast<gpio_num_t>(IIC_SDA)
#define TOUCH_I2C_SCL_PIN static_cast<gpio_num_t>(IIC_SCL)

#define BUILTIN_LED_GPIO GPIO_NUM_NC
#define BOOT_BUTTON_GPIO GPIO_NUM_NC
#define VOLUME_UP_BUTTON_GPIO GPIO_NUM_NC
#define VOLUME_DOWN_BUTTON_GPIO GPIO_NUM_NC

#define DISPLAY_WIDTH LCD_WIDTH
#define DISPLAY_HEIGHT LCD_HEIGHT
#define DISPLAY_MOSI LCD_MOSI
#define DISPLAY_SCLK LCD_SCLK
#define DISPLAY_DC LCD_DC
#define DISPLAY_RST LCD_RST
#define DISPLAY_CS LCD_CS
#define DISPLAY_BL GPIO_NUM_NC
#define DISPLAY_MIRROR_X true
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false

#define DISPLAY_OFFSET_X 0
#define DISPLAY_OFFSET_Y 0

#define DISPLAY_BACKLIGHT_PIN DISPLAY_BL
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false

#endif // _BOARD_CONFIG_H_

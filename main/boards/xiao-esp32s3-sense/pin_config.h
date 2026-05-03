/*
 * @Description: XIAO ESP32-S3 Sense Board Configuration
 * @Author: Custom
 * @Date: 2025
 * @License: GPL 3.0
 */
#pragma once

// XIAO ESP32-S3 Sense GPIO mapping (D0-D12)
// D0 = GPIO1, D1 = GPIO2, D2 = GPIO3, D3 = GPIO4, D4 = GPIO5
// D5 = GPIO6, D6 = GPIO43, D7 = GPIO44, D8 = GPIO7, D9 = GPIO8
// D10 = GPIO9, D11 = GPIO42, D12 = GPIO41

// ========== MAX98357A Audio Amplifier ==========
#define MAX98357A_BCLK 4      // D3 = GPIO4, I2S Clock
#define MAX98357A_LRCLK 5     // D4 = GPIO5, I2S Word Select
#define MAX98357A_DATA 6      // D5 = GPIO6, I2S Data
#define MAX98357A_SD_MODE 7   // D8 = GPIO7, Enable Pin

// ========== Sense Onboard Microphone (ES7210 PDM) ==========
#define MIC_PDM_CLK 42        // GPIO42, MIC Clock (D11)
#define MIC_PDM_DATA 41       // GPIO41, MIC Data (D12)

// ========== GC9A01 LCD Display (240x240 Round) ==========
#define LCD_WIDTH 240
#define LCD_HEIGHT 240
#define LCD_MOSI 8            // D9 = GPIO8, SPI Data (avoid 43/44 USB-UART)
#define LCD_SCLK 9            // D10 = GPIO9, SPI Clock
#define LCD_DC 1              // D0 = GPIO1, Data/Command
#define LCD_CS 2              // D1 = GPIO2, Chip Select
#define LCD_RST -1            // Not connected, use hardware reset
#define LCD_BL -1             // No backlight control pin

// ========== I2C (Sense Onboard Codec ES8311) ==========
#define IIC_SDA 4             // D3 = GPIO4 (shared with I2S)
#define IIC_SCL 5             // D4 = GPIO5 (shared with I2S)

// ========== ES7210 PDM Microphone I2C Address ==========
#define ES7210_I2C_ADDR 0x82  // ES7210 default I2C address

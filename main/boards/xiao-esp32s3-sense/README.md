# Xiao ESP32S3 Sense Board

Xiao ESP32S3 Sense board with MAX98357A amplifier and GC9A01A display.

## Hardware Configuration

- MCU: ESP32-S3
- Built-in PDM microphone (MP34DT05)
- External I2S amplifier: MAX98357A
- SPI display: GC9A01A (240x240)

## Pin Mapping

### Audio
- I2S BCLK: GPIO15 (D2)
- I2S LRCLK: GPIO13 (D3)
- I2S DATA: GPIO12 (D4)
- Amplifier Enable: GPIO14 (D5)

### PDM Microphone (built-in)
- PDM CLK: GPIO4
- PDM DATA: GPIO3

### Display (GC9A01A)
- SPI SCLK: GPIO27 (D6)
- SPI MOSI: GPIO33 (D7)
- SPI CS: GPIO32 (D8)
- SPI DC: GPIO35 (D9)
- SPI RESET: GPIO34 (D10)
- Backlight: GPIO25 (D1)

### Others
- Built-in LED: GPIO21
- Boot button: GPIO0

## Compilation

```bash
python scripts/release.py xiao-esp32s3-sense
```

Or manually with idf.py:

```bash
idf.py set-target esp32s3
idf.py menuconfig  # Select Board Type: Xiao ESP32S3 Sense
idf.py build
idf.py flash monitor
```

## Features

- PDM microphone input
- I2S speaker output via MAX98357A
- 240x240 SPI display
- Built-in LED and button support
- Power save timer with display dimming

## Notes

The board uses NoAudioCodecSimplexPdm for audio handling, supporting PDM microphone and I2S speaker simultaneously.
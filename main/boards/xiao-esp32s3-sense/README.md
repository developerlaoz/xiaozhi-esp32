# XIAO ESP32-S3 Sense Board

Seeed Studio XIAO ESP32-S3 Sense with MAX98357 I2S audio amplifier and GC9A01 240x240 round LCD.

## Hardware

- **MCU**: ESP32-S3-WROOM-1 (8MB Flash)
- **Audio**: MAX98357 I2S Class D Amplifier
- **Display**: GC9A01 240x240 Round SPI LCD
- **Microphone**: Onboard ES7210 PDM Microphone

## Wiring

### MAX98357A Audio
| XIAO | GPIO | MAX98357 | Function |
|------|------|----------|----------|
| 5V | - | VIN | Power (5V) |
| GND | - | GND | Ground |
| D3 | GPIO4 | BLCK | I2S Clock |
| D4 | GPIO5 | LRC | I2S Word Select |
| D5 | GPIO6 | DIN | I2S Data |
| D8 | GPIO7 | SD | Enable (High active) |

### GC9A01 LCD
| XIAO | GPIO | GC9A01 | Function |
|------|------|--------|----------|
| 3V3 | - | VCC | Power (3.3V) |
| GND | - | GND | Ground |
| D0 | GPIO1 | DC | Data/Command |
| D1 | GPIO2 | CS | Chip Select |
| D6 | GPIO43 | SDA | SPI Data |
| D7 | GPIO44 | SCL | SPI Clock |
| 3V3 | - | RST | Reset (High) |

## Build

```bash
idf.py set-target esp32s3
idf.py menuconfig  # Select Board Type -> XIAO ESP32-S3 Sense
idf.py build
idf.py flash monitor
```

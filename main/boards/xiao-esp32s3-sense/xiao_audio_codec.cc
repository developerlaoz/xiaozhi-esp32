/*
 * @Description: XIAO ESP32-S3 Sense Audio Codec Implementation
 * @Author: Custom
 * @Date: 2025
 * @License: GPL 3.0
 */
#include "xiao_audio_codec.h"

#include <esp_log.h>
#include <driver/i2s_std.h>
#include <driver/i2s_pdm.h>
#include <driver/gpio.h>

#include "config.h"

static const char TAG[] = "XIAOAudioCodec";

XIAOAudioCodec::XIAOAudioCodec(int input_sample_rate, int output_sample_rate,
    gpio_num_t spkr_bclk, gpio_num_t spkr_lrclk, gpio_num_t spkr_data) {
    input_channels_ = 1;
    output_channels_ = 2;
    input_sample_rate_ = input_sample_rate;
    output_sample_rate_ = output_sample_rate;
    input_enabled_ = false;
    output_enabled_ = false;
    tx_handle_ = nullptr;
    rx_handle_ = nullptr;

    gpio_num_t mic_clk = static_cast<gpio_num_t>(MIC_PDM_CLK);
    gpio_num_t mic_data = static_cast<gpio_num_t>(MIC_PDM_DATA);

    // ========== Step 1: Initialize I2S1 (TX) for MAX98357A Speaker ==========
    i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_1, I2S_ROLE_MASTER);
    tx_chan_cfg.auto_clear = true;
    i2s_new_channel(&tx_chan_cfg, &tx_handle_, NULL);

    // Standard Philips I2S format for MAX98357A (Mono mode)
    i2s_std_slot_config_t tx_slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(
        I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);
    
    i2s_std_config_t tx_std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(static_cast<uint32_t>(output_sample_rate)),
        .slot_cfg = tx_slot_cfg,
        .gpio_cfg = {
            .mclk = GPIO_NUM_NC,
            .bclk = spkr_bclk,
            .ws = spkr_lrclk,
            .dout = spkr_data,
            .din = GPIO_NUM_NC,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    i2s_channel_init_std_mode(tx_handle_, &tx_std_cfg);
    i2s_channel_enable(tx_handle_);

    gpio_num_t max98357_enable = static_cast<gpio_num_t>(MAX98357A_SD_MODE);
    gpio_reset_pin(max98357_enable);
    gpio_set_direction(max98357_enable, GPIO_MODE_OUTPUT);
    gpio_set_level(max98357_enable, 1);

    // ========== Step 2: Initialize PDM for ES7210 Microphone ==========
    // ES7210 uses PDM interface
    // CLK = GPIO42, DATA = GPIO41
    i2s_chan_config_t rx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    rx_chan_cfg.auto_clear = true;
    esp_err_t ret = i2s_new_channel(&rx_chan_cfg, NULL, &rx_handle_);

    // PDM mode configuration
    i2s_pdm_rx_config_t pdm_rx_cfg = {
        .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(static_cast<uint32_t>(input_sample_rate)),
        .slot_cfg = I2S_PDM_RX_SLOT_PCM_FMT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .clk = mic_clk,      // GPIO42
            .din = mic_data,     // GPIO41
            .invert_flags = {
                .clk_inv = false,
            },
        },
    };

    ret = i2s_channel_init_pdm_rx_mode(rx_handle_, &pdm_rx_cfg);
    ret = i2s_channel_enable(rx_handle_);
}

XIAOAudioCodec::~XIAOAudioCodec() {
    if (tx_handle_) {
        i2s_channel_disable(tx_handle_);
        i2s_del_channel(tx_handle_);
    }
    if (rx_handle_) {
        i2s_channel_disable(rx_handle_);
        i2s_del_channel(rx_handle_);
    }
}

void XIAOAudioCodec::SetOutputVolume(int volume) {
    volume_ = volume;
}

void XIAOAudioCodec::EnableInput(bool enable) {
    input_enabled_ = enable;
}

void XIAOAudioCodec::EnableOutput(bool enable) {
    output_enabled_ = enable;
}

int XIAOAudioCodec::Read(int16_t *dest, int samples) {
    if (!input_enabled_ || !rx_handle_) {
        return 0;
    }

    size_t bytes_to_read = samples * sizeof(int16_t);
    size_t bytes_read = 0;
    esp_err_t ret = i2s_channel_read(rx_handle_, dest, bytes_to_read, &bytes_read, pdMS_TO_TICKS(100));

    if (ret != ESP_OK) {
        return 0;
    }

    return bytes_read / sizeof(int16_t);
}

int XIAOAudioCodec::Write(const int16_t *data, int samples) {
    if (!output_enabled_ || !tx_handle_) {
        return 0;
    }

    size_t bytes_to_write = samples * sizeof(int16_t);
    size_t bytes_written = 0;

    // Apply volume
    if (volume_ < 100 && samples > 0) {
        static int16_t volume_buffer[2048];
        int samples_to_process = samples;
        if (samples_to_process > 1024) samples_to_process = 1024;
        float vol = volume_ / 100.0f;
        for (int i = 0; i < samples_to_process; i++) {
            int32_t sample = data[i];
            sample = (int32_t)(sample * vol);
            if (sample > 32767) sample = 32767;
            if (sample < -32768) sample = -32768;
            volume_buffer[i] = (int16_t)sample;
        }
        esp_err_t ret = i2s_channel_write(tx_handle_, volume_buffer, samples_to_process * sizeof(int16_t), &bytes_written, pdMS_TO_TICKS(100));
        return (ret == ESP_OK) ? (bytes_written / sizeof(int16_t)) : 0;
    }

    esp_err_t ret = i2s_channel_write(tx_handle_, data, bytes_to_write, &bytes_written, pdMS_TO_TICKS(100));
    if (ret != ESP_OK) {
        return 0;
    }
    return bytes_written / sizeof(int16_t);
}

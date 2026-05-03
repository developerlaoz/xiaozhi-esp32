/*
 * @Description: XIAO ESP32-S3 Sense Audio Codec
 * @Author: Custom
 * @Date: 2025
 * @License: GPL 3.0
 */
#ifndef _XIAO_AUDIO_CODEC_H_
#define _XIAO_AUDIO_CODEC_H_

#include "audio_codec.h"

#include <esp_codec_dev.h>
#include <esp_codec_dev_defaults.h>

class XIAOAudioCodec : public AudioCodec {
private:
    uint32_t volume_ = 70;

    void CreateVoiceHardware(gpio_num_t spkr_bclk, gpio_num_t spkr_lrclk, gpio_num_t spkr_data);

    virtual int Read(int16_t *dest, int samples) override;
    virtual int Write(const int16_t *data, int samples) override;

public:
    XIAOAudioCodec(int input_sample_rate, int output_sample_rate,
        gpio_num_t spkr_bclk, gpio_num_t spkr_lrclk, gpio_num_t spkr_data);
    virtual ~XIAOAudioCodec();

    virtual void SetOutputVolume(int volume) override;
    virtual void EnableInput(bool enable) override;
    virtual void EnableOutput(bool enable) override;
};

#endif // _XIAO_AUDIO_CODEC_H_

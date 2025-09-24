#include "util/delay.h"
#include "util/twi.h"

#include <stdint.h>
#include <stdbool.h>

#pragma once


// === === Register bit SHIFTS === ===

// Register 0x02
constexpr uint16_t REG_02H_ENABLE_SHIFT = 0;
constexpr uint16_t REG_02H_RESET_SHIFT = 1;
constexpr uint16_t REG_02H_NEW_METHOD_SHIFT = 2;
constexpr uint16_t REG_02H_RDS_EN_SHIFT = 3;
constexpr uint16_t REG_02H_CLK_MODE_SHIFT = 4;
constexpr uint16_t REG_02H_SK_MODE_SHIFT = 7;
constexpr uint16_t REG_02H_SEEK_SHIFT = 8;
constexpr uint16_t REG_02H_SEEK_DIR_SHIFT = 9;
constexpr uint16_t REG_02H_RCLK_MODE_SHIFT = 11;
constexpr uint16_t REG_02H_BASS_BOOST_SHIFT = 12;
constexpr uint16_t REG_02H_MONO_SHIFT = 13;
constexpr uint16_t REG_02H_MUTE_SHIFT = 14;
constexpr uint16_t REG_02H_AUDIO_OUTPUT_SHIFT = 15;

// Register 0x03
constexpr uint16_t REG_03H_CHAN_SPACING_SHIFT = 0;
constexpr uint16_t REG_03H_BAND_SELECT_SHIFT = 2;
constexpr uint16_t REG_03H_TUNE_SHIFT = 4;
constexpr uint16_t REG_03H_CHAN_SELECT_SHIFT = 6;

// Register 0x04
constexpr uint16_t REG_04H_AFCD_SHIFT = 8;
constexpr uint16_t REG_04H_SOFTMUTE_SHIFT = 9;
constexpr uint16_t REG_04H_DE_EMPHASIS_SHIFT = 11;

// Register 0x05
constexpr uint16_t REG_05H_VOLUME_SHIFT = 0;
constexpr uint16_t REG_05H_SNR_THRESHOLD_SHIFT = 7;
constexpr uint16_t REG_05H_INTERRUPT_SHIFT = 15;

// Register 0x0A (READ)
constexpr uint16_t REG_0AH_CHAN_READ_SHIFT = 0;
constexpr uint16_t REG_0AH_STEREO_SHIFT = 10;
constexpr uint16_t REG_0AH_RDS_BLK_E_SHIFT = 11;
constexpr uint16_t REG_0AH_RDS_SYNC_SHIFT = 12;
constexpr uint16_t REG_0AH_SEEK_FAIL_SHIFT = 13;
constexpr uint16_t REG_0AH_SEEK_DONE_SHIFT = 14;
constexpr uint16_t REG_0AH_RDS_READY_SHIFT = 15;

// Register 0x0B (READ)
constexpr uint16_t REG_0BH_BLKB_ERR_SHIFT = 0;
constexpr uint16_t REG_0BH_BLKA_ERR_SHIFT = 2;
constexpr uint16_t REG_0BH_ABCD_E_SHIFT = 4;
constexpr uint16_t REG_0BH_FM_READY_SHIFT = 7;
constexpr uint16_t REG_0BH_IS_STATION_SHIFT = 8;
constexpr uint16_t REG_0BH_RSSI_SHIFT = 9;


// === === Register bit MASKS === ===

// Register 0x02
constexpr uint16_t REG_02H_ENABLE_MASK = 0x01;
constexpr uint16_t REG_02H_RESET_MASK = 0x01;
constexpr uint16_t REG_02H_NEW_METHOD_MASK = 0x01;
constexpr uint16_t REG_02H_RDS_EN_MASK = 0x01;
constexpr uint16_t REG_02H_CLK_MODE_MASK = 0x07;
constexpr uint16_t REG_02H_SK_MODE_MASK = 0x01;
constexpr uint16_t REG_02H_SEEK_MASK = 0x01;
constexpr uint16_t REG_02H_SEEK_DIR_MASK = 0x01;
constexpr uint16_t REG_02H_RCLK_MODE_MASK = 0x01;
constexpr uint16_t REG_02H_BASS_BOOST_MASK = 0x01;
constexpr uint16_t REG_02H_MONO_MASK = 0x01;
constexpr uint16_t REG_02H_MUTE_MASK = 0x01;
constexpr uint16_t REG_02H_AUDIO_OUTPUT_MASK = 0x01;

// Register 0x03
constexpr uint16_t REG_03H_CHAN_SPACING_MASK = 0x07;
constexpr uint16_t REG_03H_BAND_SELECT_MASK = 0x07;
constexpr uint16_t REG_03H_TUNE_MASK = 0x01;
constexpr uint16_t REG_03H_CHAN_SELECT_MASK = 0x3FF;

// Register 0x04
constexpr uint16_t REG_04H_AFCD_MASK = 0x01;
constexpr uint16_t REG_04H_SOFTMUTE_MASK = 0x01;
constexpr uint16_t REG_04H_DE_EMPHASIS_MASK = 0x01;

// Register 0x05
constexpr uint16_t REG_05H_VOLUME_MASK = 0x0F;
constexpr uint16_t REG_05H_SNR_THRESHOLD_MASK = 0x0F;
constexpr uint16_t REG_05H_INTERRUPT_MASK = 0x01;

// Register 0x0A (READ)
constexpr uint16_t REG_0AH_CHAN_READ_MASK = 0x3FF;
constexpr uint16_t REG_0AH_STEREO_MASK = 0x01;
constexpr uint16_t REG_0AH_RDS_BLK_E_MASK = 0x01;
constexpr uint16_t REG_0AH_RDS_SYNC_MASK = 0x01;
constexpr uint16_t REG_0AH_SEEK_FAIL_MASK = 0x01;
constexpr uint16_t REG_0AH_SEEK_DONE_MASK = 0x01;
constexpr uint16_t REG_0AH_RDS_READY_MASK = 0x01;

// Register 0x0B (READ)
constexpr uint16_t REG_0BH_BLKB_ERR_MASK = 0x03;
constexpr uint16_t REG_0BH_BLKA_ERR_MASK = 0x03;
constexpr uint16_t REG_0BH_ABCD_E_MASK = 0x01;
constexpr uint16_t REG_0BH_FM_READY_MASK = 0x01;
constexpr uint16_t REG_0BH_IS_STATION_MASK = 0x01;
constexpr uint16_t REG_0BH_RSSI_MASK = 0x77;


class RDA5807M {
  public:
    void init();

    void tune_frequency(uint16_t frequency_mhz);
    void set_volume(uint8_t volume_level);
    void enable_mute(bool enabled);
    void enable_softmute(bool enabled);
    void enable_bass_boost(bool enabled);

    bool is_station(void);
    uint8_t get_raw_rssi(void);
    uint16_t get_frequency(void);


    static constexpr float FM_MIN_FREQUENCY = 50.0f;
    static constexpr float FM_MAX_FREQUENCY = 115.0f;

    typedef struct {
      uint8_t reg_bits;
      uint16_t value_khz;
    } chan_spacing_t;

    typedef struct {
      uint8_t reg_bits;
      uint16_t frq_start;
      uint16_t frq_end;
    } fm_band_t;


    const fm_band_t band_us_europe = {
      .reg_bits = 0x00,
      .frq_start = 870,
      .frq_end = 1080,
    };

    const fm_band_t band_japan = {
      .reg_bits = 0x01,
      .frq_start = 870,
      .frq_end = 1080,
    };

    const fm_band_t band_world = {
      .reg_bits = 0x02,
      .frq_start = 870,
      .frq_end = 1080,
    };

    const fm_band_t east_europe = {
      .reg_bits = 0x03,
      .frq_start = 650,
      .frq_end = 760,
    };

    const fm_band_t east_europe_alt = {
      .reg_bits = 0x03,
      .frq_start = 50,
      .frq_end = 650,
    };

    const fm_band_t* current_band_start = &band_us_europe;


    const chan_spacing_t chan_spacing_100 = {
      .reg_bits = 0x00,
      .value_khz = 100,
    };

    const chan_spacing_t chan_spacing_200 = {
      .reg_bits = 0x01,
      .value_khz = 200,
    };

    const chan_spacing_t chan_spacing_50 = {
      .reg_bits = 0x02,
      .value_khz = 50,
    };

    const chan_spacing_t chan_spacing_25 = {
      .reg_bits = 0x03,
      .value_khz = 25,
    };

    const chan_spacing_t* current_chan_spacing = &chan_spacing_100;


  private:
    void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits);
    void reg_get_bits(uint16_t reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t* buf);

    void reg_write_direct(uint8_t reg_addr, uint16_t reg_value);
    void reg_read_direct(uint8_t reg_addr, uint16_t* buf);

    void twi_init();
    void twi_start();
    void twi_write(uint8_t data);
    void twi_write(uint8_t* buf, uint8_t len);
    void twi_read(uint8_t* buf, uint8_t len);
    void twi_stop();


    static constexpr uint8_t RDA5807M_WRITE_INCREMENT = (0x10) << 1 | TW_WRITE;
    static constexpr uint8_t RDA5807M_READ_INCREMENT = (0x10) << 1 | TW_READ;

    static constexpr uint8_t RDA5807M_WRITE_DIRECT = (0x11) << 1 | TW_WRITE;
    static constexpr uint8_t RDA5807M_READ_DIRECT = (0x11) << 1 | TW_READ;

    uint16_t reg_02H = 0x0000;
    uint16_t reg_03H = 0x0000;
    uint16_t reg_04H = 0x0000;
    uint16_t reg_05H = 0x0000;
    uint16_t reg_06H = 0x0000;
    uint16_t reg_07H = 0x0000;
};

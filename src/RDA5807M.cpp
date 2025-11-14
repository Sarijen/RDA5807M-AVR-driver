#include "RDA5807M.h"


void RDA5807M::init() {
  twi_init();

  // Enable audio output
  reg_set_bits(&reg_02H, REG_02H_AUDIO_OUTPUT_SHIFT, REG_02H_AUDIO_OUTPUT_MASK, 1);

  // Disable mute
  reg_set_bits(&reg_02H, REG_02H_MUTE_SHIFT, REG_02H_MUTE_MASK, 1);

  // Enable the chip
  reg_set_bits(&reg_02H, REG_02H_ENABLE_SHIFT, REG_02H_ENABLE_MASK, 1);

  // Assert reset
  reg_set_bits(&reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 1);

  reg_write_direct(0x02, reg_02H);
  _delay_us(50);

  // Deassert reset
  reg_set_bits(&reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 0);

  reg_write_direct(0x02, reg_02H);
  _delay_us(50);

  // Bit 7 in Register 0x05 has to be 1, otherwise the audio will be very noisy
  // There's no information about this in the Datasheet
  reg_set_bits(&reg_05H, 7,    1,   1);
  //                     SHIFT MASK VALUE
}


void RDA5807M::reset() {

  // Assert reset
  reg_set_bits(&reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_SHIFT, 1);
  reg_write_direct(0x02, reg_02H);

  _delay_us(50);

  // Deassert reset
  reg_set_bits(&reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_SHIFT, 0);
  reg_write_direct(0x02, reg_02H);

}


/*
  Frequency represented as (uint16_t * 10) instead of float
  For example to tune 105.5MHz, pass 1055
*/
void RDA5807M::tune_frequency(uint16_t frequency_mhz) {
  uint16_t band_start_mhz = current_band_start->frq_start;
  uint16_t band_end_mhz = current_band_start->frq_end;
  uint16_t chan_spacing_khz = current_chan_spacing->value_khz;

  if (frequency_mhz > band_end_mhz) {
    frequency_mhz = band_end_mhz;
  } else if (frequency_mhz < band_start_mhz) {
    frequency_mhz = band_start_mhz;
  }

  uint16_t channel_number = ((frequency_mhz - band_start_mhz) * chan_spacing_khz);
  // Dividing by 100 to convert chan spacing kHz to MHz*10
  channel_number /= 100; 

  reg_set_bits(&reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, channel_number);


  // Tune to apply the frequency by setting TUNE bit
  constexpr uint8_t ENABLE_TUNE = 1;
  reg_set_bits(&reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, ENABLE_TUNE);

  // Write the new values into the IC
  reg_write_direct(0x03, reg_03H);
}


void RDA5807M::set_volume(uint8_t volume_level) {
  if (volume_level > REG_05H_VOLUME_MASK) {
    volume_level = REG_05H_VOLUME_MASK;
  }

  reg_set_bits(&reg_05H, REG_05H_VOLUME_SHIFT, REG_05H_VOLUME_MASK, volume_level);

  reg_write_direct(0x05, reg_05H);
}


void RDA5807M::enable_mute(bool enabled) {
  // False = Mute, True = Normal
  enabled = !enabled;

  reg_set_bits(&reg_02H, REG_02H_MUTE_SHIFT, REG_02H_RESET_MASK, enabled);
  reg_write_direct(0x02, reg_02H);
}


void RDA5807M::enable_softmute(bool enabled) {
  reg_set_bits(&reg_04H, REG_04H_SOFTMUTE_SHIFT, REG_04H_SOFTMUTE_MASK, enabled);
  reg_write_direct(0x04, reg_04H);
}


void RDA5807M::enable_bass_boost(bool enabled) {
  reg_set_bits(&reg_02H, REG_02H_BASS_BOOST_SHIFT, REG_02H_BASS_BOOST_MASK, enabled);
  reg_write_direct(0x02, reg_02H);
}


void RDA5807M::enable_softblend(bool enabled) {
  reg_set_bits(&reg_07H, REG_07H_SOFTBLEND_SHIFT, REG_07H_SOFTBLEND_MASK, enabled);
  reg_write_direct(0x07, reg_07H);
}


void RDA5807M::enable_mono(bool enabled) {
  reg_set_bits(&reg_02H, REG_02H_MONO_SHIFT, REG_02H_MONO_MASK, enabled);
  reg_write_direct(0x02, reg_02H);
}


void RDA5807M::set_deemphasis(uint8_t de_emphasis) {
  if (de_emphasis != 50 && de_emphasis != 75) {
    return;
  }

  reg_set_bits(&reg_04H, REG_04H_DE_EMPHASIS_SHIFT, REG_04H_DE_EMPHASIS_MASK, de_emphasis);

  reg_write_direct(0x04, reg_04H);
}


void RDA5807M::seek(bool enable, bool direction, bool enable_wrapping) {
  // Enable/Disable seeking
  reg_set_bits(&reg_02H, REG_02H_SEEK_EN_SHIFT, REG_02H_SEEK_EN_MASK, enable);

  // Set seeking direction (1 = UP, 0 = DOWN)
  reg_set_bits(&reg_02H, REG_02H_SEEK_DIR_SHIFT, REG_02H_SEEK_DIR_MASK, direction);

  // Enable/Disable wrapping on band end
  reg_set_bits(&reg_02H, REG_02H_SK_MODE_SHIFT, REG_02H_SK_MODE_MASK, enable_wrapping);

  reg_write_direct(0x02, reg_02H);
}


bool RDA5807M::is_station(void) {
  uint16_t temp_reg;
  uint16_t is_station;

  reg_read_direct(0x0B, &temp_reg);
  reg_get_bits(temp_reg, REG_0BH_IS_STATION_SHIFT, REG_0BH_IS_STATION_MASK, &is_station);

  return (bool)is_station;
}


bool RDA5807M::is_mono(void) {
  uint16_t temp_reg;
  uint16_t is_stereo;

  reg_read_direct(0x0A, &temp_reg);
  reg_get_bits(temp_reg, REG_0AH_STEREO_SHIFT, REG_0AH_STEREO_MASK, &is_stereo);

  return (bool)is_stereo;
}


uint8_t RDA5807M::get_raw_rssi(void) {
  uint16_t temp_reg;
  uint16_t raw_rssi_value;

  reg_read_direct(0x0B, &temp_reg);
  reg_get_bits(temp_reg, REG_0BH_RSSI_SHIFT, REG_0BH_RSSI_MASK, &raw_rssi_value);

  return raw_rssi_value;
}

/*
  Frequency represented as (uint16_t * 10) instead of float
  For example tuned 105.5MHz, it would return 1055
*/
uint16_t RDA5807M::get_frequency(void) {
  uint16_t temp_reg;
  uint16_t channel_number;
  uint16_t frequency_mhz;

  reg_read_direct(0x0A, &temp_reg);
  reg_get_bits(temp_reg, REG_0AH_CHAN_READ_SHIFT, REG_0AH_CHAN_READ_MASK, &channel_number);

  uint16_t chan_spacing_khz = current_chan_spacing->value_khz;
  uint16_t band_start_mhz = current_band_start->frq_start;

  // Dividing by 100 to convert kHz to MHz*10
  frequency_mhz = (chan_spacing_khz * channel_number) / 100 + band_start_mhz;

  return frequency_mhz;
}


void RDA5807M::reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits) {
  if (bits > bit_mask) {
    bits = bit_mask;
  }

  *reg &= ~(bit_mask << bit_shift); // Clear bits
  *reg |= (bits << bit_shift); // Write new bits
}


void RDA5807M::reg_get_bits(uint16_t reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t* buf) {
  uint16_t new_bits;
  
  new_bits = reg >> bit_shift; // Discard bits from right
  new_bits &= bit_mask; // Discard bits from left
  
  // Only bits left are those we want, so we're done
  *buf = new_bits;
}


void RDA5807M::reg_write_direct(uint8_t reg_addr, uint16_t reg_value) {

  uint8_t buffer[3] = {
    reg_addr,
    (uint8_t)(reg_value >> 8), // High byte
    (uint8_t)(reg_value & 0xFF), // Low byte
  };

  twi_start();
  twi_write(RDA5807M_WRITE_DIRECT);
  twi_write(buffer, 3);
  twi_stop();
}


void RDA5807M::reg_read_direct(uint8_t reg_addr, uint16_t* buf) {
  uint8_t temp_buf[2];

  twi_start();
  twi_write(RDA5807M_WRITE_DIRECT);
  twi_write(reg_addr); // Select the register we want to read

  twi_start(); // Repeated start without STOP
  twi_write(RDA5807M_READ_DIRECT);
  twi_read(temp_buf, 2);
  twi_stop();

  *buf = (temp_buf[0] << 8) | (temp_buf[1]);
}


void RDA5807M::twi_init() {
  // Prescaler = 1
  TWSR = 0;

  // 100 kHz SCL
  TWBR = (F_CPU / 100000UL - 16) / 2;
}


void RDA5807M::twi_start() {
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
  while (!(TWCR & _BV(TWINT)));
}


void RDA5807M::twi_write(uint8_t data) {
  TWDR = data; 
  TWCR = _BV(TWINT) | _BV(TWEN);
  while (!(TWCR & _BV(TWINT))); // Wait for transmission
}


void RDA5807M::twi_write(uint8_t* buf, uint8_t len) {
  for (; len > 0; len--) {
    TWDR = *buf++; 
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT))); // Wait for transmission
  }
}


void RDA5807M::twi_read(uint8_t* buf, uint8_t len) {
  for (; len > 1; len--) {
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA); // ACK
    while (!(TWCR & _BV(TWINT))); // Wait for transmission
    *buf++ = TWDR;
  }

  TWCR = _BV(TWINT) | _BV(TWEN);
  while (!(TWCR & _BV(TWINT))); // Wait for transmission
  *buf++ = TWDR;
}


void RDA5807M::twi_stop() {
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

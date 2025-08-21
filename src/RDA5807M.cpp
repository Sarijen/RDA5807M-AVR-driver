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


void RDA5807M::tune_frequency(float frequency) {
  if (frequency > FM_MAX_FREQUENCY) {
    frequency = FM_MAX_FREQUENCY;
  } else if (frequency < FM_MIN_FREQUENCY) {
    frequency = FM_MIN_FREQUENCY;
  }

  constexpr float FM_BAND_START = 87.0f;
  uint16_t channel = static_cast<uint16_t>((frequency - FM_BAND_START) * 10);
  reg_set_bits(&reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, channel);

  constexpr uint8_t ENABLE_TUNE = 1;
  reg_set_bits(&reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, ENABLE_TUNE);

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


void RDA5807M::reg_set_bits(uint16_t* reg, uint16_t reg_shift, uint16_t reg_mask, uint16_t bits) {
  if (bits > reg_mask) {
    bits = reg_mask;
  }

  *reg &= ~(reg_mask << reg_shift); // Clear bits
  *reg |= (bits << reg_shift); // Write new bits
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

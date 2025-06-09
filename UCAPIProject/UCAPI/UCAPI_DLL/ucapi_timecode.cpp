#include "pch.h"
#include "ucapi.h"
#include "ucapi_timecode.h"
#include <cstring>

// SMPTE LTC implementation
smpte_ltc_t::smpte_ltc_t() {
    clear();
}

smpte_ltc_t::~smpte_ltc_t() {
    _clean_up();
}

void smpte_ltc_t::clear() {
    memset(data, 0, sizeof(data));
    set_sync_word();
}

void smpte_ltc_t::_clean_up() {
    // Nothing to clean up for now
}

void smpte_ltc_t::_set_bit(uint8_t bit_position, bool value) {
    if (bit_position >= 80) return;
    
    uint8_t byte_index = bit_position / 8;
    uint8_t bit_index = bit_position % 8;
    
    if (value) {
        data[byte_index] |= (1 << bit_index);
    } else {
        data[byte_index] &= ~(1 << bit_index);
    }
}

bool smpte_ltc_t::_get_bit(uint8_t bit_position) const {
    if (bit_position >= 80) return false;
    
    uint8_t byte_index = bit_position / 8;
    uint8_t bit_index = bit_position % 8;
    
    return (data[byte_index] & (1 << bit_index)) != 0;
}

void smpte_ltc_t::_set_bcd_value(uint8_t byte_offset, uint8_t bit_offset, uint8_t value, uint8_t max_value) {
    if (value > max_value) value = max_value;
    
    // Convert to BCD and set bits
    uint8_t units = value % 10;
    uint8_t tens = value / 10;
    
    // Set units (4 bits)
    for (int i = 0; i < 4; i++) {
        _set_bit(bit_offset + i, (units & (1 << i)) != 0);
    }
    
    // Set tens (remaining bits)
    uint8_t tens_bits = (max_value >= 10) ? 4 : 2;  // Adjust based on max value
    for (int i = 0; i < tens_bits && (bit_offset + 8 + i) < 80; i++) {
        _set_bit(bit_offset + 8 + i, (tens & (1 << i)) != 0);
    }
}

uint8_t smpte_ltc_t::_get_bcd_value(uint8_t byte_offset, uint8_t bit_offset, uint8_t mask) const {
    uint8_t units = 0;
    uint8_t tens = 0;
    
    // Get units (4 bits)
    for (int i = 0; i < 4; i++) {
        if (_get_bit(bit_offset + i)) {
            units |= (1 << i);
        }
    }
    
    // Get tens (variable bits based on field)
    uint8_t tens_bits = 4;
    if (bit_offset == 24 || bit_offset == 40) tens_bits = 3;  // seconds/minutes tens (0-5)
    if (bit_offset == 56) tens_bits = 2;  // hours tens (0-2)
    if (bit_offset == 8) tens_bits = 2;   // frame tens (0-2)
    
    for (int i = 0; i < tens_bits && (bit_offset + 8 + i) < 80; i++) {
        if (_get_bit(bit_offset + 8 + i)) {
            tens |= (1 << i);
        }
    }
    
    return tens * 10 + units;
}

void smpte_ltc_t::set_frame(uint8_t frame) {
    if (frame > 29) frame = 29;  // Max 29 frames (0-29)
    _set_bcd_value(0, 0, frame, 29);
}

void smpte_ltc_t::set_second(uint8_t second) {
    if (second > 59) second = 59;
    _set_bcd_value(2, 16, second, 59);
}

void smpte_ltc_t::set_minute(uint8_t minute) {
    if (minute > 59) minute = 59;
    _set_bcd_value(4, 32, minute, 59);
}

void smpte_ltc_t::set_hour(uint8_t hour) {
    if (hour > 23) hour = 23;
    _set_bcd_value(6, 48, hour, 23);
}

void smpte_ltc_t::set_drop_frame(bool drop_frame) {
    _set_bit(10, drop_frame);
}

void smpte_ltc_t::set_color_frame(bool color_frame) {
    _set_bit(11, color_frame);
}

uint8_t smpte_ltc_t::get_frame() const {
    return _get_bcd_value(0, 0, 0x3F);
}

uint8_t smpte_ltc_t::get_second() const {
    return _get_bcd_value(2, 16, 0x7F);
}

uint8_t smpte_ltc_t::get_minute() const {
    return _get_bcd_value(4, 32, 0x7F);
}

uint8_t smpte_ltc_t::get_hour() const {
    return _get_bcd_value(6, 48, 0x3F);
}

bool smpte_ltc_t::get_drop_frame() const {
    return _get_bit(10);
}

bool smpte_ltc_t::get_color_frame() const {
    return _get_bit(11);
}

void smpte_ltc_t::set_user_bits(uint8_t field, uint8_t value) {
    if (field > 7 || value > 15) return;
    
    // User bits are at positions: 4-7, 12-15, 20-23, 28-31, 36-39, 44-47, 52-55, 60-63
    uint8_t bit_positions[] = {4, 12, 20, 28, 36, 44, 52, 60};
    uint8_t start_bit = bit_positions[field];
    
    for (int i = 0; i < 4; i++) {
        _set_bit(start_bit + i, (value & (1 << i)) != 0);
    }
}

uint8_t smpte_ltc_t::get_user_bits(uint8_t field) const {
    if (field > 7) return 0;
    
    uint8_t bit_positions[] = {4, 12, 20, 28, 36, 44, 52, 60};
    uint8_t start_bit = bit_positions[field];
    uint8_t value = 0;
    
    for (int i = 0; i < 4; i++) {
        if (_get_bit(start_bit + i)) {
            value |= (1 << i);
        }
    }
    
    return value;
}

void smpte_ltc_t::set_sync_word() {
    // Sync word: 0x3FFD (0011 1111 1111 1101) at bits 64-79
    // Set bits 64-79 to the sync pattern
    uint16_t sync_word = 0x3FFD;
    
    for (int i = 0; i < 16; i++) {
        _set_bit(64 + i, (sync_word & (1 << i)) != 0);
    }
}

bool smpte_ltc_t::is_valid_sync() const {
    uint16_t sync_word = 0;
    
    for (int i = 0; i < 16; i++) {
        if (_get_bit(64 + i)) {
            sync_word |= (1 << i);
        }
    }
    
    return sync_word == 0x3FFD;
}

// Legacy timecode implementation
timecode_t::timecode_t() {
	m_frame_number = 0;
	m_second_number = 0;
	m_minute_number = 0;
	m_hour_number = 0;
	m_frame_rate = FRAME_RATE_60; // Default to 60 fps
	m_drop_frame = 0;
	m_reserved = 0;
}

uint32_t timecode_t::pack(timecode_t timecode) {
	uint32_t packed = 0;
	packed |= (timecode.m_frame_number & 0xFF);
	packed |= ((timecode.m_second_number & 0x3F) << 8);
	packed |= ((timecode.m_minute_number & 0x3F) << 14);
	packed |= ((timecode.m_hour_number & 0x1F) << 20);
	packed |= ((timecode.m_frame_rate & 0xF) << 25);
	packed |= ((timecode.m_drop_frame & 0x1) << 29);
	packed |= ((timecode.m_reserved & 0x3) << 30);
	return packed;
}

timecode_t timecode_t::unpack(uint32_t packed) {
	timecode_t timecode;
	timecode.m_frame_number = packed & 0xFF;
	timecode.m_second_number = (packed >> 8) & 0x3F;
	timecode.m_minute_number = (packed >> 14) & 0x3F;
	timecode.m_hour_number = (packed >> 20) & 0x1F;
	timecode.m_frame_rate = (packed >> 25) & 0xF;
	timecode.m_drop_frame = (packed >> 29) & 0x1;
	timecode.m_reserved = (packed >> 30) & 0x3;
	return timecode;
}

smpte_ltc_t timecode_t::pack_smpte(timecode_t timecode) {
    smpte_ltc_t ltc;
    
    ltc.set_frame(timecode.m_frame_number);
    ltc.set_second(timecode.m_second_number);
    ltc.set_minute(timecode.m_minute_number);
    ltc.set_hour(timecode.m_hour_number);
    ltc.set_drop_frame(timecode.m_drop_frame != 0);
    ltc.set_color_frame(false);  // Default to false
    
    // Set sync word
    ltc.set_sync_word();
    
    return ltc;
}

timecode_t timecode_t::unpack_smpte(const smpte_ltc_t& ltc) {
    timecode_t timecode;
    
    timecode.m_frame_number = ltc.get_frame();
    timecode.m_second_number = ltc.get_second();
    timecode.m_minute_number = ltc.get_minute();
    timecode.m_hour_number = ltc.get_hour();
    timecode.m_drop_frame = ltc.get_drop_frame() ? 1 : 0;
    timecode.m_frame_rate = FRAME_RATE_30;  // Default frame rate
    timecode.m_reserved = 0;
    
    return timecode;
}

timecode_t::~timecode_t() {
	_clean_up();
}

void timecode_t::_clean_up() {
}


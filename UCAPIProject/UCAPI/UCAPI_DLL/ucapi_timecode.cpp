#include "pch.h"
#include "ucapi.h"
#include "ucapi_timecode.h"

timecode_t::timecode_t() {
	m_frame_number = 0;
	m_second_number = 0;
	m_minute_number = 0;
	m_hour_number = 0;
	m_frame_rate = FRAME_RATE_60;
	m_drop_frame = 0;
	m_color_frame = 0;
	memset(m_user_bits, 0, sizeof(m_user_bits));
}

void timecode_t::pack_smpte(timecode_t timecode, uint8_t smpte_data[10]) {
	memset(smpte_data, 0, 10);
	
	uint8_t frame_units = timecode.m_frame_number % 10;
	uint8_t frame_tens = timecode.m_frame_number / 10;
	uint8_t sec_units = timecode.m_second_number % 10;
	uint8_t sec_tens = timecode.m_second_number / 10;
	uint8_t min_units = timecode.m_minute_number % 10;
	uint8_t min_tens = timecode.m_minute_number / 10;
	uint8_t hour_units = timecode.m_hour_number % 10;
	uint8_t hour_tens = timecode.m_hour_number / 10;
	
	smpte_data[0] = (frame_units & 0x0F) | ((timecode.m_user_bits[0] & 0x0F) << 4);
	smpte_data[1] = ((frame_tens & 0x03) | (timecode.m_drop_frame ? 0x04 : 0x00) | (timecode.m_color_frame ? 0x08 : 0x00)) | ((timecode.m_user_bits[1] & 0x0F) << 4);
	smpte_data[2] = (sec_units & 0x0F) | ((timecode.m_user_bits[2] & 0x0F) << 4);
	smpte_data[3] = (sec_tens & 0x07) | ((timecode.m_user_bits[3] & 0x0F) << 4);
	smpte_data[4] = (min_units & 0x0F) | ((timecode.m_user_bits[0] & 0x0F) << 4);
	smpte_data[5] = (min_tens & 0x07) | ((timecode.m_user_bits[1] & 0x0F) << 4);
	smpte_data[6] = (hour_units & 0x0F) | ((timecode.m_user_bits[2] & 0x0F) << 4);
	smpte_data[7] = (hour_tens & 0x03) | ((timecode.m_user_bits[3] & 0x0F) << 4);
	
	smpte_data[8] = 0x3F;
	smpte_data[9] = 0xFD;
}

timecode_t timecode_t::unpack_smpte(const uint8_t smpte_data[10]) {
	timecode_t timecode;
	
	uint8_t frame_units = smpte_data[0] & 0x0F;
	uint8_t frame_tens = smpte_data[1] & 0x03;
	uint8_t sec_units = smpte_data[2] & 0x0F;
	uint8_t sec_tens = smpte_data[3] & 0x07;
	uint8_t min_units = smpte_data[4] & 0x0F;
	uint8_t min_tens = smpte_data[5] & 0x07;
	uint8_t hour_units = smpte_data[6] & 0x0F;
	uint8_t hour_tens = smpte_data[7] & 0x03;
	
	timecode.m_frame_number = frame_tens * 10 + frame_units;
	timecode.m_second_number = sec_tens * 10 + sec_units;
	timecode.m_minute_number = min_tens * 10 + min_units;
	timecode.m_hour_number = hour_tens * 10 + hour_units;
	timecode.m_drop_frame = (smpte_data[1] & 0x04) ? 1 : 0;
	timecode.m_color_frame = (smpte_data[1] & 0x08) ? 1 : 0;
	timecode.m_frame_rate = FRAME_RATE_60;
	
	timecode.m_user_bits[0] = (smpte_data[0] >> 4) & 0x0F;
	timecode.m_user_bits[1] = (smpte_data[1] >> 4) & 0x0F;
	timecode.m_user_bits[2] = (smpte_data[2] >> 4) & 0x0F;
	timecode.m_user_bits[3] = (smpte_data[3] >> 4) & 0x0F;
	
	return timecode;
}

uint32_t timecode_t::pack(timecode_t timecode) {
	uint32_t packed = 0;
	packed |= (timecode.m_frame_number & 0xFF);
	packed |= ((timecode.m_second_number & 0x3F) << 8);
	packed |= ((timecode.m_minute_number & 0x3F) << 14);
	packed |= ((timecode.m_hour_number & 0x1F) << 20);
	packed |= ((timecode.m_frame_rate & 0xF) << 25);
	packed |= ((timecode.m_drop_frame & 0x1) << 29);
	packed |= ((timecode.m_color_frame & 0x1) << 30);
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
	timecode.m_color_frame = (packed >> 30) & 0x1;
	memset(timecode.m_user_bits, 0, sizeof(timecode.m_user_bits));
	return timecode;
}

timecode_t::~timecode_t() {
	_clean_up();
}

void timecode_t::_clean_up() {
}

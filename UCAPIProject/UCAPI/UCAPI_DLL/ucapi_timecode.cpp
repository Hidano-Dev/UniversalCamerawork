#include "pch.h"
#include "ucapi.h"
#include "ucapi_timecode.h"

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

timecode_t::~timecode_t() {
	_clean_up();
}

void timecode_t::_clean_up() {
}

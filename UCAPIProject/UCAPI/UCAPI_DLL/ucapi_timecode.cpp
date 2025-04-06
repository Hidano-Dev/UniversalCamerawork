#include "pch.h"
#include "ucapi.h"
#include "ucapi_timecode.h"

timecode_t::timecode_t(const void* dataPtr) {
	m_frame_number = 0;
	m_second_number = 0;
	m_minute_number = 0;
	m_hour_number = 0;
	m_frame_rate = FRAME_RATE_60;
	m_drop_frame = 0;
	m_reserved = 0;

	if (dataPtr == nullptr) {
		return;
	}

	try {
		_read(dataPtr);
	}
	catch (...) {
		_clean_up();
		throw;
	}
}

timecode_t::timecode_t(uint32_t packed) {
	m_frame_number = packed & 0xFF;
	m_second_number = (packed >> 8) & 0x3F;
	m_minute_number = (packed >> 14) & 0x3F;
	m_hour_number = (packed >> 20) & 0x1F;
	m_frame_rate = (packed >> 25) & 0xF;
	m_drop_frame = (packed >> 29) & 0x1;
	m_reserved = (packed >> 30) & 0x3;
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

void timecode_t::_read(const void* dataPtr) {
	const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);

	// frame_numberは1バイト
	m_frame_number = data[0] & 0xFF;
	// second_numberはdata[1]の下位6ビット
	m_second_number = (data[1] & 0x3F);
	// minute_numberはdata[1]の上位2ビットとdata[2]の下位4ビット
	m_minute_number = ((data[1] >> 6) & 0x03) | ((data[2] & 0x0F) << 2);
	// hour_numberはdata[2]の上位4ビットとdata[3]の下位1ビット
	m_hour_number = ((data[2] >> 4) & 0x0F) | ((data[3] & 0x01) << 4);
	// frame_rateはdata[3]の下位5～2ビット
	m_frame_rate = (data[3] >> 2) & 0x0F;
	// drop_frameはdata[3]の上位3ビット目
	m_drop_frame = (data[3] >> 1) & 0x01;
	// reservedはdata[3]の上位2ビット
	m_reserved = (data[3] >> 7) & 0x01;
}

timecode_t::~timecode_t() {
	_clean_up();
}

void timecode_t::_clean_up() {
}

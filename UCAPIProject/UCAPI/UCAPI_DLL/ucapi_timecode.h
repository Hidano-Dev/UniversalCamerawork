#pragma once
#include <string>

class timecode_t {

public:

	timecode_t(const void* dataPtr = nullptr);
	timecode_t(uint32_t packed);
	static uint32_t pack(timecode_t timecode);

private:
	void _read(const void* dataPtr);
	void _clean_up();

	enum {
		FRAME_RATE_23_976 = 0,
		FRAME_RATE_24 = 1,
		FRAME_RATE_25 = 2,
		FRAME_RATE_29_97 = 3,
		FRAME_RATE_30 = 4,
		FRAME_RATE_50 = 5,
		FRAME_RATE_59_94 = 6,
		FRAME_RATE_60 = 7,
		FRAME_RATE_119_88 = 8,
		FRAME_RATE_120 = 9,
		FRAME_RATE_144 = 10,
		FRAME_RATE_239_76 = 11,
		FRAME_RATE_240 = 12,
	};

public:
	~timecode_t();

	uint8_t m_frame_number;
	uint8_t m_second_number;
	uint8_t m_minute_number;
	uint8_t m_hour_number;
	uint8_t m_frame_rate;
	uint8_t m_drop_frame;
	uint8_t m_reserved;
};

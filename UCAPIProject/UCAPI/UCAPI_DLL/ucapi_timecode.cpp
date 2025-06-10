#include "pch.h"
#include "ucapi.h"
#include "ucapi_timecode.h"
#include <cstring>

timecode_t::timecode_t() {
    hour = 0;
    minute = 0;
    second = 0;
    frame = 0;
    std::memset(user_bits, 0, sizeof(user_bits));
    subframe = 0;
}

timecode_t::~timecode_t() {
    _clean_up();
}

void timecode_t::_clean_up() {
}

// BCD変換ヘルパー
static uint8_t to_bcd(uint8_t val) { return ((val / 10) << 4) | (val % 10); }
static uint8_t from_bcd(uint8_t bcd) { return ((bcd >> 4) * 10) + (bcd & 0x0F); }

std::array<uint8_t, 12> timecode_t::pack(const timecode_t& timecode) {
    std::array<uint8_t, 12> data = {0};
    data[0] = to_bcd(timecode.frame);
    data[1] = to_bcd(timecode.second);
    data[2] = to_bcd(timecode.minute);
    data[3] = to_bcd(timecode.hour);
    std::memcpy(&data[4], timecode.user_bits, 4);
    data[8] = (timecode.subframe >> 8) & 0xFF;
    data[9] = timecode.subframe & 0xFF;
    // 10,11バイト目は将来拡張用に0
    return data;
}

timecode_t timecode_t::unpack(const std::array<uint8_t, 12>& data) {
    timecode_t timecode;
    timecode.frame = from_bcd(data[0]);
    timecode.second = from_bcd(data[1]);
    timecode.minute = from_bcd(data[2]);
    timecode.hour = from_bcd(data[3]);
    std::memcpy(timecode.user_bits, &data[4], 4);
    timecode.subframe = (static_cast<uint16_t>(data[8]) << 8) | data[9];
    return timecode;
}

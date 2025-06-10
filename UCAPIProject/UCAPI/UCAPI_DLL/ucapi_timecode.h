#pragma once
#include <string>
#include <array>

class timecode_t {
public:
    timecode_t();
    ~timecode_t();
    // SMPTE 80bit (10byte) + subframe (2byte) 対応
    static std::array<uint8_t, 12> pack(const timecode_t& timecode);
    static timecode_t unpack(const std::array<uint8_t, 12>& data);

    // BCD形式の時分秒フレーム
    uint8_t hour;      // BCD
    uint8_t minute;    // BCD
    uint8_t second;    // BCD
    uint8_t frame;     // BCD
    uint8_t user_bits[4]; // ユーザバイト
    uint16_t subframe; // サブフレーム（ゲームエンジン拡張）

private:
    void _clean_up();
};

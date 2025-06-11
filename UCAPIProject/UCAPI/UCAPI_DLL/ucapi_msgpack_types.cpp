#include "pch.h"
#include "ucapi_msgpack_types.h"
#include "ucapi_timecode.h"
#include <cstring>

void smpte_ltc_msgpack_t::set_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t frame) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_time(hour, minute, second, frame);
    
    memcpy(data.data(), temp_ltc.data, 10);
    
    this->hour = hour;
    this->minute = minute;
    this->second = second;
    this->frame = frame;
}

void smpte_ltc_msgpack_t::set_drop_frame(bool drop_frame) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_drop_frame(drop_frame);
    
    memcpy(data.data(), temp_ltc.data, 10);
    this->drop_frame = drop_frame;
}

void smpte_ltc_msgpack_t::set_color_frame(bool color_frame) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_color_frame(color_frame);
    
    memcpy(data.data(), temp_ltc.data, 10);
    this->color_frame = color_frame;
}

void smpte_ltc_msgpack_t::set_sync_word() {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_sync_word();
    
    memcpy(data.data(), temp_ltc.data, 10);
}

bool smpte_ltc_msgpack_t::is_valid_sync_word() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.is_valid_sync_word();
}

uint8_t smpte_ltc_msgpack_t::get_frame() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.get_frame();
}

uint8_t smpte_ltc_msgpack_t::get_second() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.get_second();
}

uint8_t smpte_ltc_msgpack_t::get_minute() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.get_minute();
}

uint8_t smpte_ltc_msgpack_t::get_hour() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.get_hour();
}

bool smpte_ltc_msgpack_t::get_drop_frame() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.get_drop_frame();
}

bool smpte_ltc_msgpack_t::get_color_frame() const {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, const_cast<uint8_t*>(data.data()), 10);
    
    return temp_ltc.get_color_frame();
}

void smpte_ltc_msgpack_t::set_frame(uint8_t frame) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_frame(frame);
    
    memcpy(data.data(), temp_ltc.data, 10);
    this->frame = frame;
}

void smpte_ltc_msgpack_t::set_second(uint8_t second) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_second(second);
    
    memcpy(data.data(), temp_ltc.data, 10);
    this->second = second;
}

void smpte_ltc_msgpack_t::set_minute(uint8_t minute) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_minute(minute);
    
    memcpy(data.data(), temp_ltc.data, 10);
    this->minute = minute;
}

void smpte_ltc_msgpack_t::set_hour(uint8_t hour) {
    smpte_ltc_t temp_ltc;
    memcpy(temp_ltc.data, data.data(), 10);
    
    temp_ltc.set_hour(hour);
    
    memcpy(data.data(), temp_ltc.data, 10);
    this->hour = hour;
}

#pragma once
#include <string>
#include <cstdint>

// SMPTE 12M Linear Timecode (LTC) 80-bit structure
struct smpte_ltc_t {
    uint8_t data[10];  // 80 bits = 10 bytes
    
    // Constructor
    smpte_ltc_t();
    
    // Destructor
    ~smpte_ltc_t();
    
    // Set timecode values
    void set_frame(uint8_t frame);
    void set_second(uint8_t second);
    void set_minute(uint8_t minute);
    void set_hour(uint8_t hour);
    void set_drop_frame(bool drop_frame);
    void set_color_frame(bool color_frame);
    
    // Get timecode values
    uint8_t get_frame() const;
    uint8_t get_second() const;
    uint8_t get_minute() const;
    uint8_t get_hour() const;
    bool get_drop_frame() const;
    bool get_color_frame() const;
    
    // User bits (32 bits total, 8 fields of 4 bits each)
    void set_user_bits(uint8_t field, uint8_t value);  // field: 0-7, value: 0-15
    uint8_t get_user_bits(uint8_t field) const;
    
    // Clear all data
    void clear();
    
    // Set sync word (automatically called by other methods)
    void set_sync_word();
    
    // Validate sync word
    bool is_valid_sync() const;
    
    // Convenience methods
    void set_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t frame);
    bool is_valid_sync_word() const;
    
private:
    void _clean_up();
    void _set_bcd_value(uint8_t byte_offset, uint8_t bit_offset, uint8_t value, uint8_t max_value);
    uint8_t _get_bcd_value(uint8_t byte_offset, uint8_t bit_offset, uint8_t mask) const;
    void _set_bit(uint8_t bit_position, bool value);
    bool _get_bit(uint8_t bit_position) const;
};

// Legacy timecode class for backward compatibility
class timecode_t {

public:
	timecode_t();

private:
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
	
	// Legacy 32-bit pack/unpack functions (deprecated)
	static uint32_t pack(timecode_t timecode);
	static timecode_t unpack(uint32_t packed);
	
	// New SMPTE LTC functions
	static smpte_ltc_t pack_smpte(timecode_t timecode);
	static timecode_t unpack_smpte(const smpte_ltc_t& ltc);

	uint8_t m_frame_number;
	uint8_t m_second_number;
	uint8_t m_minute_number;
	uint8_t m_hour_number;
	uint8_t m_frame_rate;
	uint8_t m_drop_frame;
	uint8_t m_reserved;
};

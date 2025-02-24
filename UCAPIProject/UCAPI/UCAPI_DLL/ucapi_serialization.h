#ifndef UCAPI_SERIALIZATION_H_
#define UCAPI_SERIALIZATION_H_

#include <ostream>
#include <cstdint>
#include <vector>
#include "ucapi_deserialization.h"

/// <summary>
/// Writes the UCAPI header structure to the given output stream.
/// </summary>
/// <param name="header">Pointer to the UCAPI header structure.</param>
/// <param name="os">Output stream to write binary data to.</param>
void write_ucapi_header(const ucapi_serialization_t::header_t* header, std::ostream& os);

/// <summary>
/// Writes the UCAPI timecode structure to the given output stream.
/// The timecode fields are bit-packed (7, 6, 6, 5, 8 bits) in big-endian order.
/// </summary>
/// <param name="timecode">Pointer to the UCAPI timecode structure.</param>
/// <param name="os">Output stream to write binary data to.</param>
void write_ucapi_timecode(const ucapi_serialization_t::timecode_t* timecode, std::ostream& os);

/// <summary>
/// Writes a single UCAPI record structure to the given output stream.
/// </summary>
/// <param name="record">Pointer to the UCAPI record structure.</param>
/// <param name="os">Output stream to write binary data to.</param>
void write_ucapi_record(const ucapi_serialization_t::record_t* record, std::ostream& os);

/// <summary>
/// Writes the entire UCAPI structure (header and all records) to the given output stream.
/// </summary>
/// <param name="ucapi">Pointer to the root UCAPI structure.</param>
/// <param name="os">Output stream to write binary data to.</param>
void write_ucapi(const ucapi_serialization_t* ucapi, std::ostream& os);

#endif  // UCAPI_SERIALIZATION_H_

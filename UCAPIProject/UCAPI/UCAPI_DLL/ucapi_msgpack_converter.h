#pragma once
#include "ucapi.h"
#include "ucapi_msgpack_types.h"

ucapi_t* convert_to_ucapi(const ucapi_msgpack_t& msgpack_obj);
ucapi_msgpack_t convert_to_msgpack(const ucapi_t* ucapi_obj);

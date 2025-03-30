#pragma once
#include "ucapi.h"
#include "ucapi_serializer.h"
#include <ostream>

void write_ucapi(const ucapi_t* obj, std::ostream& os);

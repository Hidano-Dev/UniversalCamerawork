#pragma once
#include <vector>
#include <cstdint>
#include <windows.h>
#include "../ucapi.h"

namespace ucapi {
    using CameraState = ucapi_t::record_t;

    class IMessageSerializer {
    public:
        virtual ~IMessageSerializer() = default;

        virtual HRESULT Serialize(const CameraState& src, std::vector<uint8_t>& out) noexcept = 0;

        virtual HRESULT Deserialize(const uint8_t* data, size_t size, CameraState& out) noexcept = 0;
    };
}

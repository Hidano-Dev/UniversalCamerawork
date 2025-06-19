#include "pch.h"
#include "include/ucapi_dummy_serializer.h"
#include <cstring>

namespace ucapi {
    HRESULT DummySerializer::Serialize(const CameraState& src, std::vector<uint8_t>& out) noexcept {
        try {
            out.clear();
            return S_OK;
        }
        catch (...) {
            return E_FAIL;
        }
    }

    HRESULT DummySerializer::Deserialize(const uint8_t* data, size_t size, CameraState& out) noexcept {
        try {
            std::memset(&out, 0, sizeof(CameraState));
            return S_OK;
        }
        catch (...) {
            return E_FAIL;
        }
    }
}

#include "pch.h"
#include "ucapi_dll.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

// CRC16 zero length returns init value
TEST(UcapiDll_CalcCRC16, ZeroLength) {
  uint16_t crc = UCAPI_CalcCRC16(nullptr, 0, 0x1021, 0xFFFF);
  EXPECT_EQ(crc, 0xFFFF);
}

// CRC16 deterministic for same input
TEST(UcapiDll_CalcCRC16, Deterministic) {
  uint8_t data[] = {0xDE, 0xAD, 0xBE, 0xEF};
  uint16_t crc1 = UCAPI_CalcCRC16(data, sizeof(data), 0x1021, 0xFFFF);
  uint16_t crc2 = UCAPI_CalcCRC16(data, sizeof(data), 0x1021, 0xFFFF);
  EXPECT_EQ(crc1, crc2);
}

// FreeBuffer should handle nullptr without crash
TEST(UcapiDll_FreeBuffer, Null) {
  UCAPI_FreeBuffer(nullptr);
  SUCCEED();
}

// TODO: Add tests for Serialize and Deserialize
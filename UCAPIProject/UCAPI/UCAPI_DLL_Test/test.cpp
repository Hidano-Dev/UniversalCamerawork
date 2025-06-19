#include "pch.h"
#include "ucapi_dll.h"

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

// Serialize 異常系: null オブジェクト
TEST(UcapiDll_Serialize, NullObject) {
  uint8_t* buf = nullptr;
  size_t sz = 0;
  int res = UCAPI_Serialize(nullptr, &buf, &sz);
  EXPECT_NE(res, 0);
}

// Serialize 正常系
TEST(UcapiDll_Serialize, ValidObject) {
  // 正常系: C API 経由でオブジェクト生成
  ucapi_t* obj = UCAPI_Create();
  uint8_t* buf = nullptr;
  size_t sz = 0;
  int res = UCAPI_Serialize(obj, &buf, &sz);
  EXPECT_EQ(res, 0);
  EXPECT_GT(sz, 0u);
  EXPECT_NE(buf, nullptr);
  UCAPI_FreeBuffer(buf);
  UCAPI_Destroy(obj);
}

// Deserialize 異常系: null バッファ
TEST(UcapiDll_Deserialize, NullBuffer) {
  ucapi_t* obj = UCAPI_Deserialize(nullptr, 0);
  EXPECT_EQ(obj, nullptr);
}

// Deserialize 準正常系: 不正データ
TEST(UcapiDll_Deserialize, InvalidData) {
  uint8_t bad[] = {0xFF, 0x00, 0xAA};
  ucapi_t* obj = UCAPI_Deserialize(bad, sizeof(bad));
  EXPECT_EQ(obj, nullptr);
}

// Deserialize 正常系
TEST(UcapiDll_Deserialize, ValidBuffer) {
  // 正常系: テスト用オブジェクトの生成
  ucapi_t* base = UCAPI_Create();
  uint8_t* buf = nullptr;
  size_t sz = 0;
  ASSERT_EQ(UCAPI_Serialize(base, &buf, &sz), 0);
  ucapi_t* obj = UCAPI_Deserialize(buf, sz);
  EXPECT_NE(obj, nullptr);
  UCAPI_FreeBuffer(buf);
  UCAPI_Destroy(base);
  UCAPI_Destroy(obj);
}
// オブジェクト生成・破棄の正常系
TEST(UcapiDll_CreateDestroy, Basic) {
  ucapi_t* obj = UCAPI_Create();
  EXPECT_NE(obj, nullptr);
  UCAPI_Destroy(obj);
}
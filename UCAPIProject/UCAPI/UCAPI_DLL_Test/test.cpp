#include "pch.h"
#include "ucapi_dll.h"

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

// Deserialize 異常系: null バッファ
TEST(UcapiDll_Deserialize, NullBuffer) {
	ucapi_t* obj = UCAPI_Deserialize(nullptr, 0);
	EXPECT_EQ(obj, nullptr);
}

// Deserialize 準正常系: 不正データ
TEST(UcapiDll_Deserialize, InvalidData) {
	uint8_t bad[] = { 0xFF, 0x00, 0xAA };
	ucapi_t* obj = UCAPI_Deserialize(bad, sizeof(bad));
	EXPECT_EQ(obj, nullptr);
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

// Serialize 異常系: null オブジェクト
TEST(UcapiDll_Serialize, NullObject) {
	uint8_t* buf = nullptr;
	size_t sz = 0;
	int res = UCAPI_Serialize(nullptr, &buf, &sz);
	EXPECT_NE(res, 0);
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

// CRC16-CCITT-FALSE standard test vector: "123456789" -> 0x29B1
// Reference: ITU-T V.41, CRC-16/CCITT-FALSE (poly=0x1021, init=0xFFFF, refIn=false, refOut=false, xorOut=0x0000)
TEST(UcapiDll_CalcCRC16, StandardTestVector_123456789) {
  // ASCII "123456789" = 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39
  uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
  uint16_t crc = UCAPI_CalcCRC16(data, sizeof(data), 0x1021, 0xFFFF);
  EXPECT_EQ(crc, 0x29B1) << "CRC16-CCITT-FALSE for '123456789' should be 0x29B1";
}

// CRC16 single byte test vectors
TEST(UcapiDll_CalcCRC16, SingleByteTestVectors) {
  // Single byte 0x00: init 0xFFFF, process one 0x00 byte
  // Reference calculated value
  uint8_t data_00[] = {0x00};
  uint16_t crc_00 = UCAPI_CalcCRC16(data_00, sizeof(data_00), 0x1021, 0xFFFF);
  EXPECT_EQ(crc_00, 0xE1F0) << "CRC16-CCITT-FALSE for single 0x00 should be 0xE1F0";

  // Single byte 0xFF
  uint8_t data_ff[] = {0xFF};
  uint16_t crc_ff = UCAPI_CalcCRC16(data_ff, sizeof(data_ff), 0x1021, 0xFFFF);
  EXPECT_EQ(crc_ff, 0xFF00) << "CRC16-CCITT-FALSE for single 0xFF should be 0xFF00";
}

// CRC16 with different polynomial (CRC-16/XMODEM: init=0x0000)
TEST(UcapiDll_CalcCRC16, XmodemTestVector) {
  // CRC-16/XMODEM: poly=0x1021, init=0x0000
  // "123456789" -> 0x31C3
  uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
  uint16_t crc = UCAPI_CalcCRC16(data, sizeof(data), 0x1021, 0x0000);
  EXPECT_EQ(crc, 0x31C3) << "CRC16-XMODEM for '123456789' should be 0x31C3";
}

// CRC16 multiple bytes all zeros
TEST(UcapiDll_CalcCRC16, AllZerosTestVector) {
  uint8_t data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t crc = UCAPI_CalcCRC16(data, sizeof(data), 0x1021, 0xFFFF);
  EXPECT_EQ(crc, 0x84C0) << "CRC16-CCITT-FALSE for four 0x00 bytes should be 0x84C0";
}

// CRC16 multiple bytes all ones (0xFF)
TEST(UcapiDll_CalcCRC16, AllOnesTestVector) {
  uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF};
  uint16_t crc = UCAPI_CalcCRC16(data, sizeof(data), 0x1021, 0xFFFF);
  EXPECT_EQ(crc, 0x1D0F) << "CRC16-CCITT-FALSE for four 0xFF bytes should be 0x1D0F";
}

// FreeBuffer should handle nullptr without crash
TEST(UcapiDll_FreeBuffer, Null) {
  UCAPI_FreeBuffer(nullptr);
  SUCCEED();
}

// オブジェクト生成・破棄の正常系
TEST(UcapiDll_CreateDestroy, Basic) {
  ucapi_t* obj = UCAPI_Create();
  EXPECT_NE(obj, nullptr);
  UCAPI_Destroy(obj);
}
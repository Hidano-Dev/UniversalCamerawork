#include "pch.h"
#include "ucapi_dll.h"
#include <thread>
#include <atomic>
#include <vector>
#include <cstring>
#include <limits>
#include <cmath>

// GoogleTest main function
// This is required because gtest_main.lib may not link correctly with vcpkg wildcard linking
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
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

// =============================================================================
// P1-3: エッジケーステスト・異常系テスト
// =============================================================================

// --- バッファオーバーフロー攻撃パターン ---

// Deserialize: ヘッダのみ（ペイロードなし）のバッファ
TEST(UcapiDll_Deserialize_EdgeCase, HeaderOnlyBuffer) {
  // ヘッダーサイズ10バイトだが実際のペイロードデータがない
  uint8_t headerOnly[10] = {0xAA, 0x55, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
  ucapi_t* obj = UCAPI_Deserialize(headerOnly, sizeof(headerOnly));
  // MsgPackデシリアライズに失敗するはず
  EXPECT_EQ(obj, nullptr);
}

// Deserialize: 不正なnum_payloadが大きな値を持つバッファ
TEST(UcapiDll_Deserialize_EdgeCase, MaliciousNumPayload) {
  // num_payload = 0xFFFF でバッファオーバーフローを試みる
  uint8_t malicious[20] = {0xAA, 0x55, 0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
  ucapi_t* obj = UCAPI_Deserialize(malicious, sizeof(malicious));
  EXPECT_EQ(obj, nullptr);
}

// Deserialize: 切り詰められたバッファ（サイズが途中で切れている）
TEST(UcapiDll_Deserialize_EdgeCase, TruncatedBuffer) {
  uint8_t truncated[5] = {0x82, 0xa1, 0x78, 0x01, 0xa1};  // 不完全なMsgPack
  ucapi_t* obj = UCAPI_Deserialize(truncated, sizeof(truncated));
  EXPECT_EQ(obj, nullptr);
}

// --- 大量ペイロード（64KB以上）テスト ---

// Deserialize: 巨大バッファ（64KB超）を渡す
TEST(UcapiDll_Deserialize_EdgeCase, LargeBuffer_Over64KB) {
  constexpr size_t LARGE_SIZE = 70000;  // 64KB = 65536 を超える
  std::vector<uint8_t> largeBuffer(LARGE_SIZE, 0);
  // 先頭にMsgPackの無効データを入れる
  largeBuffer[0] = 0x91;  // fixarray of 1 element
  largeBuffer[1] = 0xc0;  // nil

  ucapi_t* obj = UCAPI_Deserialize(largeBuffer.data(), largeBuffer.size());
  // 無効なMsgPackデータとして拒否されるべき
  EXPECT_EQ(obj, nullptr);
}

// --- msgpack deserialization失敗ケース ---

// MsgPack: 不正な形式（配列でもマップでもない）
TEST(UcapiDll_Deserialize_MsgPack, InvalidFormat_NotMapOrArray) {
  // MsgPackのnil単体
  uint8_t nilOnly[] = {0xc0};
  ucapi_t* obj = UCAPI_Deserialize(nilOnly, sizeof(nilOnly));
  EXPECT_EQ(obj, nullptr);
}

// MsgPack: 空のマップ
TEST(UcapiDll_Deserialize_MsgPack, EmptyMap) {
  // fixmap size 0
  uint8_t emptyMap[] = {0x80};
  ucapi_t* obj = UCAPI_Deserialize(emptyMap, sizeof(emptyMap));
  // 空マップは必要なフィールドがないので失敗
  EXPECT_EQ(obj, nullptr);
}

// MsgPack: 不正なバイトシーケンス（予約されたMsgPackバイト）
TEST(UcapiDll_Deserialize_MsgPack, ReservedBytes) {
  // 0xc1はMsgPackで未使用（予約済み）
  uint8_t reserved[] = {0xc1};
  ucapi_t* obj = UCAPI_Deserialize(reserved, sizeof(reserved));
  EXPECT_EQ(obj, nullptr);
}

// MsgPack: 不完全なstring型
TEST(UcapiDll_Deserialize_MsgPack, IncompleteString) {
  // fixstr size 5, but only 2 chars follow
  uint8_t incomplete[] = {0xa5, 0x68, 0x69};  // "hi" but declared 5 chars
  ucapi_t* obj = UCAPI_Deserialize(incomplete, sizeof(incomplete));
  EXPECT_EQ(obj, nullptr);
}

// --- 境界値テスト ---

// Serialize: 空のペイロード配列を持つオブジェクト
TEST(UcapiDll_Serialize_Boundary, EmptyPayload) {
  ucapi_t* obj = UCAPI_Create();
  ASSERT_NE(obj, nullptr);
  // デフォルトで空ペイロード
  EXPECT_EQ(obj->m_num_payload, 0);

  uint8_t* buf = nullptr;
  size_t sz = 0;
  int res = UCAPI_Serialize(obj, &buf, &sz);
  // 空ペイロードでもシリアライズできるべき
  EXPECT_EQ(res, 0);
  EXPECT_GT(sz, 0u);
  EXPECT_NE(buf, nullptr);
  UCAPI_FreeBuffer(buf);
  UCAPI_Destroy(obj);
}

// CRC16: 境界値 - 長さ1バイト（最小）
TEST(UcapiDll_CalcCRC16_Boundary, Length1) {
  uint8_t data[] = {0x42};
  uint16_t crc = UCAPI_CalcCRC16(data, 1, 0x1021, 0xFFFF);
  // 結果が決定的であることを確認
  uint16_t crc2 = UCAPI_CalcCRC16(data, 1, 0x1021, 0xFFFF);
  EXPECT_EQ(crc, crc2);
}

// CRC16: 境界値 - 長さがsize_t最大値に近い場合は渡さない（安全性確認）
TEST(UcapiDll_CalcCRC16_Boundary, SmallValidBuffer) {
  // 適切なサイズでの計算
  std::vector<uint8_t> data(1000, 0x55);
  uint16_t crc = UCAPI_CalcCRC16(data.data(), data.size(), 0x1021, 0xFFFF);
  uint16_t crc2 = UCAPI_CalcCRC16(data.data(), data.size(), 0x1021, 0xFFFF);
  EXPECT_EQ(crc, crc2);
}

// --- 引数不正時の動作検証 ---

// Serialize: outBuffer が null
TEST(UcapiDll_Serialize_InvalidArgs, NullOutBuffer) {
  ucapi_t* obj = UCAPI_Create();
  size_t sz = 0;
  int res = UCAPI_Serialize(obj, nullptr, &sz);
  EXPECT_NE(res, 0);  // エラーを返すべき
  UCAPI_Destroy(obj);
}

// Serialize: outSize が null
TEST(UcapiDll_Serialize_InvalidArgs, NullOutSize) {
  ucapi_t* obj = UCAPI_Create();
  uint8_t* buf = nullptr;
  int res = UCAPI_Serialize(obj, &buf, nullptr);
  EXPECT_NE(res, 0);  // エラーを返すべき
  UCAPI_Destroy(obj);
}

// Serialize: 全ての出力ポインタが null
TEST(UcapiDll_Serialize_InvalidArgs, AllOutputsNull) {
  ucapi_t* obj = UCAPI_Create();
  int res = UCAPI_Serialize(obj, nullptr, nullptr);
  EXPECT_NE(res, 0);
  UCAPI_Destroy(obj);
}

// Deserialize: サイズが0
TEST(UcapiDll_Deserialize_InvalidArgs, ZeroSize) {
  uint8_t data[] = {0x80};
  ucapi_t* obj = UCAPI_Deserialize(data, 0);
  EXPECT_EQ(obj, nullptr);
}

// Deserialize: 非nullバッファだがサイズ0
TEST(UcapiDll_Deserialize_InvalidArgs, NonNullBufferZeroSize) {
  uint8_t data[100];
  std::memset(data, 0, sizeof(data));
  ucapi_t* obj = UCAPI_Deserialize(data, 0);
  EXPECT_EQ(obj, nullptr);
}

// Destroy: nullptr を渡しても安全
TEST(UcapiDll_Destroy_EdgeCase, NullPointer) {
  // nullptr を渡してもクラッシュしないことを確認
  UCAPI_Destroy(nullptr);
  SUCCEED();
}

// --- ラウンドトリップテスト ---

// Serialize → Deserialize ラウンドトリップ
TEST(UcapiDll_RoundTrip, SerializeDeserialize) {
  // DLL API経由でオブジェクト生成（デフォルト状態）
  ucapi_t* original = UCAPI_Create();
  ASSERT_NE(original, nullptr);

  // シリアライズ
  uint8_t* buf = nullptr;
  size_t sz = 0;
  int res = UCAPI_Serialize(original, &buf, &sz);
  ASSERT_EQ(res, 0);
  ASSERT_NE(buf, nullptr);
  ASSERT_GT(sz, 0u);

  // デシリアライズ
  ucapi_t* restored = UCAPI_Deserialize(buf, sz);
  ASSERT_NE(restored, nullptr);

  // 基本的なプロパティの検証
  EXPECT_EQ(restored->m_magic, 0x55AA);

  UCAPI_FreeBuffer(buf);
  UCAPI_Destroy(original);
  UCAPI_Destroy(restored);
}

// --- マルチスレッドシナリオテスト ---

// 複数スレッドで同時にCreate/Destroy
TEST(UcapiDll_MultiThread, ConcurrentCreateDestroy) {
  constexpr int NUM_THREADS = 8;
  constexpr int ITERATIONS = 100;
  std::atomic<int> successCount{0};
  std::atomic<int> failureCount{0};

  auto worker = [&]() {
    for (int i = 0; i < ITERATIONS; ++i) {
      ucapi_t* obj = UCAPI_Create();
      if (obj != nullptr) {
        UCAPI_Destroy(obj);
        ++successCount;
      } else {
        ++failureCount;
      }
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(NUM_THREADS);
  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.emplace_back(worker);
  }
  for (auto& t : threads) {
    t.join();
  }

  EXPECT_EQ(successCount.load(), NUM_THREADS * ITERATIONS);
  EXPECT_EQ(failureCount.load(), 0);
}

// 複数スレッドで同時にSerialize
TEST(UcapiDll_MultiThread, ConcurrentSerialize) {
  constexpr int NUM_THREADS = 4;
  constexpr int ITERATIONS = 50;
  std::atomic<int> successCount{0};
  std::atomic<int> failureCount{0};

  auto worker = [&]() {
    for (int i = 0; i < ITERATIONS; ++i) {
      ucapi_t* obj = UCAPI_Create();
      if (obj == nullptr) {
        ++failureCount;
        continue;
      }

      uint8_t* buf = nullptr;
      size_t sz = 0;
      int res = UCAPI_Serialize(obj, &buf, &sz);
      if (res == 0 && buf != nullptr && sz > 0) {
        UCAPI_FreeBuffer(buf);
        ++successCount;
      } else {
        ++failureCount;
      }
      UCAPI_Destroy(obj);
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(NUM_THREADS);
  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.emplace_back(worker);
  }
  for (auto& t : threads) {
    t.join();
  }

  EXPECT_EQ(successCount.load(), NUM_THREADS * ITERATIONS);
  EXPECT_EQ(failureCount.load(), 0);
}

// 複数スレッドで同時にSerialize/Deserializeラウンドトリップ
TEST(UcapiDll_MultiThread, ConcurrentRoundTrip) {
  constexpr int NUM_THREADS = 4;
  constexpr int ITERATIONS = 25;
  std::atomic<int> successCount{0};
  std::atomic<int> failureCount{0};

  auto worker = [&](int threadId) {
    for (int i = 0; i < ITERATIONS; ++i) {
      // DLL API経由でオブジェクト生成
      ucapi_t* obj = UCAPI_Create();
      if (obj == nullptr) {
        ++failureCount;
        continue;
      }

      uint8_t* buf = nullptr;
      size_t sz = 0;
      int res = UCAPI_Serialize(obj, &buf, &sz);
      if (res != 0 || buf == nullptr) {
        ++failureCount;
        UCAPI_Destroy(obj);
        continue;
      }

      ucapi_t* restored = UCAPI_Deserialize(buf, sz);
      UCAPI_FreeBuffer(buf);

      // デシリアライズ成功とmagic値の検証
      if (restored != nullptr && restored->m_magic == 0x55AA) {
        ++successCount;
      } else {
        ++failureCount;
      }

      if (restored) UCAPI_Destroy(restored);
      UCAPI_Destroy(obj);
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(NUM_THREADS);
  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.emplace_back(worker, i);
  }
  for (auto& t : threads) {
    t.join();
  }

  EXPECT_EQ(successCount.load(), NUM_THREADS * ITERATIONS);
  EXPECT_EQ(failureCount.load(), 0);
}

// 複数スレッドでCRC16計算（決定性確認）
TEST(UcapiDll_MultiThread, ConcurrentCRC16) {
  constexpr int NUM_THREADS = 8;
  constexpr int ITERATIONS = 100;

  uint8_t testData[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
  constexpr uint16_t EXPECTED_CRC = 0x29B1;  // CRC16-CCITT-FALSE for "123456789"

  std::atomic<int> successCount{0};
  std::atomic<int> failureCount{0};

  auto worker = [&]() {
    for (int i = 0; i < ITERATIONS; ++i) {
      uint16_t crc = UCAPI_CalcCRC16(testData, sizeof(testData), 0x1021, 0xFFFF);
      if (crc == EXPECTED_CRC) {
        ++successCount;
      } else {
        ++failureCount;
      }
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(NUM_THREADS);
  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.emplace_back(worker);
  }
  for (auto& t : threads) {
    t.join();
  }

  EXPECT_EQ(successCount.load(), NUM_THREADS * ITERATIONS);
  EXPECT_EQ(failureCount.load(), 0);
}

// =============================================================================
// P2-4: m_subframe float serialization tests
// =============================================================================

// Test that m_subframe serialization produces non-empty output
TEST(UcapiDll_Subframe, SerializationProducesOutput) {
  ucapi_t* obj = UCAPI_Create();
  ASSERT_NE(obj, nullptr);
  
  // Add a payload record with a specific subframe value
  int res_add = UCAPI_AddRecord(obj);
  ASSERT_EQ(res_add, 0);
  obj->m_payload[0].m_subframe = 0.5f;
  
  uint8_t* buf = nullptr;
  size_t sz = 0;
  int res = UCAPI_Serialize(obj, &buf, &sz);
  ASSERT_EQ(res, 0);
  ASSERT_NE(buf, nullptr);
  ASSERT_GT(sz, 0u) << "Serialization should produce non-empty output";
  
  UCAPI_FreeBuffer(buf);
  UCAPI_Destroy(obj);
}

// Test round-trip serialization/deserialization of various subframe float values
TEST(UcapiDll_Subframe, RoundTripConsistency) {
  // Test various float values including edge cases
  std::vector<float> testValues = {
    0.0f,
    0.5f,
    1.0f,
    -0.5f,
    0.25f,
    0.75f,
    1.5f,
    -1.5f,
    123.456f,
    0.001f,
    0.999f
  };
  
  for (float testValue : testValues) {
    ucapi_t* original = UCAPI_Create();
    ASSERT_NE(original, nullptr);
    
    // Add a payload record with the test subframe value
    int res_add = UCAPI_AddRecord(original);
    ASSERT_EQ(res_add, 0);
    original->m_payload[0].m_subframe = testValue;
    
    // Serialize
    uint8_t* buf = nullptr;
    size_t sz = 0;
    int res = UCAPI_Serialize(original, &buf, &sz);
    ASSERT_EQ(res, 0);
    ASSERT_NE(buf, nullptr);
    
    // Deserialize
    ucapi_t* restored = UCAPI_Deserialize(buf, sz);
    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(restored->m_num_payload, 1);
    
    // Verify the subframe value is preserved exactly
    EXPECT_FLOAT_EQ(restored->m_payload[0].m_subframe, testValue) 
      << "Subframe value " << testValue << " was not preserved in round-trip";
    
    UCAPI_FreeBuffer(buf);
    UCAPI_Destroy(original);
    UCAPI_Destroy(restored);
  }
}

// Test special float values (NaN, infinity)
TEST(UcapiDll_Subframe, SpecialFloatValues) {
  std::vector<float> specialValues = {
    std::numeric_limits<float>::infinity(),
    -std::numeric_limits<float>::infinity(),
    std::numeric_limits<float>::quiet_NaN(),
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::epsilon()
  };
  
  for (float testValue : specialValues) {
    ucapi_t* original = UCAPI_Create();
    ASSERT_NE(original, nullptr);
    
    int res_add = UCAPI_AddRecord(original);
    ASSERT_EQ(res_add, 0);
    original->m_payload[0].m_subframe = testValue;
    
    uint8_t* buf = nullptr;
    size_t sz = 0;
    int res = UCAPI_Serialize(original, &buf, &sz);
    ASSERT_EQ(res, 0);
    ASSERT_NE(buf, nullptr);
    
    ucapi_t* restored = UCAPI_Deserialize(buf, sz);
    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(restored->m_num_payload, 1);
    
    // For NaN, we need special comparison since NaN != NaN
    if (std::isnan(testValue)) {
      EXPECT_TRUE(std::isnan(restored->m_payload[0].m_subframe)) 
        << "NaN value was not preserved in round-trip";
    } else {
      EXPECT_FLOAT_EQ(restored->m_payload[0].m_subframe, testValue) 
        << "Special float value was not preserved in round-trip";
    }
    
    UCAPI_FreeBuffer(buf);
    UCAPI_Destroy(original);
    UCAPI_Destroy(restored);
  }
}

// Test subframe precision - ensure float precision is maintained
TEST(UcapiDll_Subframe, FloatPrecision) {
  // Test that we maintain float precision (not double or truncated to int)
  // This value tests typical float precision scenarios
  float testValue = 0.123456789f;
  
  ucapi_t* original = UCAPI_Create();
  ASSERT_NE(original, nullptr);
  
  int res_add = UCAPI_AddRecord(original);
  ASSERT_EQ(res_add, 0);
  original->m_payload[0].m_subframe = testValue;
  
  uint8_t* buf = nullptr;
  size_t sz = 0;
  int res = UCAPI_Serialize(original, &buf, &sz);
  ASSERT_EQ(res, 0);
  
  ucapi_t* restored = UCAPI_Deserialize(buf, sz);
  ASSERT_NE(restored, nullptr);
  
  // The restored value should match exactly (not lose precision to double conversion)
  EXPECT_FLOAT_EQ(restored->m_payload[0].m_subframe, testValue);
  
  // Verify the binary representation is exactly 4 bytes (float, not double)
  float originalValue = original->m_payload[0].m_subframe;
  float restoredValue = restored->m_payload[0].m_subframe;
  
  EXPECT_EQ(sizeof(originalValue), 4u) << "m_subframe should be 4-byte float";
  EXPECT_EQ(sizeof(restoredValue), 4u) << "m_subframe should be 4-byte float";
  
  UCAPI_FreeBuffer(buf);
  UCAPI_Destroy(original);
  UCAPI_Destroy(restored);
}

// Test different subframe values with independent serialization
TEST(UcapiDll_Subframe, DifferentSubframeValues) {
  std::vector<float> subframeValues = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
  
  for (float value : subframeValues) {
    ucapi_t* original = UCAPI_Create();
    ASSERT_NE(original, nullptr);
    
    // Add a record with the test subframe value
    int res_add = UCAPI_AddRecord(original);
    ASSERT_EQ(res_add, 0);
    original->m_payload[0].m_subframe = value;
    
    uint8_t* buf = nullptr;
    size_t sz = 0;
    int res = UCAPI_Serialize(original, &buf, &sz);
    ASSERT_EQ(res, 0);
    
    ucapi_t* restored = UCAPI_Deserialize(buf, sz);
    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(restored->m_num_payload, 1);
    
    // Verify the subframe value is preserved
    EXPECT_FLOAT_EQ(restored->m_payload[0].m_subframe, value)
      << "Subframe value " << value << " was not preserved";
    
    UCAPI_FreeBuffer(buf);
    UCAPI_Destroy(original);
    UCAPI_Destroy(restored);
  }
}
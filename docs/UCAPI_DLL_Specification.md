# UCAPI_DLL プロジェクト仕様書

最終更新日: 2026年4月12日

---

## 1. プロジェクト概要

- プロジェクト名：UniversalCamerawork API (UCAPI_DLL)
- 目的：複数プラットフォーム（Unity / Unreal / Blender 等）間で、カメラ状態情報（位置、回転、光学パラメータ、タイムコード等）を共通フォーマットで授受するための C++ DLL を提供する。
- 言語／環境：C++17 / MSVC v143 / Visual Studio 2022 / Windows x64
- 出力形式：MessagePack（推奨）／独自バイナリ（Legacy 経路、CRC16 整合性チェック付き）
- 依存：vcpkg manifest モード（msgpack-c）、GoogleTest（テスト）

## 2. コア機能

1. カメラ状態レコード型 (`ucapi_record_flat_t`)
   - フレーム参照: `camera_no`, `commands`（ビットフラグ）, `timecode`（32bit パック）, `subframe`, `packet_no`
   - 位置・姿勢: `eye_position` / `look_vector` / `up_vector` の各 (right, up, forward) 成分（メートル単位）
   - 光学パラメータ: `focal_length_mm`, `aspect_ratio`, `focus_distance_m`, `aperture`
   - センサ／クリップ: `sensor_size_(width|height)_mm`, `near_clip_m`, `far_clip_m`
   - レンズシフト／ディストーション: `lens_shift_(horizontal|vertical)_ratio`, `lens_distortion_radial_coefficients_(k1|k2)`, `lens_distortion_center_point_(right|up)_mm`
2. MessagePack シリアライズ／デシリアライズ（推奨経路）
3. 独自バイナリ形式シリアライズ（Header 10 byte + Record × N）／CRC16-CCITT 整合性チェック（Legacy 経路）
4. タイムコード処理（14 種フレームレート対応、ドロップフレーム対応）
5. シリアライザのプラグイン式切替（Factory + Strategy）
6. Singleton ロガー（リングバッファ最大 256 エントリ、5 段階レベル、スレッドセーフ）
7. DLL エクスポート API（オブジェクト操作・C 互換 Flat Record 操作・ロギング）

## 3. システム構成とモジュール

```
UCAPI_DLL
├─ CoreAPI                     // ucapi.h / ucapi.cpp                       (ucapi_t, record_t, CRC16)
├─ DLL Export Layer            // ucapi_dll.h / ucapi_dll.cpp               (C 互換エクスポート関数群)
├─ SerializerInterface         // include/ucapi_message_serializer.h        (IMessageSerializer)
├─ SerializerFactoryInterface  // include/ucapi_message_serializer_factory.h
├─ SerializerFactoryImpl       // ucapi_message_serializer_factory_impl.*   (Factory 実装)
├─ MsgPackSerializer           // ucapi_msgpack_serializer.*                (MessagePack 実装)
├─ MsgPackTypes                // ucapi_msgpack_types.h                     (MessagePack 用型定義)
├─ BinarySerializerUtility     // ucapi_serializer_utility.h / .cpp         (独自バイナリ実装)
├─ Timecode                    // ucapi_timecode.h / .cpp                   (timecode_t pack/unpack)
├─ Config                      // ucapi_config.h / .cpp                     (Singleton 設定/ファクトリ取得)
└─ Logger                      // ucapi_logger.h / .cpp                     (Singleton ロガー)
```

> **DLL 範囲外**:
> - Transport 層（ローカル IPC / ネットワーク通信） — 各プラットフォーム側プラグインで実装
> - 各 DCC ツール／ゲームエンジン用プラグイン（Unity プラグイン `UCAPI4Unity` は別パッケージ、Unreal/Blender/Maya は今後検討）
> - FlatBuffers 対応シリアライザ（P2-8 として保留中）

## 4. インターフェース仕様

### 4.1 推奨 API — C 互換 Flat Record

外部言語バインディング（C# P/Invoke、Python ctypes 等）を想定した POD ベースの API。

```cpp
#pragma pack(push, 1)
typedef struct ucapi_record_flat_t {
    uint32_t camera_no;
    uint16_t commands;
    uint32_t timecode;
    float    subframe;
    uint8_t  packet_no;
    float    eye_position_right_m;
    float    eye_position_up_m;
    float    eye_position_forward_m;
    float    look_vector_right_m;
    float    look_vector_up_m;
    float    look_vector_forward_m;
    float    up_vector_right_m;
    float    up_vector_up_m;
    float    up_vector_forward_m;
    float    focal_length_mm;
    float    aspect_ratio;
    float    focus_distance_m;
    float    aperture;
    float    sensor_size_width_mm;
    float    sensor_size_height_mm;
    float    near_clip_m;
    float    far_clip_m;
    float    lens_shift_horizontal_ratio;
    float    lens_shift_vertical_ratio;
    float    lens_distortion_radial_coefficients_k1;
    float    lens_distortion_radial_coefficients_k2;
    float    lens_distortion_center_point_right_mm;
    float    lens_distortion_center_point_up_mm;
} ucapi_record_flat_t;
#pragma pack(pop)

UCAPI_API int  UCAPI_SerializeRecord(const ucapi_record_flat_t* record,
                                     uint8_t** outBuffer, size_t* outSize);
UCAPI_API int  UCAPI_DeserializeRecord(const uint8_t* buffer, size_t bufferSize,
                                       ucapi_record_flat_t* outRecord);
UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer);
```

### 4.2 オブジェクト API（Legacy / C++ 内部利用）

C++ クラス `ucapi_t` を直接扱う API。互換性のため残置。

```cpp
UCAPI_API ucapi_t* UCAPI_Create();
UCAPI_API void     UCAPI_Destroy(ucapi_t* obj);
UCAPI_API int      UCAPI_AddRecord(ucapi_t* obj);

UCAPI_API int      UCAPI_Serialize(ucapi_t* obj, uint8_t** outBuffer, size_t* outSize);
UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount);

UCAPI_API uint16_t UCAPI_CalcCRC16(const void* recordPtr, size_t length,
                                   uint16_t poly, uint16_t initValue);
```

### 4.3 Logger API

DLL 内部から記録されたログを呼び出し側で取得するための API。リングバッファに最大 256 エントリを保持する。

```cpp
UCAPI_API size_t  UCAPI_GetLogCount();
UCAPI_API int     UCAPI_GetLog(size_t index,
                               uint8_t* outLevel, uint64_t* outTimestamp,
                               char* outFunction, size_t functionBufSize,
                               char* outMessage,  size_t messageBufSize);
UCAPI_API int     UCAPI_GetLogMessage(size_t index, char* outBuffer, size_t bufferSize);
UCAPI_API void    UCAPI_ClearLogs();
UCAPI_API void    UCAPI_SetLogLevel(uint8_t level);
UCAPI_API uint8_t UCAPI_GetLogLevel();
```

ログレベル: `0=None, 1=Error, 2=Warning, 3=Info, 4=Debug`

## 5. エラー条件・戻り値仕様

各 DLL 関数の戻り値および異常系動作。例外は内部で捕捉され、Logger に記録された上で戻り値（負値 / nullptr）として呼び出し側へ伝播する。

### 5.1 推奨 API

| 関数 | 正常 | 異常時の戻り値 | 主な失敗条件 |
|---|---|---|---|
| `UCAPI_SerializeRecord` | `0` | `-1` | `record / outBuffer / outSize == nullptr`、シリアライザ生成失敗、msgpack 例外 |
| `UCAPI_DeserializeRecord` | `0` | `-1` | `buffer == nullptr`、`bufferSize == 0`、`outRecord == nullptr`、シリアライザ生成失敗、デシリアライズ失敗、msgpack 例外 |
| `UCAPI_FreeBuffer` | — | — | `buffer == nullptr` は無害（`delete[] nullptr` 相当） |

### 5.2 オブジェクト API

| 関数 | 正常 | 異常時の戻り値 | 主な失敗条件 |
|---|---|---|---|
| `UCAPI_Create` | 有効ポインタ | （例外時）プロセス終了 | `new` 失敗 |
| `UCAPI_Destroy` | — | — | `obj == nullptr` は無害 |
| `UCAPI_AddRecord` | `0` | `-1` | `obj == nullptr`、内部例外 |
| `UCAPI_Serialize` | `0` | `-1` | `obj / outBuffer / outSize == nullptr`、シリアライザ生成・実行失敗 |
| `UCAPI_Deserialize` | 有効ポインタ | `nullptr` | `buffer == nullptr`、`payloadCount == 0`、デシリアライズ失敗、内部例外（RAII で自動解放） |
| `UCAPI_CalcCRC16` | CRC 値 | 初期値 | `recordPtr == nullptr` のとき初期値を返す |

### 5.3 Logger API

| 関数 | 正常 | 異常時の戻り値 | 主な失敗条件 |
|---|---|---|---|
| `UCAPI_GetLogCount` | エントリ数 (0–256) | `0` | 例外時のフォールバック |
| `UCAPI_GetLog` | `0` | `-1` / `-2` | `-1`: index 範囲外、`-2`: 出力バッファ不足 |
| `UCAPI_GetLogMessage` | `0` | `-1` / `-2` | `-1`: outBuffer == nullptr / index 範囲外、`-2`: バッファ不足 |
| `UCAPI_ClearLogs` | — | — | 例外時は無視 |
| `UCAPI_SetLogLevel` | — | — | 範囲外レベルは無視 |
| `UCAPI_GetLogLevel` | レベル値 | `1` (Error) | 例外時のフォールバック |

> **注**: すべての関数は内部で `try/catch` により例外を捕捉する設計のため、DLL 境界を越えて例外が伝播することはない。エラーの詳細は Logger API で取得できる。

## 6. 定数定義（`ucapi.h`）

| 定数 | 値 | 用途 |
|---|---|---|
| `UCAPI_MAGIC` | `0x55AA` | バイナリヘッダのマジックバイト |
| `UCAPI_HEADER_SIZE` | `10` | 独自バイナリヘッダのサイズ（バイト） |
| `UCAPI_MAX_PAYLOAD_SIZE` | `0x10000` (64 KB) | 最大ペイロードサイズ（安全上限） |
| `UCAPI_MIN_RECORD_SIZE` | `107` | レコード最小サイズ（最終フィールド offset 103 + float 4） |
| `UCAPI_CRC16_POLYNOMIAL` | `0x1021` | CRC16-CCITT 多項式 |
| `UCAPI_CRC16_INIT_VALUE` | `0xFFFF` | CRC16 初期値 |
| `UCAPI_CRC16_MSB_MASK` | `0x8000` | bit-by-bit 計算用 MSB マスク |
| `UCAPI_BITS_PER_BYTE` | `8` | バイトあたりビット数 |

なお、独自バイナリヘッダ内の `version` フィールドは現在 **`1`** をハードセットしている（将来のフォーマット変更時に増分予定）。

## 7. 実装済モジュール一覧

| モジュール | ファイル | 状態 |
|---|---|---|
| Core API | `ucapi.h`, `ucapi.cpp` | ✅ |
| DLL Export Layer | `ucapi_dll.h`, `ucapi_dll.cpp` | ✅ |
| Serializer Interface | `include/ucapi_message_serializer.h` | ✅ |
| Serializer Factory | `include/ucapi_message_serializer_factory.h`, `ucapi_message_serializer_factory_impl.*` | ✅ |
| MessagePack Serializer | `ucapi_msgpack_serializer.*`, `ucapi_msgpack_types.h` | ✅ |
| 独自バイナリ Serializer | `ucapi_serializer_utility.h`, `ucapi_serializer_utlity.cpp` | ✅ |
| Timecode | `ucapi_timecode.h`, `ucapi_timecode.cpp` | ✅ |
| Config (Singleton) | `ucapi_config.h`, `ucapi_config.cpp` | ✅ |
| Logger (Singleton) | `ucapi_logger.h`, `ucapi_logger.cpp` | ✅ |
| ユニットテスト | `UCAPI_DLL_Test/test.cpp` (GoogleTest, 36+ ケース) | ✅ |
| FlatBuffers Serializer | — | ⏸ 保留 (P2-8) |

## 8. ビルドとテスト

### ビルド

```bash
cd UCAPIProject/UCAPI

# Debug x64
msbuild UCAPI.sln /p:Configuration=Debug /p:Platform=x64

# Release x64
msbuild UCAPI.sln /p:Configuration=Release /p:Platform=x64

# AddressSanitizer x64（メモリ安全性検証用）
msbuild UCAPI.sln /p:Configuration=ASan /p:Platform=x64
```

### テスト

- GoogleTest ベースのユニットテストプロジェクト `UCAPI_DLL_Test` を Visual Studio Test Explorer から実行、またはビルド後の実行ファイルを直接実行
- カバー範囲: シリアライズ往復、CRC16 標準テストベクタ、バッファ境界、悪意入力、並行性（concurrent create/destroy/serialize/CRC16）、msgpack 異常フォーマット
- AddressSanitizer 構成によるメモリリーク検出を CI で実行

### CI/CD（GitHub Actions）

| ワークフロー | トリガ | 内容 |
|---|---|---|
| `gtest-ci.yml` | main への push / PR | Release x64 ビルド + GoogleTest 実行 + テストレポート |
| `build.yml` | バージョンタグ push (`v*`) | Release + ASan ビルド + テスト + アーティファクト保存（90日） |

## 9. ドキュメント参照

- データフォーマット仕様: `docs/UCAPI_Data_Format_Specification.yaml`
- プロジェクト構造・パターン: `memory-bank/systemPatterns.md`
- 進捗・既知課題: `memory-bank/progress.md`, `memory-bank/technical-debt-tasks.md`
- Unity プラグイン: `Unity/UCAPI_Develop/Packages/UCAPI4Unity/`

---

*以上*

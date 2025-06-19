# UCAPI_DLL プロジェクト仕様書

最終更新日: 2025年6月19日

---

## 1. プロジェクト概要

- プロジェクト名：UniversalCamerawork API (UCAPI_DLL)
- 目的：複数プラットフォーム（Unity/Unreal/Blender 等）間でカメラ状態情報（位置、回転、ズームレベル）を共通バイナリ形式でやり取りする C++ DLL を提供する。
- 言語／環境：C++17 / MSVC (Visual Studio 2022)
- 出力形式：独自バイナリ／CRC16 整合性チェック付き、MessagePack 併用オプション

## 2. コア機能

1. カメラ状態記録型（位置 Vector3、回転 Quaternion、ズーム float）
2. 独自バイナリシリアライズ／デシリアライズ
3. CRC16 チェックサムによるデータ整合性検証
4. MessagePack 形式へのマッピング（プラグイン式切替）
5. DLL エクスポート API：初期化、シリアライズ、デシリアライズ、エラーログ取得
6. エラーハンドリング：HRESULT 返却および内部ロギング（Singleton ロガー）

## 3. システム構成とモジュール

```
UCAPI_DLL
├─ CoreAPI           // ucapi.h / ucapi.cpp
├─ Serializer        // ucapi_serializer_utility.h / .cpp
├─ Converter         // ucapi_msgpack_types.h / ucapi_msgpack_converter.*
├─ Timecode          // ucapi_timecode.h / .cpp
├─ Factory           // Serializer/Converter 切替実装
├─ Logger            // Singleton ロガー
└─ Transport(Later)  // Strategy パターンによる切替予定
```

## 4. インターフェース仕様
```cpp
// C API: エクスポート関数一覧
UCAPI_API ucapi_t* UCAPI_Create();
UCAPI_API void UCAPI_Destroy(ucapi_t* obj);

UCAPI_API int UCAPI_Serialize(ucapi_t* obj, uint8_t** outBuffer, size_t* outSize);
UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer);

UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount);

UCAPI_API uint16_t UCAPI_CalcCRC16(const void* recordPtr, size_t length, uint16_t poly, uint16_t initValue);
```

## 5. 現在実装済み項目

| モジュール                             | 実装状況                           |
|---------------------------------------|-----------------------------------|
| ucapi.h / ucapi.cpp                  | DLLエントリポイント、API 定義     |
| ucapi_serializer_utility.h/.cpp      | バイナリ形式シリアライズ／CRC16    |
| ucapi_timecode.h/.cpp                | タイムコード型生成／パース        |
| ucapi_msgpack_types.h                | MessagePack 用型定義              |
| ucapi_msgpack_converter.*            | MessagePack シリアライズ対応      |
| プロジェクト設定                     | MSVC プロジェクト構成            |
| GoogleTest ベースのユニットテスト    | UCAPI_DLL_Test プロジェクト       |

## 6. ビルドとテスト

- ビルド手順: `msbuild UCAPI_DLL.sln /p:Configuration=Debug /p:Platform=x64`
- 単体テスト: GoogleTest (UCAPI_DLL_Test)
- 自動テスト: `run_ucapi_testtool.bat` (C# NUnit ツール)

## 7. ドキュメント参照

- データフォーマット仕様: `docs/UCAPI_Data_Format_Specification.yaml`
- プロジェクト構造: `memory-bank/systemPatterns.md`

---

*以上*

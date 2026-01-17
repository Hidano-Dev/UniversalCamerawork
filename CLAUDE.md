# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## プロジェクト概要

UniversalCamerawork (UCAPI) は、Unity/Unreal/Blender等の複数プラットフォーム間でカメラ状態情報（位置、回転、ズーム等）を共通バイナリ形式で共有するためのC++ DLLです。

## ビルドコマンド

```bash
# ソリューションディレクトリに移動
cd UCAPIProject/UCAPI

# Debug x64 ビルド
msbuild UCAPI_DLL.sln /p:Configuration=Debug /p:Platform=x64

# Release x64 ビルド
msbuild UCAPI_DLL.sln /p:Configuration=Release /p:Platform=x64

# 単体テストプロジェクトのみビルド
msbuild UCAPI_DLL_Test.vcxproj /p:Configuration=Debug /p:Platform=x64
```

## テスト実行

GoogleTestベースのユニットテストは `UCAPI_DLL_Test` プロジェクトに含まれています。Visual Studio Test Explorerから実行するか、ビルド後の実行ファイルを直接実行してください。

## アーキテクチャ

```
Core API層 (ucapi.h/cpp)
    ↓
シリアライザ層 (ucapi_serializer_utility.h/cpp)
    ↓
コンバータ層 (ucapi_msgpack_converter.h/cpp)
```

### 主要モジュール

| モジュール | 役割 |
|-----------|------|
| ucapi.h/cpp | カメラデータ構造体 (`ucapi_t`, `record_t`)、CRC16計算 |
| ucapi_dll.h/cpp | DLLエクスポート関数（Create/Destroy/Serialize/Deserialize） |
| ucapi_serializer_utility.h/cpp | バイナリシリアライズ/デシリアライズ、CRC16組み込み |
| ucapi_timecode.h/cpp | フレームレート対応タイムコード処理 |
| ucapi_msgpack_converter.h/cpp | MessagePack形式変換 |
| ucapi_message_serializer_factory_impl.h/cpp | シリアライザ生成・切替 |

### デザインパターン

- **Factory**: シリアライザ/コンバータの生成と切替
- **Strategy**: 異なるシリアライズ形式の切替（独自バイナリ/MessagePack/FlatBuffers予定）
- **Singleton**: グローバルロガー、設定管理

### DLLエクスポートAPI

```cpp
UCAPI_API ucapi_t* UCAPI_Create();
UCAPI_API void UCAPI_Destroy(ucapi_t* obj);
UCAPI_API int UCAPI_Serialize(ucapi_t* obj, uint8_t** outBuffer, size_t* outSize);
UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer);
UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount);
UCAPI_API uint16_t UCAPI_CalcCRC16(const void* recordPtr, size_t length, uint16_t poly, uint16_t initValue);
```

## 技術スタック

- C++17 / MSVC v143 / Visual Studio 2022
- GoogleTest（C++テスト）
- プラットフォーム: Windows 10/11 (x86/x64)

## コーディング規約

- ファイル/クラス名は `ucapi_` プレフィックス
- STL使用は最小限（DLLサイズ軽量化のため）
- 依存ライブラリはシングルヘッダまたはソース統合
- プラットフォーム依存の変換処理はDLL外（各プラットフォーム側プラグイン）で実装

## 重要なドキュメント

- `docs/UCAPI_Data_Format_Specification.yaml` - バイナリフォーマット仕様（Header 10bytes + Record 128bytes）
- `docs/UCAPI_DLL_Specification.md` - DLL仕様書

## Memory Bank（知見管理システム）

`memory-bank/` ディレクトリには、プロジェクトの知見・タスク管理・設計決定が集約されています。

### 作業開始時に確認すべきファイル

| ファイル | 内容 |
|----------|------|
| `activeContext.md` | 現在の作業フォーカス、最新の変更履歴 |
| `progress.md` | 進捗状況、完了タスク、既知の課題 |
| `technical-debt-tasks.md` | 技術的負債・バグ修正タスクリスト（P0〜P2優先度） |

### 技術調査時に参照すべきファイル

| ファイル | 内容 |
|----------|------|
| `systemPatterns.md` | アーキテクチャ、デザインパターン |
| `project-knowledge.md` | 設計決定、実装パターンとアンチパターン |
| `common-patterns.md` | よく使用するコマンドやテンプレート |

### 作業完了時の更新ルール

- タスク完了時は `progress.md` と `technical-debt-tasks.md` を更新
- 重要な設計決定や教訓は `project-improvements.md` に記録
- 議論内容や決定事項は `chatContext.md` に追記

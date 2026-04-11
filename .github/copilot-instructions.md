# Copilot Instructions

このファイルは、GitHub Copilot Agentが本プロジェクトで効果的に作業するための学習と参照用ドキュメントです。

## プロジェクト概要
- プロジェクト名：UniversalCamerawork (UCAPI)
- 目的：複数プラットフォーム（Unity、Unreal、Blenderなど）間でカメラ状態情報（位置、回転、光学パラメータ、タイムコード）を共通フォーマットでシリアライズ／デシリアライズし共有するC++ DLLを開発する。
- コア機能：
  - MessagePack シリアライズ／デシリアライズ（推奨経路）
  - 独自バイナリ形式でのシリアライズ + CRC16 整合性チェック（Legacy 経路）
  - シリアライザのプラグイン式切替（Factory + Strategy）
  - Singleton ロガー（リングバッファ、5段階レベル、スレッドセーフ）

## プロジェクト構造・パターン
- `UCAPIProject/UCAPI/UCAPI_DLL/`：C++ DLL本体（コアAPI、シリアライザ、ロガー、設定）
- `UCAPIProject/UCAPI/UCAPI_DLL_Test/`：GoogleTestベースのユニットテスト（36+ ケース）
- `Unity/UCAPI_Develop/Packages/UCAPI4Unity/`：Unityプラグイン（C# P/Invokeラッパー、Cinemachine統合）
- `docs/`：仕様書（UCAPI_DLL_Specification.md、UCAPI_Data_Format_Specification.yaml）
- `memory-bank/`：知見・タスク・進捗管理ドキュメント群
- モジュール層：Core API → Serializer (Strategy + Factory) → DLL Export Layer
- デザインパターン：
  - Singleton（Logger、Config）
  - Factory（IMessageSerializerFactory）
  - Strategy（IMessageSerializer の複数実装切替）
- 命名規則：
  - ファイル/クラス名は `ucapi_` プレフィックス
  - ヘッダファイル拡張子 `.h`、実装 `.cpp`
  - DLL エクスポート関数は `UCAPI_` プレフィックス

## 開発・テスト
- C++ コンパイル：Visual Studio 2022, MSVC v143, C++17
- C++ テスト：GoogleTest（vcpkg manifest モードで管理、v1.15+）
- 依存ライブラリ：msgpack-c（vcpkg manifest）
- メモリ安全性検証：MSVC AddressSanitizer (`Configuration=ASan`)
- CI/CD：GitHub Actions
  - `gtest-ci.yml`：main への push / PR で Release x64 ビルド + テスト
  - `build.yml`：バージョンタグ push で Release + ASan ビルド + アーティファクト保存

## ドキュメント・仕様
- `docs/UCAPI_DLL_Specification.md`：DLL 仕様書（API、エラー条件、定数定義）
- `docs/UCAPI_Data_Format_Specification.yaml`：バイナリフォーマット詳細
- `README.md`：プロジェクト概要と CI ワークフロー説明

## コーディングガイドライン
- 依存最小化：STL 使用を必要最小限に抑え、シングルヘッダライブラリを優先
- エラーハンドリング：DLL 境界では `int 0/-1` または `nullptr` を返却。例外は内部で捕捉し Logger に記録、呼び出し側は Logger API でエラー詳細を取得
- パフォーマンス：メモリアロケーション抑制、データコピー最小化
- セキュリティ：将来の暗号化／アクセス制御機構設計を考慮
- スレッド安全性：Singleton（Config / Logger）は `std::call_once` + `std::atomic` で初期化、ログ操作は `std::mutex` で排他制御

## 学習すべきポイント
- プラットフォーム依存の変換処理は DLL コアではなく、各プラットフォーム側のプラグインで実装する
- ローカル PC 内通信とネットワーク通信は **DLL 範囲外**（Transport 層）。各プラットフォーム側で実装
- IMessageSerializer インターフェース + Factory による拡張機構は実装済。FlatBuffers / Cap'n Proto 対応は将来課題（P2-8 保留）
- バイナリフォーマットは固定長レコード方式。ペイロード長は `numPayload × sizeof(Record)` で暗黙計算

## 継続的学習ポイント
- プラグイン式シリアライザ登録機構の拡張方法
- プラットフォーム別の DLL ロード／呼び出しサンプル
- シリアライザフォーマットの後方互換性管理（version フィールド運用）

## 重要な履歴
1. メモリバンク構造の定義と初期ドキュメント作成 (2025-06-19)
2. P0（セキュリティ・安定性）全 6 項目完了 (2026-01-17): メモリリーク修正、バッファオーバーフロー対策、Config スレッドセーフ化、RAII 化
3. P1（品質）全 6 項目完了 (2026-01-19): エッジケーステスト追加、AddressSanitizer 統合、GoogleTest v1.15 移行
4. P2（保守）主要項目完了 (2026-01-17〜19): vcpkg manifest 化、Singleton ロガー実装、CI/CD 構築
5. P2-6 ドキュメント同期完了 (2026-04-12): UCAPI_DLL_Specification.md / Data_Format_Specification.yaml / copilot-instructions.md / README.md を実装に追従

## 知見管理システム
以下のファイルで知見を体系的に管理しています (`memory-bank/` 配下):
- `memory-bank/activeContext.md`: 現在の作業フォーカスと最新変更履歴
- `memory-bank/chatContext.md`: AIアシスタントやチームとのチャットログ・議論内容
- `memory-bank/productContext.md`: プロジェクト背景、狙い、解決する問題
- `memory-bank/projectbrief.md`: プロジェクト概要、ゴール、制約条件
- `memory-bank/systemPatterns.md`: アーキテクチャ、デザインパターン、コンポーネント関係
- `memory-bank/techContext.md`: 使用技術、開発環境、依存関係や技術制約
- `memory-bank/technical-debt-tasks.md`: P0〜P2 優先度別の技術的負債タスクリスト
- `memory-bank/progress.md`: 完了/未完了タスク、既知課題、現在ステータス
- `memory-bank/project-knowledge.md`: 技術的知見、設計決定、実装パターンとアンチパターン
- `memory-bank/project-improvements.md`: 改善履歴、試行錯誤の記録と教訓
- `memory-bank/common-patterns.md`: よく使用するコマンドやテンプレート

*更新時は必ず上記ファイルに知見を追記し、後続作業で参照すること。*

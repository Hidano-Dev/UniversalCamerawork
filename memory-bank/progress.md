# 進捗 (progress.md)

## 完了
- Memory Bank基本ファイル作成
- `tasks.md`, `chatContext.md` の追加
- **P0タスク全完了** (2026-01-17)
  - P0-1: メモリリーク修正 - payloadArray
  - P0-2: メモリリーク修正 - reserved配列
  - P0-3: バッファオーバーフロー修正 - record_t::_read()
  - P0-4: バッファオーバーフロー修正 - ucapi_t::_read()
  - P0-5: スレッド安全性修正 - Config初期化
  - P0-6: 例外時リソースリーク修正 - UCAPI_Deserialize
- **P1-1完了**: _clean_up()関数の実装 (2026-01-17)
- **P1-2完了**: CRC16テストに参照値を追加 (2026-01-17)
- **P1-3完了**: エッジケーステスト・異常系テストの追加 (2026-01-17)
- **P1-4完了**: AddressSanitizer/メモリリークテストの導入 (2026-01-17)
  - ASan|x64ビルド構成を追加
  - GitHub ActionsにASanテストジョブを追加
- **P1-6完了**: 未使用コードの削除 - convert_to_ucapi/convert_to_msgpack (2026-01-17)
- **P2-1完了**: vcpkg依存関係の明示化 (2026-01-17)
- **P2-2完了**: マジックナンバーの定数化 (2026-01-17)
- **P2-4完了**: timecode_tとucapi_tの統合 (2026-01-17)
  - アクセサメソッド `get_timecode()` / `set_timecode()` を追加
  - `m_subframe` の型を `uint32_t` から `float` に修正（仕様書準拠）
- **P2-7完了**: CI/CD環境構築 (2026-01-17)
- **P1-5完了**: GoogleTestバージョン更新 (2026-01-19)
  - NuGetパッケージからvcpkgに移行（v1.8.1→v1.15+）

## 進行中
- なし（P1タスク全完了）

## 未完了
- P2タスク（保守性・開発体験向上）
  - P2-3: Singletonロガーの実装
  - P2-4: timecode_tとucapi_tの統合
  - P2-5: コメント言語の統一
  - P2-6: 仕様書と実装の同期・ドキュメント整備
  - P2-8: FlatBuffers対応シリアライザ実装（保留）
  - P2-9: Mayaプラグイン骨子プロジェクト作成（保留）
- Unityプラグイン改善（ucapi4unity-todo.md参照）

## 現在のステータス
- DLLコアのセキュリティ・安定性に関わるP0タスクは全て完了
- **P1タスク全完了**（品質・保守性向上）
- CI/CDでAddressSanitizerによるメモリ安全性検証が自動化
- GoogleTestをvcpkgに統一（最新版v1.15+）
- Memory Bankドキュメントの整備継続中

## 既知の課題
- P2タスクの保守性・開発体験向上
- Unityプラグイン（UCAPI4Unity）のUPMパッケージ化対応

## 更新履歴
- 2026-01-19: P1-5完了（GoogleTestバージョン更新、NuGet→vcpkg移行）
- 2026-01-17: P1-4完了（AddressSanitizer/メモリリークテストの導入）
- 2026-01-17: P2-4完了（timecode_tとucapi_tの統合: アクセサ追加、m_subframeをfloatに修正）
- 2026-01-17: P1-6完了（未使用コードの削除）
- 2026-01-17: P2-2完了（マジックナンバーの定数化）
- 2026-01-17: P1-3完了、P2-7完了、P2-1完了を反映
- 2026-01-17: P0タスク全完了、P1-1完了、P1-2完了を反映
- 2025-06-19: 初版作成

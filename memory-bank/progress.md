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
- **P1タスク一部完了** (2026-01-17)
  - P1-1: _clean_up()関数の実装
  - P1-2: CRC16テストに参照値を追加
  - P1-3: エッジケーステスト・異常系テストの追加
  - P1-6: 未使用コードの削除 - convert_to_ucapi
- **P2タスク一部完了** (2026-01-17)
  - P2-1: vcpkg依存関係の明示化
  - P2-2: マジックナンバーの定数化
  - P2-4: timecode_tとucapi_tの統合（アクセサ追加、m_subframeをfloatに修正）
  - P2-5: コメント言語の統一（英語に統一）
  - P2-7: CI/CD環境構築（GitHub Actions）

## 進行中
- P1タスク（品質・保守性向上）
  - P1-4: AddressSanitizer/メモリリークテストの導入
  - P1-5: GoogleTestバージョン更新

## 未完了
- P2タスク（保守性・開発体験向上）
  - P2-3: Singletonロガーの実装
  - P2-6: 仕様書と実装の同期・ドキュメント整備
  - P2-8: FlatBuffers対応シリアライザ実装（保留）
  - P2-9: Mayaプラグイン骨子プロジェクト作成（保留）
- Unityプラグイン改善（ucapi4unity-todo.md参照）

## 現在のステータス
- DLLコアのセキュリティ・安定性に関わるP0タスクは全て完了
- P1タスク（P1-1～P1-3, P1-6）完了、残りP1-4～P1-5が進行中
- P2タスク（P2-1, P2-2, P2-4, P2-5, P2-7）完了
- CI/CD環境構築完了（GitHub Actions）
- Memory Bankドキュメントの整備継続中

## 既知の課題
- P1-4～P1-5のテスト・品質改善タスク
- P2-3, P2-6の保守性向上タスク
- Unityプラグイン（UCAPI4Unity）のUPMパッケージ化対応

## 更新履歴
- 2026-01-17: mainブランチをマージしてprogress.mdを最新状態に同期
- 2026-01-17: P2-4完了（timecode_tとucapi_tの統合: アクセサ追加、m_subframeをfloatに修正）
- 2026-01-17: P1-6完了（未使用コードの削除）
- 2026-01-17: P0タスク全完了、P1-1完了を反映
- 2025-06-19: 初版作成

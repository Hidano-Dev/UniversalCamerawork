# 改善履歴・教訓 (memory-bank/project-improvements.md)

## 2026-01-17: P0セキュリティ・安定性修正

**内容**: コードレビューで発見された緊急度の高い技術的問題を全て修正

**修正項目**:
- P0-1: メモリリーク修正（未使用の`payloadArray`変数削除）
- P0-2: メモリリーク修正（未使用の`reserved`ローカル変数削除）
- P0-3: バッファオーバーフロー修正（`record_t::_read()`に境界チェック追加）
- P0-4: バッファオーバーフロー修正（`ucapi_t::_read()`にバッファサイズ検証追加）
- P0-5: スレッド安全性修正（`std::call_once` + `std::atomic<bool>`でConfig初期化をスレッドセーフ化）
- P0-6: 例外時リソースリーク修正（`UCAPI_Deserialize`を`std::unique_ptr`でRAII化）

**教訓**:
- DLLの外部公開APIは特に境界チェックを厳密に行う必要がある
- RAIIパターンを積極的に活用してリソース管理を安全に
- マルチスレッド環境を想定した初期化処理の設計が重要

---

## 2026-01-17: Memory Bankドキュメント整備

**内容**: CLAUDE.mdとmemory-bank内ファイルの整合性を確認・修正

**修正項目**:
- `common-patterns.md`: ビルドコマンドをCLAUDE.mdに合わせて修正
- `progress.md`: P0タスク完了状況を反映
- `activeContext.md`: 現在の作業フォーカスを更新
- `project-improvements.md`: 改善履歴を追加

**教訓**:
- 定期的にドキュメント間の整合性を確認する習慣が重要
- CLAUDE.mdを正式なプロジェクトガイドとして位置づけ

---

## 2025-06-19: 知見管理システム更新

**内容**: `.claude/` から `memory-bank/` に移行し、ファイル構造を整理

**教訓**:
- ファイル構造はシンプルに保つ
- プロジェクト依存知見は`memory-bank/`に集約

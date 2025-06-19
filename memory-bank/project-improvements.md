# 改善履歴・教訓 (memory-bank/project-improvements.md)

## 2025-06-19: 知見管理システム更新
**内容**: `.claude/` から `memory-bank/` に移行し、ファイル構造を整理

## 2025-06-18: FlatBuffers対応試作
- ❌ メモリコピー多発によるパフォーマンス低下
- ✅ IMessageSerializer経由の遅延ローディング実装

## 2025-06-17: LMAX Disruptor検証
- ❌ オーバーヘッド高
- ✅ 独自リングバッファ実装に変更

## 教訓
- ファイル構造はシンプルに保つ
- プロジェクト依存知見は`memory-bank/`に集約

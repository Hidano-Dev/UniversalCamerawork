# タスクリスト (tasks.md)

> **注記**: このファイルは2026-01-17に整理されました。
> 有効なタスクは `technical-debt-tasks.md` に統合済みです。

---

## 統合・整理結果

### 完了済み（既に実装済みのため削除）

| タスク | 状態 | 備考 |
|--------|------|------|
| IMessageSerializerインターフェース定義 | 完了 | `ucapi_message_serializer.h` に実装済み |
| GoogleTest環境構築（P0-A） | 完了 | `packages.config` に設定済み |
| NUnit環境構築（P0-B） | 完了 | Unity Test Framework経由で構築済み |
| プラグイン式コンバータ登録機構実装 | 完了 | `IMessageSerializerFactory::RegisterSerializer` で実装済み |
| Unityプラグイン骨子プロジェクト作成 | 完了 | `Unity/UCAPI_Develop/UCAPI4Unity` に存在 |

### DLL範囲外として削除

| タスク | 理由 |
|--------|------|
| Transportレイヤ抽象化インターフェース定義 | DLLの責務はシリアライズ/デシリアライズ。Transportは各プラットフォームプラグイン側で実装すべき |
| ロックフリーリングバッファ実装 | プラットフォーム側の責務。DLLはデータ変換のみに集中 |
| UDPベースネットワークチャネル実装 | 同上 |
| LMAX Disruptorラッパー実装 | 過剰設計。パフォーマンス問題発生時に検討 |
| クロック同期（PTP/NTP）スタブ実装 | 現時点では過剰。必要性が明確になってから検討 |

### technical-debt-tasks.md に統合

| 元タスク | 統合先 |
|----------|--------|
| 各APIのエラー条件を仕様書に明文化 | P2-6 |
| 準異常系テストシナリオ定義 | P1-3 |
| 不足テストケース実装 | P1-3 |
| CI定義＆README更新 | P2-7 (新規) |
| ドキュメント整備 | P2-6 |
| FlatBuffers対応シリアライザ実装 | P2-8 (新規・保留) |
| Mayaプラグイン骨子プロジェクト作成 | P2-9 (新規・保留) |

---

## 更新履歴

- 2026-01-17: タスク整理・統合完了。有効なタスクは`technical-debt-tasks.md`へ移行

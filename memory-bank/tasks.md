# タスクリスト (tasks.md)

以下のタスクを優先度順に実施します。

## P0: 直近必須
- IMessageSerializerインターフェース定義
- FlatBuffers対応シリアライザ実装
- Transportレイヤ抽象化インターフェース定義
- ロックフリーリングバッファ実装
- GoogleTest環境構築（P0-A）
- NUnit環境構築（P0-B）

## P1: 並行して進めたい
- UDPベースネットワークチャネル実装
- LMAX Disruptorラッパー実装
- プラグイン式コンバータ登録機構実装
- Unity/Mayaプラグイン骨子プロジェクト作成

## P2: 余裕があれば
- クロック同期（PTP/NTP）スタブ実装
- CI定義＆README更新
- ドキュメント整備

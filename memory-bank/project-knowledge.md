# プロジェクト知見集 (memory-bank/project-knowledge.md)

## アーキテクチャ決定
- Core API 層: カメラ情報取得・公開インターフェース
- シリアライザ層: 独自バイナリ＋CRC16組み込み
- コンバータ層: バイナリ↔プラットフォーム型変換（プラグイン式）

## デザインパターン
- シングルトン: ロガー、設定管理
- ファクトリ: Serializer/Converter生成
- ストラテジー: Serialize形式切替
- オブザーバ: 更新通知

## 実装パターン
- IMessageSerializerインターフェースによるFlatBuffers/Cap’n Proto切替
- プラグイン式コンバータ登録機構をFactoryで管理
- Transportレイヤ（リングバッファ/Disruptor）切替

## 避けるべきアンチパターン
- DLLコアにプラットフォーム依存処理を実装
- STL過度使用によるバイナリ肥大化
- エラーハンドリング不足（HRESULT/例外併用を推奨）

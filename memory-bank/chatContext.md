# チャットコンテキスト (chatContext.md)

## 主な議論内容

1. リアルタイム・ストリーミングのデザインパターン
   - Observerパターンの課題とロックフリーリングバッファ、LMAX Disruptorの提案
   - Producer–Consumerモデルによる低レイテンシ実現

2. シリアライズライブラリの選択肢
   - MessagePackの軽量性とFlatBuffers／Cap’n Protoのゼロコピーの比較
   - 将来の置き換えを容易にするためのIMessageSerializerインターフェースの導入

3. プラットフォーム依存コンバータの実装レイヤ
   - DLLコアに依存処理を含めず、各プラグイン側で動作する旨を明文化

4. タイミング同期の具体化
   - PTP/NTPベースでクロック同期し、送信タイムスタンプ付きバッファリング
   - 小バッファ内で順序調整し即時デシリアライズ

5. 単一ヘッダ実装の保守性とハイブリッドアプローチ
   - コアパフォーマンス部分はヘッダ、上位APIはバイナリライブラリで提供

## 今後の要件
- IMessageSerializer／ITransportChannelインターフェースの定義
- テスト環境（GoogleTest、NUnit）のP0タスクへの追加

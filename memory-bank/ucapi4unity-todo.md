# UCAPI4Unity TODOリスト

Unity側スクリプト群の構成レビューで発見された改善項目一覧です。

## ステータス凡例
- [ ] 未着手
- [x] 完了
- [~] 進行中

---

## 🔴 高優先度 (P0)

### UNITY-001: UPMパッケージ化対応
- **ステータス**: [~] 進行中（部分完了）
- **概要**: `package.json` がなくUnityPackageManagerで配布できない構成
- **対応内容**:
  - [ ] `Packages/com.universalcamerawork.ucapi4unity/` へファイル移動
  - [x] `package.json` 作成（name, version, displayName, dependencies等）
  - [ ] サンプルを `Samples~/` フォルダへ移動（UPM標準形式）
  - [x] `README.md` 追加
- **影響範囲**: ディレクトリ構成全体
- **備考**: 2026-01-17 ファイル移動せずAssets以下にpackage.jsonとREADME.mdを作成

### UNITY-002: Assembly Definition参照方式の修正
- **ステータス**: [x] 完了
- **概要**: asmdefファイルでGUID参照を使用しており、ファイル移動時に破損リスクあり
- **対応内容**:
  - 全asmdefファイルのGUID参照をアセットパス参照に変更
- **対象ファイル**:
  - `Runtime/Core/UcApi.Core.asmdef` - 参照なし、変更不要
  - `Runtime/UnityCamera/UcApi4Unity.UnityCamera.asmdef` - [x] 修正済み
  - `Runtime/CinemachineCamera/UcApi4Unity.CinemachineCamera.asmdef` - [x] 修正済み
  - `Sample/Scripts/UcApi4Unity.Sample.asmdef` - [x] 修正済み
  - `Tests/UcApiTest.asmdef` - 既に名前参照使用
- **備考**: 2026-01-17 GUID参照を名前参照に変換完了

---

## 🟡 中優先度 (P1)

### UNITY-004: UcApiRecordParserのアクセス修飾子変更
- **ステータス**: [x] 完了
- **概要**: 内部実装詳細が `public` として露出している
- **対応内容**:
  - `UcApiRecordParser` クラスを `internal` に変更
  - ファサードクラス経由でのみ機能を公開
- **対象ファイル**:
  - `Runtime/UnityCamera/UcApiRecordParser.cs`
  - `Runtime/CinemachineCamera/UcApiRecordParser.cs`
- **備考**: 2026-01-17 両ファイルのアクセス修飾子を `public` から `internal` に変更完了

### UNITY-005: エラーハンドリングの強化
- **ステータス**: [ ] 未着手
- **概要**: CRC16ミスマッチ時はログ出力のみで、呼び出し元がエラーを検知できない
- **対応内容**:
  - CRC16ミスマッチ時に例外スローまたはResult型導入
  - DLLロード失敗時のエラーハンドリング追加
  - 引数バリデーション追加
- **対象ファイル**: `Runtime/Core/UcApiCore.cs`

### UNITY-006: テストカバレッジ拡充
- **ステータス**: [ ] 未着手
- **概要**: Core層のみテスト済み。プラットフォーム層のテストがない
- **対応内容**:
  - `UcApi4UnityCamera` のユニットテスト追加
  - `UcApi4CinemachineCamera` のユニットテスト追加
  - `UcApiRecordParser` の変換テスト追加
- **対象フォルダ**: `Tests/`

### UNITY-007: XMLドキュメントコメント追加
- **ステータス**: [ ] 未着手
- **概要**: 公開APIにドキュメントコメントがなく、IntelliSenseで説明が表示されない
- **対応内容**:
  - 全 `public` クラス・メソッドに `<summary>` タグ追加
  - パラメータ説明 `<param>` タグ追加
  - 戻り値説明 `<returns>` タグ追加
- **対象ファイル**: Runtime以下の全公開クラス

---

## 🟢 低優先度 (P2)

### UNITY-008: Editor拡張機能の追加
- **ステータス**: [ ] 未着手
- **概要**: インスペクタ拡張やシーンビュー可視化ツールがない
- **対応内容**:
  - `Editor/` フォルダ作成
  - UcApiRecord のカスタムインスペクタ実装
  - シーンビューでのカメラデータ可視化ギズモ
  - TimeCode/FrameRate設定用EditorWindow
- **新規作成フォルダ**: `Editor/`

### UNITY-009: 非同期API対応
- **ステータス**: [ ] 未着手
- **概要**: 全て同期処理で、高負荷時にフレーム落ちの可能性
- **対応内容**:
  - `async/await` 対応のシリアライズ/デシリアライズAPI追加
  - UniTask対応の検討
- **対象ファイル**:
  - `Runtime/Core/UcApiCore.cs`
  - `Runtime/UnityCamera/UcApi4UnityCamera.cs`
  - `Runtime/CinemachineCamera/UcApi4CinemachineCamera.cs`

### UNITY-010: サンプルコードの改善
- **ステータス**: [ ] 未着手
- **概要**: サンプルコードが最低限の実装のみ
- **対応内容**:
  - コメント追加による使用方法説明
  - エラーハンドリングのベストプラクティス例示
  - 複数カメラ同時送受信のサンプル追加

---

## 更新履歴

| 日付 | 更新内容 |
|------|----------|
| 2026-01-17 | 初版作成（構成レビュー結果より10項目抽出） |
| 2026-01-17 | UNITY-003削除（設計思想に反するため：タイムコードは送信側が厳密に定義すべき） |
| 2026-01-17 | UNITY-004完了（UcApiRecordParserを`internal`に変更） |

# UCAPI4Unity

**Universal Camerawork API (UCAPI)** のUnityプラグインです。

## 概要

UCAPI4Unityは、共通のバイナリフォーマットを使用して、複数プラットフォーム（Unity、Unreal Engine、Blender）間でカメラ状態データ（位置、回転、ズーム、タイムコード等）を共有することを可能にします。

## 機能

- UCAPI C++ DLLを使用したカメラデータのシリアライズ/デシリアライズ
- 標準Unity Cameraのサポート
- Cinemachine仮想カメラのサポート
- 複数のフレームレート規格に対応したタイムコードサポート（24fps, 25fps, 30fps, 60fps等）

## 動作要件

- Unity 2021.3 以降
- （オプション）Cinemachineカメラサポートには Cinemachine パッケージが必要

## インストール

### Unity Package Manager経由（Git URL）

1. Unity Package Managerを開く（Window > Package Manager）
2. 「+」ボタンをクリックし、「Add package from git URL...」を選択
3. このリポジトリのGit URLを入力

### 手動インストール

1. `UCAPI4Unity`フォルダをプロジェクトの`Assets`または`Packages`フォルダにコピー

## ディレクトリ構成

```
UCAPI4Unity/
├── Plugins/           # ネイティブDLL (UCAPI_DLL.dll)
├── Runtime/
│   ├── Core/          # コアAPI (UcApiCore, UcApiRecord等)
│   ├── UnityCamera/   # 標準Unity Camera連携
│   └── CinemachineCamera/  # Cinemachine連携
├── Sample/            # サンプルシーン・スクリプト
└── Tests/             # ユニットテスト
```

## クイックスタート

### カメラデータのシリアライズ

```csharp
using UcApi.Core;

// カメラからレコードを作成
var record = new UcApiRecord();
record.Position = transform.position;
record.Rotation = transform.rotation;
record.FieldOfView = camera.fieldOfView;

// シリアライズ
byte[] data = UcApiCore.Serialize(record);
```

### カメラデータのデシリアライズ

```csharp
using UcApi.Core;

// 受信したデータをデシリアライズ
UcApiRecord record = UcApiCore.Deserialize(data);

// カメラに適用
transform.position = record.Position;
transform.rotation = record.Rotation;
camera.fieldOfView = record.FieldOfView;
```

## ライセンス

詳細はLICENSEファイルを参照してください。

## 関連ドキュメント

- [UCAPI DLL仕様書](docs/UCAPI_DLL_Specification.md)
- [UCAPIデータフォーマット仕様](docs/UCAPI_Data_Format_Specification.yaml)

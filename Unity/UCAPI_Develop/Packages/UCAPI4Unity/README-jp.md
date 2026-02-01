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

- Unity 6000.0 以降
- （オプション）Cinemachineカメラサポートには Cinemachine パッケージが必要

## インストール

### npmレジストリ経由（Scoped Registry）

`Packages/manifest.json` に以下を追加：

```json
{
  "scopedRegistries": [
    {
      "name": "hidano",
      "url": "https://registry.npmjs.com",
      "scopes": ["com.hidano"]
    }
  ],
  "dependencies": {
    "com.hidano.ucapi4unity": "0.1.0-preview.1"
  }
}
```

### Unity Package Manager経由（Git URL）

1. Unity Package Managerを開く（Window > Package Manager）
2. 「+」ボタンをクリックし、「Add package from git URL...」を選択
3. 以下のURLを入力

```
https://github.com/Hidano-Dev/UniversalCamerawork.git?path=Unity/UCAPI_Develop/Packages/UCAPI4Unity
```

## ディレクトリ構成

```
UCAPI4Unity/
├── Plugins/           # ネイティブDLL (UCAPI_DLL.dll)
├── Runtime/
│   ├── Core/          # コアAPI (UcApiCore, UcApiRecord等)
│   ├── UnityCamera/   # 標準Unity Camera連携
│   └── CinemachineCamera/  # Cinemachine連携
├── Samples~/          # サンプルシーン・スクリプト
└── Tests/             # ユニットテスト
```

## クイックスタート

### カメラデータのシリアライズ

```csharp
using UCAPI4Unity.Runtime.UnityCamera;

// カメラの状態をバイト配列にシリアライズ
byte[] data = UcApi4UnityCamera.SerializeFromCamera(camera);
```

### カメラデータのデシリアライズ

```csharp
using UCAPI4Unity.Runtime.UnityCamera;

// デシリアライズしてカメラに適用
UcApi4UnityCamera.ApplyToCamera(data, camera);
```

### Cinemachineカメラ

```csharp
using UCAPI4Unity.Runtime.CinemachineCamera;

// Cinemachine仮想カメラからシリアライズ
byte[] data = UcApi4CinemachineCamera.SerializeFromVirtualCamera(virtualCamera);

// デシリアライズしてCinemachine仮想カメラに適用
UcApi4CinemachineCamera.ApplyToVirtualCamera(data, virtualCamera, volumeSettings);
```

## ライセンス

本プロジェクトはMITライセンスの下で公開されています。詳細は[LICENSE](LICENSE)ファイルを参照してください。

## 関連ドキュメント

- [UCAPI DLL仕様書](docs/UCAPI_DLL_Specification.md)
- [UCAPIデータフォーマット仕様](docs/UCAPI_Data_Format_Specification.yaml)

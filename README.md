# UniversalCamerawork (UCAPI)

[![Release Build (CD)](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/build.yml/badge.svg)](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/build.yml)
[![CI](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/gtest-ci.yml/badge.svg)](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/gtest-ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

複数プラットフォーム（Unity / Unreal / Blender 等）間でカメラ状態情報（位置、回転、光学パラメータ、タイムコード等）を共通フォーマットで授受するための C++ DLL です。

## 特徴

- **C 互換 Flat Record API** — `#pragma pack(1)` の POD 構造体により、C# P/Invoke、Python ctypes 等の言語バインディングが容易
- **MessagePack シリアライズ** — vcpkg 経由で msgpack-c を統合、ラウンドトリップ実装済
- **独自バイナリ形式** — Header 10 byte + 固定長 Record 128 byte、CRC16-CCITT による整合性検証
- **タイムコード対応** — 14 種フレームレート（23.976〜240fps）+ ドロップフレーム対応
- **Singleton ロガー** — リングバッファ最大 256 エントリ、5 段階ログレベル、スレッドセーフ
- **メモリ安全性** — AddressSanitizer 構成を CI に統合
- **広範なテスト** — GoogleTest による 36+ ケース（往復、並行性、悪意入力、境界値）

## 対応プラットフォーム

| プラットフォーム | 状態 |
|---|---|
| Windows x64 (MSVC v143 / VS2022) | ✅ 対応 |
| Unity (UCAPI4Unity プラグイン) | ✅ 別パッケージで提供 |
| Unreal Engine | 🔜 検討中 |
| Blender | 🔜 検討中 |
| Maya | ⏸ 保留 |
| Linux / macOS | ❌ 未対応 |

## ビルド

### 前提条件

- Visual Studio 2022（MSVC v143、C++17）
- vcpkg（manifest モード対応）

### ビルドコマンド

```bash
cd UCAPIProject/UCAPI

# Debug x64
msbuild UCAPI.sln /p:Configuration=Debug /p:Platform=x64

# Release x64
msbuild UCAPI.sln /p:Configuration=Release /p:Platform=x64

# AddressSanitizer x64（メモリ安全性検証用）
msbuild UCAPI.sln /p:Configuration=ASan /p:Platform=x64
```

依存ライブラリ（msgpack-c、GoogleTest）は `UCAPIProject/UCAPI/vcpkg.json` から vcpkg manifest モードで自動取得されます。

## 使用例

### C++ から利用する場合

```cpp
#include "ucapi_dll.h"

// 1. レコード作成
ucapi_record_flat_t record = {};
record.camera_no = 1;
record.eye_position_right_m = 1.5f;
record.eye_position_up_m = 1.2f;
record.eye_position_forward_m = -3.0f;
record.focal_length_mm = 35.0f;
record.aspect_ratio = 16.0f / 9.0f;
// ... 他のフィールドも設定

// 2. シリアライズ
uint8_t* buffer = nullptr;
size_t bufferSize = 0;
if (UCAPI_SerializeRecord(&record, &buffer, &bufferSize) == 0) {
    // buffer をネットワーク送信 / ファイル保存等

    // 3. デシリアライズ
    ucapi_record_flat_t restored = {};
    if (UCAPI_DeserializeRecord(buffer, bufferSize, &restored) == 0) {
        // restored を使用
    }

    // 4. バッファ解放
    UCAPI_FreeBuffer(buffer);
}
```

### C# / Unity から利用する場合

`ucapi_record_flat_t` は `#pragma pack(1)` の POD 構造体なので、C# 側でも `[StructLayout(LayoutKind.Sequential, Pack = 1)]` でマッピングできます。

```csharp
using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct UcapiRecordFlat
{
    public uint   camera_no;
    public ushort commands;
    public uint   timecode;
    public float  subframe;
    public byte   packet_no;
    public float  eye_position_right_m;
    public float  eye_position_up_m;
    public float  eye_position_forward_m;
    public float  look_vector_right_m;
    public float  look_vector_up_m;
    public float  look_vector_forward_m;
    public float  up_vector_right_m;
    public float  up_vector_up_m;
    public float  up_vector_forward_m;
    public float  focal_length_mm;
    public float  aspect_ratio;
    public float  focus_distance_m;
    public float  aperture;
    public float  sensor_size_width_mm;
    public float  sensor_size_height_mm;
    public float  near_clip_m;
    public float  far_clip_m;
    public float  lens_shift_horizontal_ratio;
    public float  lens_shift_vertical_ratio;
    public float  lens_distortion_radial_coefficients_k1;
    public float  lens_distortion_radial_coefficients_k2;
    public float  lens_distortion_center_point_right_mm;
    public float  lens_distortion_center_point_up_mm;
}

public static class UcapiNative
{
    private const string DllName = "UCAPI_DLL";

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int UCAPI_SerializeRecord(
        ref UcapiRecordFlat record,
        out IntPtr outBuffer,
        out UIntPtr outSize);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int UCAPI_DeserializeRecord(
        IntPtr buffer,
        UIntPtr bufferSize,
        ref UcapiRecordFlat outRecord);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void UCAPI_FreeBuffer(IntPtr buffer);
}

// 使用例（Unity の MonoBehaviour 等から）
public void SendCameraState(Camera cam)
{
    var record = new UcapiRecordFlat
    {
        camera_no = 1,
        eye_position_right_m   = cam.transform.position.x,
        eye_position_up_m      = cam.transform.position.y,
        eye_position_forward_m = cam.transform.position.z,
        focal_length_mm        = cam.focalLength,
        aspect_ratio           = cam.aspect,
    };

    if (UcapiNative.UCAPI_SerializeRecord(ref record, out var buffer, out var size) == 0)
    {
        try
        {
            byte[] managed = new byte[(int)size];
            Marshal.Copy(buffer, managed, 0, managed.Length);
            // managed をネットワーク送信 / ファイル保存等
        }
        finally
        {
            UcapiNative.UCAPI_FreeBuffer(buffer);
        }
    }
}
```

> 完成度の高い Unity 統合は `Unity/UCAPI_Develop/Packages/UCAPI4Unity/` の UPM パッケージとして提供されています（Cinemachine 統合・サンプルシーン含む）。

詳細な API 仕様は [`docs/UCAPI_DLL_Specification.md`](docs/UCAPI_DLL_Specification.md) を参照してください。

## ドキュメント

| ファイル | 内容 |
|---|---|
| [`docs/UCAPI_DLL_Specification.md`](docs/UCAPI_DLL_Specification.md) | DLL 仕様書（API、エラー条件、定数定義） |
| [`docs/UCAPI_Data_Format_Specification.yaml`](docs/UCAPI_Data_Format_Specification.yaml) | バイナリフォーマット仕様 |
| [`memory-bank/progress.md`](memory-bank/progress.md) | 進捗状況、完了タスク、既知の課題 |
| [`memory-bank/technical-debt-tasks.md`](memory-bank/technical-debt-tasks.md) | 技術的負債タスクリスト |

## ライセンス

本プロジェクトは [MIT License](LICENSE) のもとで公開されています。

## GitHub Actions ワークフロー

本プロジェクトでは、継続的インテグレーションとデプロイのために 2 つの GitHub Actions ワークフローを使い分けています。

### 1. 継続的インテグレーション (CI) — `gtest-ci.yml`
**トリガ:** `main` ブランチへの push および全プルリクエスト

**目的:** コード変更を検証し、開発者へ素早いフィードバックを提供する

**実行内容:**
- ソリューションを Release 構成 (x64) でビルド
- GoogleTest スイートを実行
- テストレポートをアーティファクトとして生成・アップロード
- マージ前に全テストの通過を保証

### 2. リリースビルド (CD) — `build.yml`
**トリガ:** バージョンタグの push 時のみ実行 (例: `v1.0.0`、`v2.0.0-beta`)

**目的:** 本番リリース用の成果物を生成する

**実行内容:**
- Release と ASan (AddressSanitizer) の両構成でビルド
- メモリ安全性チェックを含む包括的なテストスイートを実行
- リリース成果物 (UCAPI_DLL.dll および UCAPI_DLL.lib) をアップロード
- 配布用のダウンロード可能なアーティファクトを生成（90 日間保持）

### ワークフロー分離の理由

ワークフローを意図的に分離することで、以下を実現しています:
- **CI 速度の最適化:** gtest-ci ワークフローは高速なビルドとテストに集中し、プルリクエストへの素早いフィードバックを提供
- **リリース品質の確保:** build ワークフローは追加のチェック (ASan) と成果物の発行をタグ付きリリース時のみに限定
- **リソース使用量の削減:** リリースビルドと成果物アップロードは実際のリリース時のみ実施し、毎コミットでは実行しない

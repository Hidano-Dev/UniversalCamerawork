# UCAPI4Unity

Unity plugin for **Universal Camerawork API (UCAPI)**.

## Overview

UCAPI4Unity enables sharing camera state data (position, rotation, zoom, timecode, etc.) across multiple platforms (Unity, Unreal Engine, Blender) using a common binary format.

## Features

- Serialize/Deserialize camera data using UCAPI C++ DLL
- Support for standard Unity Camera
- Support for Cinemachine virtual cameras
- Timecode support with multiple frame rate standards (24fps, 25fps, 30fps, 60fps, etc.)

## Requirements

- Unity 6000.0 or later
- (Optional) Cinemachine package for Cinemachine camera support

## Installation

### Via npm Registry (Scoped Registry)

Add the following to your `Packages/manifest.json`:

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

### Via Unity Package Manager (Git URL)

1. Open Unity Package Manager (Window > Package Manager)
2. Click the "+" button and select "Add package from git URL..."
3. Enter the following URL

```
https://github.com/Hidano-Dev/UniversalCamerawork.git?path=Unity/UCAPI_Develop/Packages/UCAPI4Unity
```

## Directory Structure

```
UCAPI4Unity/
├── Plugins/           # Native DLL (UCAPI_DLL.dll)
├── Runtime/
│   ├── Core/          # Core API (UcApiCore, UcApiRecord, etc.)
│   ├── UnityCamera/   # Standard Unity Camera integration
│   └── CinemachineCamera/  # Cinemachine integration
├── Samples~/          # Sample scenes and scripts
└── Tests/             # Unit tests
```

## Quick Start

### Serialize Camera Data

```csharp
using UCAPI4Unity.Runtime.UnityCamera;

// Serialize camera state to byte array
byte[] data = UcApi4UnityCamera.SerializeFromCamera(camera);
```

### Deserialize Camera Data

```csharp
using UCAPI4Unity.Runtime.UnityCamera;

// Deserialize and apply to camera
UcApi4UnityCamera.ApplyToCamera(data, camera);
```

### Cinemachine Camera

```csharp
using UCAPI4Unity.Runtime.CinemachineCamera;

// Serialize from Cinemachine virtual camera
byte[] data = UcApi4CinemachineCamera.SerializeFromVirtualCamera(virtualCamera);

// Deserialize and apply to Cinemachine virtual camera
UcApi4CinemachineCamera.ApplyToVirtualCamera(data, virtualCamera, volumeSettings);
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Related

- [UCAPI DLL Specification](docs/UCAPI_DLL_Specification.md)
- [UCAPI Data Format Specification](docs/UCAPI_Data_Format_Specification.yaml)

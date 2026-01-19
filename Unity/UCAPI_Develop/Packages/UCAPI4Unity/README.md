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

### Via OpenUPM

If you have [openupm-cli](https://openupm.com/docs/getting-started.html) installed:

```bash
openupm add com.hidano.ucapi4unity
```

Or, add the following to your `Packages/manifest.json`:

```json
{
  "scopedRegistries": [
    {
      "name": "OpenUPM",
      "url": "https://package.openupm.com",
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
using UcApi.Core;

// Create a record from camera
var record = new UcApiRecord();
record.Position = camera.transform.position;
record.Rotation = camera.transform.rotation;
record.FieldOfView = camera.fieldOfView;

// Serialize
byte[] data = UcApiCore.Serialize(record);
```

### Deserialize Camera Data

```csharp
using UcApi.Core;

// Deserialize received data
UcApiRecord record = UcApiCore.Deserialize(data);

// Apply to camera
camera.transform.position = record.Position;
camera.transform.rotation = record.Rotation;
camera.fieldOfView = record.FieldOfView;
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Related

- [UCAPI DLL Specification](docs/UCAPI_DLL_Specification.md)
- [UCAPI Data Format Specification](docs/UCAPI_Data_Format_Specification.yaml)

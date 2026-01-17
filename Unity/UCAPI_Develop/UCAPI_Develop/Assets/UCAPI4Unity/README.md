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

- Unity 2021.3 or later
- (Optional) Cinemachine package for Cinemachine camera support

## Installation

### Via Unity Package Manager (Git URL)

1. Open Unity Package Manager (Window > Package Manager)
2. Click the "+" button and select "Add package from git URL..."
3. Enter the Git URL of this repository

### Manual Installation

1. Copy the `UCAPI4Unity` folder to your project's `Assets` or `Packages` folder

## Directory Structure

```
UCAPI4Unity/
├── Plugins/           # Native DLL (UCAPI_DLL.dll)
├── Runtime/
│   ├── Core/          # Core API (UcApiCore, UcApiRecord, etc.)
│   ├── UnityCamera/   # Standard Unity Camera integration
│   └── CinemachineCamera/  # Cinemachine integration
├── Sample/            # Sample scenes and scripts
└── Tests/             # Unit tests
```

## Quick Start

### Serialize Camera Data

```csharp
using UcApi.Core;

// Create a record from camera
var record = new UcApiRecord();
record.Position = transform.position;
record.Rotation = transform.rotation;
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
transform.position = record.Position;
transform.rotation = record.Rotation;
camera.fieldOfView = record.FieldOfView;
```

## License

See LICENSE file for details.

## Related

- [UCAPI DLL Specification](docs/UCAPI_DLL_Specification.md)
- [UCAPI Data Format Specification](docs/UCAPI_Data_Format_Specification.yaml)

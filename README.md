# UniversalCamerawork

[![Build and Test](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/build.yml/badge.svg)](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/build.yml)
[![CI](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/gtest-ci.yml/badge.svg)](https://github.com/Hidano-Personal/UniversalCamerawork/actions/workflows/gtest-ci.yml)

## GitHub Actions Workflows

This project uses two separate GitHub Actions workflows to handle continuous integration and deployment:

### 1. Continuous Integration (CI) - `gtest-ci.yml`
**Trigger:** Runs on every push to `main` branch and on all pull requests

**Purpose:** Provides fast feedback for developers by validating code changes

**What it does:**
- Builds the solution in Release configuration (x64)
- Runs the GoogleTest suite
- Generates and uploads test reports as artifacts
- Ensures all tests pass before code can be merged

### 2. Release Build (CD) - `build.yml`
**Trigger:** Runs only when a version tag is pushed (e.g., `v1.0.0`, `v2.0.0-beta`)

**Purpose:** Creates production-ready release artifacts

**What it does:**
- Builds both Release and ASan (AddressSanitizer) configurations
- Runs comprehensive test suite including memory safety checks
- Uploads release artifacts (UCAPI_DLL.dll and UCAPI_DLL.lib)
- Creates downloadable artifacts for distribution (retained for 90 days)

### Workflow Separation Rationale

The workflows are intentionally separated to:
- **Optimize CI speed**: The gtest-ci workflow focuses on quick builds and tests for rapid feedback on pull requests
- **Ensure release quality**: The build workflow includes additional checks (ASan) and artifact publishing only for tagged releases
- **Reduce resource usage**: Release builds with artifact uploads only occur for actual releases, not every commit

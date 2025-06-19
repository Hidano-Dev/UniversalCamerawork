# 技術コンテキスト (techContext.md)

## 使用技術
- 開発言語: C++17
- ビルド: Visual Studio 2022 (MSVC)
- テスト: GoogleTest（C++）、NUnit（C#）
- シリアライズ: 独自バイナリ + CRC16, MessagePack(Optional)

## 開発環境
- Windows 10/11
- PowerShell
- Visual Studio 2022

## 技術制約
- クロスプラットフォームAPI非依存
- サイズ軽量化のためSTL最小限使用
- 依存ライブラリは単一ヘッダまたはソースに統合

## 依存関係
- GoogleTest (externally included)
- CRC計算ライブラリ（ヘッダ実装）

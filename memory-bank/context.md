# プロジェクトコンテキスト (context.md)

## 概要
- プロジェクト名: UniversalCamerawork (UCAPI)
- 目的: 複数プラットフォーム間でカメラ状態情報を共通バイナリ形式でシリアライズ／共有するC++ DLLを開発

## 背景・課題
- 各プラットフォーム（Unity、Unreal、Blender）間でのカメラ同期手段が分散していた
- データ整合性のためCRC16チェックを組み込んだ独自バイナリ形式を採用

## 制約条件
- 言語: C++17, ビルド: Visual Studio 2022 (MSVC)
- プラットフォーム依存API非依存設計
- DLLサイズ最小化と高性能化

## 技術スタック
- シリアライズ: 独自バイナリ＋CRC16, MessagePack(Optional)
- テスト: GoogleTest (C++), NUnit (C#)
- 通信: リングバッファ／LMAX Disruptor拡張モデル

## 開発環境
- OS: Windows 10/11, シェル: PowerShell
- IDE: Visual Studio 2022

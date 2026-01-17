# 技術的負債・バグ修正タスク (technical-debt-tasks.md)

コードレビューで発見された技術的問題点を優先度順にリスト化しています。

---

## P1: 重要（品質・保守性向上）

### P1-3: エッジケーステスト・異常系テストの追加
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL_Test/test.cpp`
- **問題**: Happy Pathのみで、以下のテストが不足
  - バッファオーバーフロー攻撃パターン
  - 大量ペイロード（64KB以上）
  - マルチスレッドシナリオ
  - msgpack deserialization失敗ケース
  - 境界値テスト（アンダーフロー/オーバーフロー）
  - 引数不正時の動作検証
- **対応**:
  - 上記シナリオのテストケースを追加
  - 準異常系テストシナリオを定義・実装
- **状態**: [x] 完了 (2026-01-17)
- **統合元**: tasks.md P1「準異常系テストシナリオ定義」「不足テストケース実装」
- **実装詳細**:
  - バッファオーバーフロー攻撃パターン: HeaderOnlyBuffer, MaliciousNumPayload, TruncatedBuffer
  - 大量ペイロード: LargeBuffer_Over64KB (70KBバッファテスト)
  - マルチスレッドシナリオ: ConcurrentCreateDestroy, ConcurrentSerialize, ConcurrentRoundTrip, ConcurrentCRC16
  - msgpack失敗ケース: InvalidFormat_NotMapOrArray, EmptyMap, ReservedBytes, IncompleteString
  - 境界値テスト: EmptyPayload, Length1, SmallValidBuffer
  - 引数不正時: NullOutBuffer, NullOutSize, AllOutputsNull, ZeroSize, NonNullBufferZeroSize, NullPointer(Destroy)
  - ラウンドトリップテスト: SerializeDeserialize
  - 合計22個の新規テストを追加（14→36テスト）

### P1-4: AddressSanitizer/メモリリークテストの導入
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/UCAPI_DLL.vcxproj`
  - `UCAPIProject/UCAPI/UCAPI_DLL_Test/UCAPI_DLL_Test.vcxproj`
- **問題**: メモリ安全性を検証する自動テストがない
- **対応**:
  - MSVC の AddressSanitizer を有効化したビルド構成を追加
  - CI でメモリリークテストを実行
- **状態**: [ ] 未着手

### P1-5: GoogleTestバージョン更新
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL_Test/packages.config`
  - `UCAPIProject/UCAPI/UCAPI_DLL_Test/UCAPI_DLL_Test.vcxproj`
- **問題**: GoogleTest v1.8.1.7（2019年頃）は古く、最新機能が使えない
- **対応**: 最新の安定版（v1.14以降）に更新
- **状態**: [ ] 未着手

### P1-6: 未使用コードの削除 - convert_to_ucapi
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_msgpack_converter.h`, `.cpp`
- **問題**: `convert_to_ucapi()`/`convert_to_msgpack()`が定義されているが未使用。`MessagePackSerializer`が同等機能を持つため重複
- **対応**:
  - 使用箇所がなければ削除
  - または統合して単一の変換パスに整理
- **状態**: [ ] 未着手

---

## P2: 改善推奨（保守性・開発体験向上）

### P2-2: マジックナンバーの定数化
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp`
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.h`
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_dll.cpp`
- **問題**: `0x10000`（64KB制限）などのマジックナンバーがハードコーディング
- **対応**: 名前付き定数として定義し、ドキュメントコメントを追加
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**:
  - `UCAPI_MAGIC` (0x55AA): マジックバイト
  - `UCAPI_HEADER_SIZE` (10): ヘッダーサイズ（バイト）
  - `UCAPI_MAX_PAYLOAD_SIZE` (0x10000): 最大ペイロードサイズ（64KB）
  - `UCAPI_MIN_RECORD_SIZE` (107): 最小レコードサイズ
  - `UCAPI_CRC16_POLYNOMIAL` (0x1021): CRC16-CCITT多項式
  - `UCAPI_CRC16_INIT_VALUE` (0xFFFF): CRC16初期値
  - `UCAPI_CRC16_MSB_MASK` (0x8000): CRC16最上位ビットマスク
  - `UCAPI_BITS_PER_BYTE` (8): バイトあたりのビット数

### P2-3: Singletonロガーの実装
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_logger.h`（新規作成）
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_logger.cpp`（新規作成）
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_dll.h`（API追加）
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_dll.cpp`（std::cerr使用箇所を置換）
- **問題**: 仕様書にSingletonロガーの記載があるが未実装（現在は`std::cerr`のみ）
- **対応**:
  - ロガークラスを実装
  - DLLエクスポート関数にログ取得APIを追加
- **状態**: [ ] 未着手

### P2-4: timecode_tとucapi_tの統合
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_timecode.h`
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_timecode.cpp`
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.h`
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp`
- **問題**: `timecode_t`クラスが実装されているが、`ucapi_t::m_timecode`は`uint32_t`型で互換性がない
- **対応**:
  - `timecode_t`を`ucapi_t`から使用するよう統合
  - または不要なら削除
- **状態**: [ ] 未着手

### P2-5: コメント言語の統一
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp`（日本語コメント：行8, 33, 42, 45, 50）
  - `UCAPIProject/UCAPI/UCAPI_DLL/*.h`, `*.cpp`（全般）
- **問題**: 日本語と英語のコメントが混在
- **対応**: プロジェクトの方針を決めて統一
- **状態**: [ ] 未着手

### P2-6: 仕様書と実装の同期・ドキュメント整備
- **ファイル**:
  - `docs/UCAPI_DLL_Specification.md`
  - `docs/UCAPI_Data_Format_Specification.yaml`
  - `.github/copilot-instructions.md`
  - `README.md`
- **問題**:
  - Transport層が「予定」のまま（→DLL範囲外として明記）
  - Loggerが未実装
  - Converterの使用状況が不明確
  - 各APIのエラー条件（引数不正、内部例外など）が未明文化
- **対応**:
  - 実装状況に合わせて仕様書を更新
  - DLL範囲外の機能を明確に記載
  - 各APIのエラー条件・戻り値を仕様書に明文化
  - READMEの更新
- **状態**: [ ] 未着手
- **統合元**: tasks.md P1「APIエラー条件明文化」、P2「ドキュメント整備」

### P2-7: CI/CD環境構築
- **ファイル**:
  - `.github/workflows/` (新規作成)
  - `README.md`
- **問題**: 継続的インテグレーション環境がない
- **対応**:
  - GitHub Actionsでビルド・テスト自動化
  - ビルドバッジをREADMEに追加
- **状態**: [x] 完了 (2026-01-17)
- **統合元**: tasks.md P2「CI定義＆README更新」
- **実装詳細**:
  - `.github/workflows/build.yml`を作成
  - トリガー: mainへのpush + mainへのPR
  - 構成: Release x64
  - ステップ: vcpkgセットアップ → NuGet復元 → ビルド → テスト → アーティファクト保存
  - README.mdにビルドバッジを追加

### P2-8: FlatBuffers対応シリアライザ実装（将来検討）
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_flatbuffers_serializer.h` (新規)
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_flatbuffers_serializer.cpp` (新規)
- **問題**: ゼロコピーデシリアライズが必要な高性能ユースケースでMessagePackでは不十分な可能性
- **対応**:
  - FlatBuffersスキーマ定義
  - IMessageSerializer実装
  - パフォーマンス比較テスト
- **状態**: [ ] 保留（必要性が明確になってから着手）
- **統合元**: tasks.md P0「FlatBuffers対応シリアライザ実装」
- **備考**: 現状MessagePackで十分。パフォーマンス問題発生時に検討

### P2-9: Mayaプラグイン骨子プロジェクト作成
- **ファイル**:
  - `Maya/` (新規ディレクトリ)
- **問題**: Maya対応プラグインが未着手
- **対応**:
  - Maya Python/C++ API用のプロジェクト作成
  - DLL呼び出しラッパー実装
- **状態**: [ ] 保留（需要確認後に着手）
- **統合元**: tasks.md P1「Unity/Mayaプラグイン骨子プロジェクト作成」
- **備考**: Unityプラグイン（UCAPI4Unity）は既存。Maya対応の需要を確認後に実装

---

## 完了済み

### P0-1: メモリリーク修正 - payloadArray ✓
- 2026-01-17 完了
- 未使用の`payloadArray`変数を削除

### P0-2: メモリリーク修正 - reserved配列 ✓
- 2026-01-17 完了
- クラスメンバとして使用されていないローカル変数`reserved`を削除

### P0-3: バッファオーバーフロー修正 - record_t::_read() ✓
- 2026-01-17 完了
- `payload_length`を使用した境界チェックを追加（最小107バイト）

### P0-4: バッファオーバーフロー修正 - ucapi_t::_read() ✓
- 2026-01-17 完了
- `bufferSize`パラメータ追加、ヘッダ・ペイロードの境界チェックを追加

### P0-5: スレッド安全性修正 - Config初期化 ✓
- 2026-01-17 完了
- `std::call_once` + `std::atomic<bool>` でスレッドセーフ化

### P0-6: 例外時リソースリーク修正 - UCAPI_Deserialize ✓
- 2026-01-17 完了
- `std::make_unique<ucapi_t>()`でRAII化、成功時に`release()`で返却

### P1-1: _clean_up()関数の実装 ✓
- 2026-01-17 完了
- `record_t::_clean_up()`に将来の拡張に備えたドキュメントコメントを追加

### P1-2: CRC16テストに参照値を追加 ✓
- 2026-01-17 完了
- CRC16-CCITT-FALSE標準テストベクタ "123456789" -> 0x29B1 を追加
- 単一バイトテスト (0x00 -> 0xE1F0, 0xFF -> 0xFF00) を追加
- CRC-16/XMODEMテストベクタ "123456789" -> 0x31C3 を追加
- 複数バイトテスト (4x0x00 -> 0x84C0, 4x0xFF -> 0x1D0F) を追加

### P2-1: vcpkg依存関係の明示化 ✓
- 2026-01-17 完了
- `UCAPIProject/UCAPI/vcpkg.json`を作成（msgpack依存を明示）
- `UCAPI_DLL.vcxproj`にマニフェストモード有効化 + MSGPACK_NO_BOOST追加
- vcpkgをD:\vcpkgにインストール、VS2022にMSBuild統合

### P1-3: エッジケーステスト・異常系テストの追加 ✓
- 2026-01-17 完了
- バッファオーバーフロー攻撃パターン: HeaderOnlyBuffer, MaliciousNumPayload, TruncatedBuffer
- 大量ペイロード: LargeBuffer_Over64KB (70KBバッファテスト)
- マルチスレッドシナリオ: ConcurrentCreateDestroy, ConcurrentSerialize, ConcurrentRoundTrip, ConcurrentCRC16
- msgpack失敗ケース: InvalidFormat_NotMapOrArray, EmptyMap, ReservedBytes, IncompleteString
- 境界値テスト: EmptyPayload, Length1, SmallValidBuffer
- 引数不正時: NullOutBuffer, NullOutSize, AllOutputsNull, ZeroSize, NonNullBufferZeroSize, NullPointer(Destroy)
- ラウンドトリップテスト: SerializeDeserialize
- 合計22個の新規テストを追加（14→36テスト）

### P2-7: CI/CD環境構築 ✓
- 2026-01-17 完了
- `.github/workflows/build.yml`を作成（GitHub Actions）
- トリガー: mainへのpush + mainへのPR、構成: Release x64
- README.mdにビルドバッジを追加

### P2-2: マジックナンバーの定数化 ✓
- 2026-01-17 完了
- ucapi.hに8つの定数を追加（UCAPI_MAGIC, UCAPI_HEADER_SIZE, UCAPI_MAX_PAYLOAD_SIZE, UCAPI_MIN_RECORD_SIZE, CRC16関連4定数）
- ucapi.cpp, ucapi_dll.cppのハードコーディングを定数参照に置換
- 全定数にドキュメントコメントを追加

---

## 更新履歴

- 2026-01-17: P2-2完了（マジックナンバーの定数化、8定数をucapi.hに定義）
- 2026-01-17: P1-3完了（エッジケーステスト・異常系テスト22個追加）
- 2026-01-17: P2-7完了（GitHub Actions CI/CD環境構築、ビルドバッジ追加）
- 2026-01-17: P2-1完了（vcpkg.json作成、マニフェストモード有効化、MSGPACK_NO_BOOST追加）
- 2026-01-17: P1-2完了（CRC16テストに業界標準参照値を追加）
- 2026-01-17: tasks.mdから有効なタスクを統合（P1-3拡張、P2-6拡張、P2-7/P2-8/P2-9新規追加）
- 2026-01-17: P0-6完了（UCAPI_Deserializeのstd::unique_ptrによるRAII化）
- 2026-01-17: P1-1完了（_clean_up()関数にドキュメントコメント追加）
- 2026-01-17: P0-1〜P0-4完了（メモリリーク修正、バッファオーバーフロー修正）
- 2026-01-17: P0-5完了（Config初期化のスレッドセーフ化）
- 2026-01-17: 初版作成（コードレビューに基づく技術的問題点の整理）

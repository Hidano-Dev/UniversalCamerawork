# 技術的負債・バグ修正タスク (technical-debt-tasks.md)

コードレビューで発見された技術的問題点を優先度順にリスト化しています。

---

## P0: 緊急対応（セキュリティ・安定性に直結）

### P0-1: メモリリーク修正 - payloadArray
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp` 行57
- **問題**: `new std::vector<record_t>()` が割り当てられたまま未使用・未削除
- **対応**: 不要なコードを削除、または意図した用途があれば適切に使用・削除する
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**: 未使用の`payloadArray`変数を削除

### P0-2: メモリリーク修正 - reserved配列
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp` 行124
- **問題**: `record_t`コンストラクタで`uint8_t* reserved = new uint8_t[25]`が確保されるが、`_clean_up()`が空実装のためデストラクタで削除されない
- **対応**:
  - `_clean_up()`に削除処理を追加
  - または`std::array<uint8_t, 25>`に変更してRAII化
  - または`std::unique_ptr<uint8_t[]>`を使用
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**: クラスメンバとして使用されていないローカル変数`reserved`を削除

### P0-3: バッファオーバーフロー修正 - record_t::_read()
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp` 行138-169
- **問題**:
  - `payload_length`パラメータが宣言されているが使用されていない
  - 入力バッファの境界チェックがない
  - offset 103までアクセスするが、データ長の検証なし
- **対応**:
  - `payload_length`を使用してアクセス前に境界チェック
  - 最小必要サイズ（107バイト以上）の検証を追加
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**:
  - `MIN_RECORD_SIZE = 107`の定数を追加
  - `payload_length < MIN_RECORD_SIZE`の場合は例外をスロー

### P0-4: バッファオーバーフロー修正 - ucapi_t::_read()
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp` 行31-68
- **問題**:
  - ヘッダ読み込み時に最小長（8バイト）のチェックがない
  - `m_num_payload`が異常値の場合、ループが暴走
  - `&data[10 + i * payloadLength]`へのアクセスが境界外になる可能性
- **対応**:
  - ヘッダ読み込み前に最小バッファ長を検証
  - `m_num_payload`の上限値チェック
  - ペイロードアクセス前にバッファ長との整合性を検証
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**:
  - コンストラクタとヘッダに`bufferSize`パラメータを追加
  - `HEADER_SIZE = 10`、`MAX_PAYLOAD_SIZE = 0x10000`の定数を追加
  - ヘッダ読み込み前に`bufferSize < HEADER_SIZE`チェック
  - ペイロード読み込み前に`bufferSize < requiredSize`チェック

### P0-5: スレッド安全性修正 - Config初期化
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_config.cpp` 行18-23
- **問題**:
  - `s_initialized`と`s_factory`のチェック・設定がthread-unsafe
  - DLL Mainから呼び出されるため、複数スレッドでの競合状態（TOCTOU問題）が発生可能
- **対応**:
  - `std::call_once`と`std::once_flag`を使用してthread-safe化
  - またはC++11のスタティックローカル変数初期化（Magic Statics）を利用
- **状態**: [x] 完了
- **実装詳細**:
  - `s_initialized`を`std::atomic<bool>`に変更
  - `std::once_flag s_init_flag`を追加
  - `Initialize()`で`std::call_once`を使用してスレッドセーフ化
  - `memory_order_acquire/release`でメモリ順序を保証

### P0-6: 例外時リソースリーク修正 - UCAPI_Deserialize
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi_dll.cpp` 行9-42
- **問題**: `new ucapi_t()`後に例外が発生した場合、割り当てたメモリが削除されない
- **対応**:
  - `std::unique_ptr<ucapi_t>`を使用してRAII化
  - 成功時に`release()`でポインタを返却
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**:
  - `<memory>`ヘッダを追加
  - `new ucapi_t()`を`std::make_unique<ucapi_t>()`に変更
  - 成功時に`native.release()`でポインタを返却
  - 例外発生時は`unique_ptr`のデストラクタが自動的にメモリ解放

---

## P1: 重要（品質・保守性向上）

### P1-1: _clean_up()関数の実装
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp` 行194-198
- **問題**: `_clean_up()`が空実装のため、リソースクリーンアップが機能していない
- **対応**: 動的確保したリソースの削除処理を実装
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**:
  - 現在の`record_t`は全メンバがPOD型（uint32_t, uint16_t, uint8_t, float）で動的リソースなし
  - `ucapi_t::_clean_up()`は既に`m_payload.clear()`を実装済み
  - `record_t::_clean_up()`に将来の拡張に備えたドキュメントコメントを追加

### P1-2: CRC16テストに参照値を追加
- **ファイル**: `UCAPIProject/UCAPI/UCAPI_DLL_Test/test.cpp`
- **問題**: CRC16テストが決定性のみを確認し、計算結果の正当性を検証していない
- **対応**:
  - 業界標準のCRC16-CCITT参照値との比較テストを追加
  - 複数のテストベクタで正当性を検証
- **状態**: [x] 完了 (2026-01-17)
- **実装詳細**:
  - CRC16-CCITT-FALSE標準テストベクタ "123456789" -> 0x29B1 を追加
  - 単一バイトテスト (0x00 -> 0xE1F0, 0xFF -> 0xFF00) を追加
  - CRC-16/XMODEMテストベクタ (init=0x0000) "123456789" -> 0x31C3 を追加
  - 複数バイトテスト (4x0x00 -> 0x84C0, 4x0xFF -> 0x1D0F) を追加

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
- **状態**: [ ] 未着手
- **統合元**: tasks.md P1「準異常系テストシナリオ定義」「不足テストケース実装」

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

### P2-1: vcpkg依存関係の明示化
- **ファイル**:
  - `vcpkg.json`（新規作成）
  - `UCAPIProject/UCAPI/UCAPI_DLL/UCAPI_DLL.vcxproj`
- **問題**: `vcpkg.json`がなく、msgpack等の依存関係バージョンが不明確
- **対応**: `vcpkg.json`を作成し、依存ライブラリを明示
- **状態**: [ ] 未着手

### P2-2: マジックナンバーの定数化
- **ファイル**:
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.cpp`
  - `UCAPIProject/UCAPI/UCAPI_DLL/ucapi.h`
- **問題**: `0x10000`（64KB制限）などのマジックナンバーがハードコーディング
- **対応**: 名前付き定数として定義し、ドキュメントコメントを追加
- **状態**: [ ] 未着手

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
- **状態**: [ ] 未着手
- **統合元**: tasks.md P2「CI定義＆README更新」

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

---

## 更新履歴

- 2026-01-17: P1-2完了（CRC16テストに業界標準参照値を追加）
- 2026-01-17: tasks.mdから有効なタスクを統合（P1-3拡張、P2-6拡張、P2-7/P2-8/P2-9新規追加）
- 2026-01-17: P0-6完了（UCAPI_Deserializeのstd::unique_ptrによるRAII化）
- 2026-01-17: P1-1完了（_clean_up()関数にドキュメントコメント追加）
- 2026-01-17: P0-1〜P0-4完了（メモリリーク修正、バッファオーバーフロー修正）
- 2026-01-17: P0-5完了（Config初期化のスレッドセーフ化）
- 2026-01-17: 初版作成（コードレビューに基づく技術的問題点の整理）

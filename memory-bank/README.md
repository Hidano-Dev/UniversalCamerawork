# Memory Bank

このディレクトリは、プロジェクトの知見・タスク管理・設計決定を集約するための場所です。
AIアシスタント（Claude Code等）がプロジェクトのコンテキストを理解し、一貫した支援を行うために活用されます。

## ファイル一覧と役割

### コンテキスト系
| ファイル | 役割 |
|----------|------|
| `activeContext.md` | 現在の作業フォーカス、最新の変更履歴 |
| `context.md` | プロジェクト概要、背景、制約条件のサマリ |
| `productContext.md` | プロジェクトの背景・狙い・解決する問題 |
| `projectbrief.md` | プロジェクト概要、ゴール、制約条件のブリーフ |

### 技術系
| ファイル | 役割 |
|----------|------|
| `techContext.md` | 使用技術、開発環境、依存関係、技術制約 |
| `systemPatterns.md` | アーキテクチャ、デザインパターン、コンポーネント関係 |
| `project-knowledge.md` | 技術的知見、設計決定、実装パターンとアンチパターン |
| `common-patterns.md` | よく使用するコマンドやテンプレート |

### 進捗・タスク系
| ファイル | 役割 |
|----------|------|
| `progress.md` | 進捗状況、完了タスク、既知の課題 |
| `technical-debt-tasks.md` | 技術的負債・バグ修正タスクリスト（P0〜P2優先度） |
| `ucapi4unity-todo.md` | Unityプラグイン（UCAPI4Unity）のTODOリスト |

### 履歴系
| ファイル | 役割 |
|----------|------|
| `chatContext.md` | AIアシスタントとの議論内容・決定事項の記録 |
| `project-improvements.md` | 改善履歴、試行錯誤の記録と教訓 |

## 使い方

### AIアシスタント向け
1. **作業開始時**: `activeContext.md` と `progress.md` で現在の状況を把握
2. **技術調査時**: `techContext.md`、`systemPatterns.md`、`project-knowledge.md` を参照
3. **タスク確認時**: `technical-debt-tasks.md` で優先度と状態を確認
4. **作業完了時**: 関連ファイルを更新し、知見を蓄積

### 開発者向け
1. **プロジェクト理解**: `projectbrief.md` → `context.md` → `systemPatterns.md` の順で読む
2. **タスク確認**: `technical-debt-tasks.md` で未着手タスクを確認
3. **ビルド・テスト**: `common-patterns.md` のコマンドを参照

## 更新ルール

- 各ファイルは継続的に更新し、常に現状を反映
- CLAUDE.mdをプロジェクトの正式なガイドとして参照
- 日本語ドキュメントで統一
- 重要な設計決定や教訓は必ず記録

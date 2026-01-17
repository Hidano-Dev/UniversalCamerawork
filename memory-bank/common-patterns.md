# よく使用するコマンド・テンプレート (memory-bank/common-patterns.md)

## プロジェクト初期化
```
claude /init
```

## C++ビルド
```
# Debug x64 ビルド
msbuild UCAPIProject\UCAPI\UCAPI_DLL.sln /p:Configuration=Debug /p:Platform=x64

# Release x64 ビルド
msbuild UCAPIProject\UCAPI\UCAPI_DLL.sln /p:Configuration=Release /p:Platform=x64
```

## テスト実行
```
# 単体テストプロジェクトのみビルド
msbuild UCAPIProject\UCAPI\UCAPI_DLL_Test.vcxproj /p:Configuration=Debug /p:Platform=x64

# Visual Studio Test Explorerから実行、またはビルド後の実行ファイルを直接実行
```

## Serializer生成
```
claude create serializer {SerializerName} --interface IMessageSerializer
```

## エラー解析
```
claude analyze error log and suggest fixes for [error-description]
```

# よく使用するコマンド・テンプレート (memory-bank/common-patterns.md)

## プロジェクト初期化
```
claude /init
```

## C++ビルド
```
msbuild UCAPIProject\UCAPI\UCAPI.sln /p:Configuration=Release /p:Platform=x64
```

## テスト実行
```
msbuild UCAPIProject\UCAPI\UCAPI_TestTool\UCAPI_TestTool.sln /t:Test
```

## Serializer生成
```
claude create serializer {SerializerName} --interface IMessageSerializer
```

## エラー解析
```
claude analyze error log and suggest fixes for [error-description]
```

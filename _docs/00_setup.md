# 00. Setup #

## repository ##

使用Git Submodule功能在`modules`目录下管理扩展module。

## auto/configure ##

运行auto脚本，生成平台相关的配置头文件，生成 `objs/` 文件夹

```bash
$ ./auto/configure
```

### nginx-rtmp-module ###

```bash
$ ./auto/configure --add-module=./modules/nginx-rtmp-module/
```
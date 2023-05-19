# Setup & Build Nginx Source Code #

## 1. Repository ##

使用Git Submodule功能在 `modules` 目录下管理扩展模块，如：`ngx_rtmp_module`。

## 2. Build ##

新版本的Nginx模块源码构建脚本位于 `/auto` 目录下，执行后生成平台相关的配置头文件和 `Makefile` ，都位于 `/objs` 目录下。

如果需要在macOS平台下编译Nginx源码，则需要依赖一些开放三方库，如： `openssl`, `pcre`, `zlib` 等。

```bash
# 在macOS上源码编译安装Nginx

# 如果依赖的三方模块没有安装，可以通过brew直接安装
$ brew install pcre zlib openssl
$
$ cd [ngx_src_path]
$ ./auto/configure --prefix=/tmp/desert/nginx --with-debug --with-pcre \
                   --with-http_ssl_module --with-openssl=/usr/local/opt/openssl
```

如果遇到 `openssl` 模块路径问题，可以尝试修改 `/auto/lib/openssl/conf` 文件中的配置。

```shell
# 注意路径：源码中的路径有 .openssl 子目录，那是根据 openssl 源码编译出来的产出目录
 39             CORE_INCS="$CORE_INCS $OPENSSL/include"
 40             CORE_DEPS="$CORE_DEPS $OPENSSL/include/openssl/ssl.h"
 41             CORE_LIBS="$CORE_LIBS $OPENSSL/lib/libssl.a"
 42             CORE_LIBS="$CORE_LIBS $OPENSSL/lib/libcrypto.a"
 43             CORE_LIBS="$CORE_LIBS $NGX_LIBDL"
 44             CORE_LIBS="$CORE_LIBS $NGX_LIBPTHREAD"
```

### 2.1. nginx-rtmp-module ###

```bash
# 编译扩展模块 nginx-rtmp-module 时只需要在编译命理中附带模块地址即可（同样注意 openssl 路径问题）
$ ./auto/configure --prefix=/tmp/desert/nginx --with-debug --with-pcre \
                   --with-http_ssl_module --with-openssl=/usr/local/opt/openssl \
                   --add-module=./modules/nginx-rtmp-module/
```

### 2.2. nginx-lua-module ###

`nginx-lua-module` 模块还额外依赖了 `luajit2` 和 `ngx_devel_kit`；其中 `luajit2` 工具需要前置编译安装完成，`ngx_devel_kit` 作为附加模块一并编译即可。
编译过程中注意 `luajit2` 的头文件和库文件地址配置，配置文件位于 `nginx-lua-module/config` 中，现已手动额外配置。

```bash
# ./modules/nginx-lua-module/config
LUAJIT_INC=/usr/local/include/luajit-2.1
LUAJIT_LIB=/usr/local/lib
```

```bash
$ ./auto/configure --prefix=/tmp/desert/nginx --with-debug --with-pcre \
                   --with-http_ssl_module --with-openssl=/usr/local/opt/openssl \
                   --add-module=./modules/nginx-devel-kit --add-module=./modules/nginx-lua-module
```
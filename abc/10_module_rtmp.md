# ngx_rtmp_module #


## 模块的初始化过程

在Nginx配置解析和初始化的过程中，会调用各模块定义结构体`ngx_module_t`中的指令处理回调函数 `ngx_rtmp_block` 模块配置初始化过程的回调函数，处理配置文件中关于rtmp的配置，最后会调用 `ngx_rtmp_optimize_servers()` 来设置 `listening->handler = ngx_rtmp_init_connection`


## 模块处理事件主流程
ngx_event_accept
listening->handler = ngx_rtmp_init_connection

主要调用栈：
```c++
ngx_rtmp_init_connection(ngx_connection_t *c)
    ngx_rtmp_init_session(ngx_connection_t *c, ngx_rtmp_addr_conf_t *addr_conf)
    ngx_rtmp_handshake(ngx_rtmp_session_t *s)
        ngx_rtmp_handshake_done(ngx_rtmp_session_t *s)
            ngx_rtmp_cycle(ngx_rtmp_session_t *s)
                ngx_rtmp_recv(ngx_event_t *rev)
                    ngx_rtmp_receive_message(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h, ngx_chain_t *in)
                        cmcf->events handlers
```

## RTMP推流参数过长问题解决

推流命令中可以带有参数，这些参数可以头传到业务侧，但基于现有实现，对参数读取有最大长度限制。

解决方案是在 `ngx_rtmp_cmd_module` 中，解析RTMP协议AMF包时，放宽对参数长度的限定。

```c++
// ngx_rtmp_cmd_module.h
#define NGX_RTMP_MAX_NAME           1024
#define NGX_RTMP_MAX_URL            1024
#define NGX_RTMP_MAX_ARGS           NGX_RTMP_MAX_NAME
```


```text
 2021/11/08 12:42:28 [debug] 65272#0: *1 recv: eof:0, avail:728, err:0
 2021/11/08 12:42:28 [debug] 65272#0: *1 recv: fd:10 728 of 4018
 2021/11/08 12:42:28 [debug] 65272#0: *1 RTMP bheader fmt=0 csid=8
 2021/11/08 12:42:28 [debug] 65272#0: *1 RTMP mheader fmt=0 amf_cmd (20) time=0+0 mlen=716 len=0 msid=1
 2021/11/08 12:42:28 [debug] 65272#0: *1 RTMP recv amf_cmd (20) csid=8 timestamp=0 mlen=716 msid=1 nbufs=1
 2021/11/08 12:42:28 [debug] 65272#0: *1 nhandlers: 1
 2021/11/08 12:42:28 [debug] 65272#0: *1 calling handler 0
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (1) 02 '?'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (2) 00 07 '??'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (7) 70 75 62 6C 69 73 68 'publish'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF func 'publish' passed to handler 0/1
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (1) 00 '?'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (8) 40 14 00 00 00 00 00 00 '@???????'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (1) 05 '?'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (1) 02 '?'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (2) 02 AE '??'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (686) 72 74 6D 70 2D 74 65 73 74 2D 6C 69 76 65 2D 63 'rtmp-test-live-c'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (1) 02 '?'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (2) 00 04 '??'
 2021/11/08 12:42:28 [debug] 65272#0: *1 AMF read (4) 6C 69 76 65 'live'
 2021/11/08 12:42:28 [info] 65272#0: *1 publish: name='rtmp-test-live-channel' args='Expires=1635574750&OSSAccessKeyId=LTAI5tMRgWjS8f88YBqgJj5s&Signature=Bne9XOon3wE8If6yyPC1rc4mJS4%3D&playlistName=rtmp-test-playlist.m3u8&SecurityToken=X7W49ME7THYJNRKAZTYYZJTBVDBZ4T88GMYX8HFQNZG8IGSK6QU8OEU09KN4VYDEPO8HR534OCNZ7Q0NJYQH566NQYN4OKNRUNHBPSGCSIP4S0VTCMT6LEI6QNJEEH5LFBMPK9JPDCHYOU1EOBRHHMA6BZPCHIXGZMGFA2AR9Y8P0YOKCETX1ZE7SU2Z9Y38YQAVQCKZ69F2ML53I74FOHODCOGLPNGOULR90N5OTMAYKTELKKCYBUBCF5O8POBMG435KBB 7PHI7HY4NQOWVG1D34VH9R1Z7NFSEF8ZQ5CO2WQJ4D8NCNDZLFIBS0I4EBXFVK1CYHO169LB3GHF0K8CUYA5H5S35VT2F33G5LCIEY7QUOFFAKWUAOVW7I888R2TAQUOQB6VH16T88W4IMSLM5UVNPXEII056I2SAN5Z2K5HD4RJHEEWI2AD7HLO0QK4JJZJ59NKE8BRJQ5ARO08JDJRDI1PJLUU447NTJKKIOFTN1EK0ZRNCUBQ5EWE2' type=live silent=0, client:     127.0.0.1, server: 0.0.0.0:1935
 2021/11/08 12:42:28 [debug] 65272#0: *1 live: publish: name='rtmp-test-live-channel' type='live'
 2021/11/08 12:42:28 [debug] 65272#0: *1 live: join 'rtmp-test-live-channel'
 2021/11/08 12:42:28 [debug] 65272#0: *1 live: create stream 'rtmp-test-live-channel'
```
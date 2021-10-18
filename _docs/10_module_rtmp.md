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
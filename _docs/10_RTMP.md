

## nginx_rtmp_modules

ngx_rtmp_module;
ngx_rtmp_core_module;
ngx_rtmp_cmd_module;
ngx_rtmp_codec_module;
ngx_rtmp_access_module;
ngx_rtmp_record_module;
ngx_rtmp_live_module;
ngx_rtmp_play_module;
ngx_rtmp_flv_module;
ngx_rtmp_mp4_module;
ngx_rtmp_netcall_module;
ngx_rtmp_relay_module;
ngx_rtmp_exec_module;
ngx_rtmp_auto_push_module;
ngx_rtmp_auto_push_index_module;
ngx_rtmp_notify_module;
ngx_rtmp_log_module;
ngx_rtmp_limit_module;
ngx_rtmp_hls_module;
ngx_rtmp_dash_module;

ngx_rtmp_modules 中有2个 NGX_CORE_MODULE 类型
ngx_rtmp_module
ngx_rtmp_auto_push_module


### rtmp_module 配置初始化
ngx_rtmp.c
ngx_rtmp_block()
ngx_rtmp_optimize_servers()
listening->handler = ngx_rtmp_init_connection


### rtmp_module 事件初始化
ngx_event_accept
listening->handler ->
ngx_rtmp_init_connection ->
ingx_rtmp_init_session ->
ngx_rtmp_handshake
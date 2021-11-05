
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16

#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


// 回调函数指针，参数是一个无类型指针
typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

// 注意这里定义是一个链表
struct ngx_pool_cleanup_s {
    // 回收时的回调函数指针
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    // 下一个清理节点
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    // 指向下一个大数据块
    ngx_pool_large_t     *next;
    // 分配的大数据块内存
    void                 *alloc;
};


// 匿名结构体，内存池中的数据块
typedef struct {
    // 保存当前内存池中已经分配地址的最末尾，也就是下次分配的起始位置
    u_char               *last;
    // 当前内存池的结束位置
    u_char               *end;
    // 内存池中有多个数据块，next指向下一个内存块
    ngx_pool_t           *next;
    // 尝试分配内存失败的次数统计
    ngx_uint_t            failed;
} ngx_pool_data_t;


// 内存池
struct ngx_pool_s {
    // 数据块
    ngx_pool_data_t       d;
    // 内存池中数据块大小的最大值，如果数据块超过这个最大值则会在large内存池中分配
    size_t                max;
    // 指向当前内存池，即指向自己，起始位置指针
    ngx_pool_t           *current;

    ngx_chain_t          *chain;
    // 专门分配大数据块的内存池链表
    ngx_pool_large_t     *large;
    // 回收内存池时的回调信息，其中包括清理用的回调函数指针
    ngx_pool_cleanup_t   *cleanup;
    // 日志输出
    ngx_log_t            *log;
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


// 创建一个内存池
ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
// 销毁回收一个内存池
void ngx_destroy_pool(ngx_pool_t *pool);
// 重置内存池
void ngx_reset_pool(ngx_pool_t *pool);

// 分配一个大小为size的内存空间
void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */


/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


// 动态数组
typedef struct {
    // 数组实际的数据存储位置，等效数组首地址
    void        *elts;
    // 数组实际元素个数
    ngx_uint_t   nelts;
    // 数组单个元素的大小，单位字节
    size_t       size;
    // 数组的容量，表示该数组在不引发扩容的前提下，可以最多存储的元素的个数
    ngx_uint_t   nalloc;
    // 当前动态数组分配内存的内存池
    ngx_pool_t  *pool;
} ngx_array_t;


// 创建一个新的数组对象，并返回这个对象
ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);

// 销毁该数组对象，并释放其分配的内存回内存池
void ngx_array_destroy(ngx_array_t *a);

// 在数组a上增加1个新元素，返回新元素的地址；返回新元素地址后需要进行类型转换，再赋值
void *ngx_array_push(ngx_array_t *a);

// 在数组a上增加n个新元素，返回增加的n个元素的首地址；需要重新赋值
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    // 在 pool 中分配内存
    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */

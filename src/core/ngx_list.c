
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

// 在pool中申请一个list ，list中part的元素为n个，每个元素为size大小,并返回list地址
ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, sizeof(ngx_list_t));
    if (list == NULL) {
        return NULL;
    }

    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NULL;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return list;
}

// 向给定的list 的最后一个元素数组中追加信息，若最后一个元素数组已经满了，则再重新申请一个
// 并返回追加信息的地址
// 返回的elt是一个ngx_str_t的指针，这个函数的用法通常是调用ngx_list_push得到返回的元素地址，在对返回的地址进行赋值。
// 例如： ngx_str_t * str = ngx_list_push(testlist);
//        if ( str  == NULL ) {
//          return NGX_ERROR;
//        }
//        str->len ＝ sizeof("Hello World");
//        str->value = "Hello World";
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    // 定位列表的最后一个节点
    last = l->last;

    // 判断最后一个part中存储的数据是否已经满了
    if (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        // 将新申请的list节点添加到list末尾
        l->last->next = last;
        l->last = last;
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}

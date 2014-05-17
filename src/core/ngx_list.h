
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;

// ngx_list_t ����������������ngx_list_part_sֻ����������һ��Ԫ��
struct ngx_list_part_s {
    void             *elts; // �����б��ռ���׵�ַ
    ngx_uint_t        nelts; // ��ǰ�Ѿ�ʹ�õĿռ�����(list�б��У�ÿ���ڵ���ʵ��Ԫ��������ÿ���ڵ��е����ݽṹΪ����)
    ngx_list_part_t  *next; // ָ����һ���б��ڵ�
};


typedef struct {
    ngx_list_part_t  *last; // list�����һ��part
    ngx_list_part_t   part; // list��ͷ��part
    size_t            size; // ������ÿ��Ԫ�صĴ�С
    ngx_uint_t        nalloc; // �ѷ���ռ��пɴ�ŵ�Ԫ�ظ���
    ngx_pool_t       *pool; // ��ǰlist���ݴ�ŵ��ڴ��
} ngx_list_t;

// ngx_list_create��ngx_list_init������һ���Ķ��Ǵ���һ��list��ֻ�Ƿ���ֵ��һ��...
ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size); // ���ڴ������ռ����eltsָ����ÿռ�
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0; // �շ�����������ûʹ�ã�����Ϊ0
    list->part.next = NULL;
    list->last = &list->part; // last��ʼ��ʱ��ָ���׽ڵ�
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
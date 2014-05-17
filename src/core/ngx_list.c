
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

// ��pool������һ��list ��list��part��Ԫ��Ϊn����ÿ��Ԫ��Ϊsize��С,������list��ַ
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

// �������list �����һ��Ԫ��������׷����Ϣ�������һ��Ԫ�������Ѿ����ˣ�������������һ��
// ������׷����Ϣ�ĵ�ַ
// ���ص�elt��һ��ngx_str_t��ָ�룬����������÷�ͨ���ǵ���ngx_list_push�õ����ص�Ԫ�ص�ַ���ڶԷ��صĵ�ַ���и�ֵ��
// ���磺 ngx_str_t * str = ngx_list_push(testlist);
//        if ( str  == NULL ) {
//          return NGX_ERROR;
//        }
//        str->len �� sizeof("Hello World");
//        str->value = "Hello World";
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    // ��λ�б�����һ���ڵ�
    last = l->last;

    // �ж����һ��part�д洢�������Ƿ��Ѿ�����
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

        // ���������list�ڵ���ӵ�listĩβ
        l->last->next = last;
        l->last = last;
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}

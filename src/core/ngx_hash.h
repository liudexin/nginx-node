
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

//�ο���
//http://blog.csdn.net/livelylittlefish/article/details/6636229
//hashԪ�ؽṹ 
typedef struct {
    void             *value;    //value����ĳ��key��Ӧ��ֵ����<key,value>�е�value 
    u_short           len;      //name����
    u_char            name[1];  //ĳ��Ҫhash������(��nginx�б���Ϊ�ַ���)����<key,value>�е�key
} ngx_hash_elt_t;


//hash�ṹ
typedef struct {
    ngx_hash_elt_t  **buckets; //hashͰ(��size��Ͱ) 
    ngx_uint_t        size;    //hashͰ����
} ngx_hash_t;


typedef struct {
    ngx_hash_t        hash;
    void             *value;
} ngx_hash_wildcard_t;


typedef struct {
    ngx_str_t         key;      //key��Ϊnginx���ַ����ṹ 
    ngx_uint_t        key_hash; //�ɸ�key�������hashֵ(ͨ��hash������ngx_hash_key_lc())
    void             *value;    //��key��Ӧ��ֵ�����һ����-ֵ��<key,value>
} ngx_hash_key_t;


typedef ngx_uint_t (*ngx_hash_key_pt) (u_char *data, size_t len);

// ֧��ͨ�����ɢ�б�
typedef struct {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


//hash��ʼ���ṹ����������������ݷ�װ������Ϊ�������ݸ�ngx_hash_init()��ngx_hash_wildcard_init()����
typedef struct {
    ngx_hash_t       *hash;         //ָ�����ʼ����hash�ṹ 
    ngx_hash_key_pt   key;          //hash����ָ��

    ngx_uint_t        max_size;     //bucket��������
    ngx_uint_t        bucket_size;  //ÿ��bucket�Ŀռ�

    char             *name;         //��hash�ṹ������(���ڴ�����־��ʹ��)  
    ngx_pool_t       *pool;         //��hash�ṹ��poolָ����ڴ���з��� 
    ngx_pool_t       *temp_pool;    //������ʱ���ݿռ���ڴ��
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_KEY     1
#define NGX_HASH_READONLY_KEY     2


typedef struct {
    ngx_uint_t        hsize; //hsize Ϊnxg_array_t �е�nalloc 

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    // ��Ҫ����û�������ַ������� ��weibo.com
    ngx_array_t       keys;     // �ӳ�ʼ���п��Կ���,keys ��һ��ngx_hash_key_t �����顣
    ngx_array_t      *keys_hash; // �ӳ�ʼ���п��Կ�����keys_hash Ҳ��һ�����飬ֻ��ÿ�������е�Ԫ�ض���ngx_array���͵�.
                                 // keys �� keys_hash ֮�����ͨ��key ������һ��������Դ�ngx_hash_add_key �п��Կ���
                                 //  ��������Ҳ�����Ƶ���˼

    // ��Ҫ����ǰ���������ַ������ݡ��� .weibo.com *.weibo.com
    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;
    // ��Ҫ��������������ַ������ݡ��� weibo.com.*
    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_keys_arrays_t;


typedef struct {
    ngx_uint_t        hash;
    ngx_str_t         key;
    ngx_str_t         value;
    u_char           *lowcase_key;
} ngx_table_elt_t;


//hash����
void *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len);
void *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);

#define ngx_hash(key, c)   ((ngx_uint_t) key * 31 + c)
ngx_uint_t ngx_hash_key(u_char *data, size_t len);
//lc��ʾlower case�����ַ���ת��ΪСд���ټ���hashֵ
ngx_uint_t ngx_hash_key_lc(u_char *data, size_t len);  
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key,
    void *value, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
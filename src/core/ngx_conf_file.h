
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_CONF_FILE_H_INCLUDED_
#define _NGX_HTTP_CONF_FILE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 *        AAAA  number of agruments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#define NGX_CONF_NOARGS      0x00000001
#define NGX_CONF_TAKE1       0x00000002
#define NGX_CONF_TAKE2       0x00000004
#define NGX_CONF_TAKE3       0x00000008
#define NGX_CONF_TAKE4       0x00000010
#define NGX_CONF_TAKE5       0x00000020
#define NGX_CONF_TAKE6       0x00000040
#define NGX_CONF_TAKE7       0x00000080

#define NGX_CONF_MAX_ARGS    8

#define NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#define NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#define NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#define NGX_CONF_ARGS_NUMBER 0x000000ff
#define NGX_CONF_BLOCK       0x00000100
#define NGX_CONF_FLAG        0x00000200
#define NGX_CONF_ANY         0x00000400
#define NGX_CONF_1MORE       0x00000800 //����Ϊһ������
#define NGX_CONF_2MORE       0x00001000
#define NGX_CONF_MULTI       0x00002000

#define NGX_DIRECT_CONF      0x00010000

#define NGX_MAIN_CONF        0x01000000
#define NGX_ANY_CONF         0x0F000000



#define NGX_CONF_UNSET       -1
#define NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#define NGX_CONF_UNSET_PTR   (void *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          NULL
#define NGX_CONF_ERROR       (void *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


struct ngx_command_s {
    ngx_str_t             name;         //��������
    ngx_uint_t            type;         //��������(�м���������)
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf); //���ݴ洢����
    ngx_uint_t            conf;         //���ݴ�����ĸ�������
    ngx_uint_t            offset;       //���ݴ�����ĸ����õ��Ǹ�λ����
    void                 *post;         //һ��Ϊnull
};

#define ngx_null_command  { ngx_null_string, 0, NULL, 0, 0, NULL }  //������


struct ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    u_char               *buffer;
    u_char               *pos;
    u_char               *last;

#if 0
    /* e.g. append mode, error_log */
    ngx_uint_t            flags;
    /* e.g. reopen db file */
    ngx_uint_t          (*handler)(void *data, ngx_open_file_t *file);
    void                 *data;
#endif
};

//�ο���
//http://blog.csdn.net/livelylittlefish/article/details/6571497
#define NGX_MODULE_V1          0, 0, 0, 0, 0, 0, 1      //�ú�������ʼ��ǰ7���ֶ�  
#define NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0   //�ú�������ʼ�����8���ֶ�

struct ngx_module_s {
    ngx_uint_t            ctx_index;    //����ģ�������
    ngx_uint_t            index;        //ģ��id����ʶĳ��ģ����ȫ��ģ�������е��±�

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;
    ngx_uint_t            spare2;
    ngx_uint_t            spare3;

    ngx_uint_t            version;      //�汾  

    void                 *ctx;          //ģ�������ģ�ÿ��ģ���в�ͬģ��������
    ngx_command_t        *commands;     //ģ�����
    ngx_uint_t            type;         //ģ�����ͣ�HTTP, Core, Mail��Event�е�һ�֣�

    ngx_int_t           (*init_master)(ngx_log_t *log);         //��ʼ��master

    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);     //��ʼ��ģ��

    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);    //��ʼ������
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);     //��ʼ���߳�
    void                (*exit_thread)(ngx_cycle_t *cycle);     //�˳��߳�
    void                (*exit_process)(ngx_cycle_t *cycle);    //�˳�����

    void                (*exit_master)(ngx_cycle_t *cycle);     //�˳�master

    uintptr_t             spare_hook0;  //�����ֶΣ�����
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};

//�ο���
//http://blog.csdn.net/livelylittlefish/article/details/7247080
typedef struct {
    ngx_str_t             name;                                         //ģ��������ngx_core_module_ctx�ṹ������
    void               *(*create_conf)(ngx_cycle_t *cycle);             //�������õ�callback
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf);   //��ʼ�����õ�callback
} ngx_core_module_t;


typedef struct {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_uint_t            line;
} ngx_conf_file_t;


typedef char *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, void *conf);


struct ngx_conf_s {
    char                 *name;  //��ǰ��������ָ����
    ngx_array_t          *args;  //��ǰ������ָ��Ĳ���

    ngx_cycle_t          *cycle; //ָ��ϵͳ��������ϵͳ�������й����У�
                                 //��Ҫʹ�õ�һЩ��������Դ��Ҫͳһ�Ĺ���
    ngx_pool_t           *pool;  //�ڴ��
    ngx_pool_t           *temp_pool; //������ʱ���ݿռ���ڴ��
    ngx_conf_file_t      *conf_file; //��ǰ�����������ļ�����
    ngx_log_t            *log; //��־

    void                 *ctx;  //ģ���������Ϣ, �ᱻ��ֵΪngx_cycle �ṹ���е�conf_ctx ������ʱ����ʹ�á������Ϳ�������ĸ����ˣ�������Ӱ��conf_ctx
    ngx_uint_t            module_type; //��ǰָ�������
    ngx_uint_t            cmd_type; //��������� ,��commands �ṹ���е�type

    ngx_conf_handler_pt   handler; //ָ������������Լ���Ϊ��������ʵ��
    char                 *handler_conf; //ָ���������������Ϣ
};


typedef char *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    void *data, void *conf);

typedef struct {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


typedef struct {
    ngx_conf_post_handler_pt  post_handler;
    char                     *old_name;
    char                     *new_name;
} ngx_conf_deprecated_t;


typedef struct {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 low;
    ngx_int_t                 high;
} ngx_conf_num_bounds_t;


typedef struct {
    ngx_str_t                 name;
    ngx_uint_t                value;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

typedef struct {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



char * ngx_conf_deprecated(ngx_conf_t *cf, void *post, void *data);
char *ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data);


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]


/* ָ�����ڵĳ�ʼֵʵ��create_conf �����õ�
 * �������ļ������Ĺ����У�����������Ӧָ�����������Ӧ��set������ָ�ֵ
 * ��δ�������ļ��ж��壬����������ļ��������֮�󣬵���init_conf�����Դ�ģ���е�����ָ�ֵ��
 * ngx_conf_init_value ����˼Ϊ: ��ָ��ֻ��create_conf �г�ʼ��������δ������Ӧ��set �������Լ���ֵ
 * �� ��init_conf �е���ngx_conf_init_value �� ָ�Ĭ��ֵ
 */
#define ngx_conf_init_value(conf, default)                                   \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = default;                                                      \
    }

#define ngx_conf_init_ptr_value(conf, default)                               \
    if (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = default;                                                      \
    }

#define ngx_conf_init_uint_value(conf, default)                              \
    if (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = default;                                                      \
    }

#define ngx_conf_init_size_value(conf, default)                              \
    if (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = default;                                                      \
    }

#define ngx_conf_init_msec_value(conf, default)                              \
    if (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = default;                                                      \
    }

#define ngx_conf_merge_value(conf, prev, default)                            \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? default : prev;                    \
    }

#define ngx_conf_merge_ptr_value(conf, prev, default)                        \
    if (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? default : prev;                \
    }

#define ngx_conf_merge_uint_value(conf, prev, default)                       \
    if (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? default : prev;               \
    }

#define ngx_conf_merge_msec_value(conf, prev, default)                       \
    if (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? default : prev;               \
    }

#define ngx_conf_merge_sec_value(conf, prev, default)                        \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? default : prev;                    \
    }

#define ngx_conf_merge_size_value(conf, prev, default)                       \
    if (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? default : prev;               \
    }

#define ngx_conf_merge_off_value(conf, prev, default)                        \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? default : prev;                    \
    }

#define ngx_conf_merge_str_value(conf, prev, default)                        \
    if (conf.data == NULL) {                                                 \
        if (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } else {                                                             \
            conf.len = sizeof(default) - 1;                                  \
            conf.data = (u_char *) default;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_value(conf, prev, default_num, default_size)     \
    if (conf.num == 0) {                                                     \
        if (prev.num) {                                                      \
            conf.num = prev.num;                                             \
            conf.size = prev.size;                                           \
        } else {                                                             \
            conf.num = default_num;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_value(conf, prev, default)                    \
    if (conf == 0) {                                                         \
        conf = (prev == 0) ? default : prev;                                 \
    }


char *ngx_conf_param(ngx_conf_t *cf);
char *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
void ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, const char *fmt, ...);


char *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
char *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


extern ngx_uint_t     ngx_max_module;
extern ngx_module_t  *ngx_modules[];


#endif /* _NGX_HTTP_CONF_FILE_H_INCLUDED_ */
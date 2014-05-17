
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ngx_uint_t  ngx_pagesize;
ngx_uint_t  ngx_pagesize_shift;
ngx_uint_t  ngx_cacheline_size;

/*
* #include <stdlib.h>
* void * malloc(size_t size)
* ʹ��malloc�����ڴ�
*/
void *
ngx_alloc(size_t size, ngx_log_t *log)
{
    void  *p;

    p = malloc(size);
    if (p == NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      "malloc(%uz) failed", size);
    }

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, log, 0, "malloc: %p:%uz", p, size);

    return p;
}

// ʹ��malloc �����ڴ棬������ngx_memzero ��������ڴ��ʼ��Ϊ��
// ngx_memzero �ο� ngx_string.h �ļ�  (#define ngx_memzero(buf, n)       (void) memset(buf, 0, n))
void *
ngx_calloc(size_t size, ngx_log_t *log)
{
    void  *p;

    p = ngx_alloc(size, log);

    if (p) {
        ngx_memzero(p, size);
    }

    return p;
}

// Linux ƽ̨�ϵ��ã�POSIX��׼
// NGX_HAVE_POSIX_MEMALIGN,������configure�ļ��У�
// if [ "$NGX_PLATFORM" != win32 ]; then
//    . auto/unix
// fi
// �� auto/unix ������NGX_HAVE_POSIX_MEMALIGN��NGX_HAVE_MEMALIGN��

#if (NGX_HAVE_POSIX_MEMALIGN)

// char *buf;
// int ret;
// ret = posix_memalign(&buf,256,1024);
// if (ret){
//     fprintf (stderr, "posix_memalign: %s\n",strerror (ret));
//      return -1;
// }
// free(buf);

void *
ngx_memalign(size_t alignment, size_t size, ngx_log_t *log)
{
    void  *p;
    int    err;

    err = posix_memalign(&p, alignment, size);

    if (err) {
        ngx_log_error(NGX_LOG_EMERG, log, err,
                      "posix_memalign(%uz, %uz) failed", alignment, size);
        p = NULL;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_ALLOC, log, 0,
                   "posix_memalign: %p:%uz @%uz", p, size, alignment);

    return p;
}

// Windows ƽ̨�ϵ���
// memalign �ο� http://gs5689.blogbus.com/logs/36655475.html
#elif (NGX_HAVE_MEMALIGN)

void *
ngx_memalign(size_t alignment, size_t size, ngx_log_t *log)
{
    void  *p;

    p = memalign(alignment, size);
    if (p == NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      "memalign(%uz, %uz) failed", alignment, size);
    }

    ngx_log_debug3(NGX_LOG_DEBUG_ALLOC, log, 0,
                   "memalign: %p:%uz @%uz", p, size, alignment);

    return p;
}

#endif

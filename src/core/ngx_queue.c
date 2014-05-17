
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */
// �ο���http://blog.csdn.net/livelylittlefish/article/details/6607324
// ��ö��е��м�ڵ㣬(��ͷ�ڵ���)���������������ڵ㣬�򷵻��м�ڵ㣻
// ��������ż�����ڵ㣬�򷵻غ������еĵ�һ���ڵ�;
ngx_queue_t *
ngx_queue_middle(ngx_queue_t *queue)
{
    ngx_queue_t  *middle, *next;

    middle = ngx_queue_head(queue);

    //��ֻ��һ��Ԫ��
    if (middle == ngx_queue_last(queue)) {
        return middle;
    }

    next = ngx_queue_head(queue);

    // ��ѭ���У�nextָ��ÿ���ƶ������ڵ㣬��middleָ��ÿ���ƶ�һ���ڵ�
    for ( ;; ) {
        middle = ngx_queue_next(middle);

        next = ngx_queue_next(next);

        // ż�����ڵ㣬�ڴ˷��غ������еĵ�һ���ڵ�
        if (next == ngx_queue_last(queue)) {
            return middle;
        }

        next = ngx_queue_next(next);

        // �������ڵ㣬�ڴ˷����м�ڵ�
        if (next == ngx_queue_last(queue)) {
            return middle;
        }
    }
}


/* the stable insertion sort */
// cmp ָ������ָ��
// ����������õ����ȶ��ļ򵥲������򷽷������ӵ�һ���ڵ㿪ʼ���������ν��ڵ�(q)����ǰ���Ѿ�����õĶ���(����)��
// prev Ϊ�Ѿ�����õ�queue
void
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *))
{
    ngx_queue_t  *q, *prev, *next;

    q = ngx_queue_head(queue);
    
    // ��ֻ��һ��Ԫ�أ�����������
    if (q == ngx_queue_last(queue)) {
        return;
    }

    for (q = ngx_queue_next(q); q != ngx_queue_sentinel(queue); q = next) {

        prev = ngx_queue_prev(q);
        // Ҳ�䵱��ѭ������
        next = ngx_queue_next(q);

        ngx_queue_remove(q);

        do {
            // ָ�뺯������
            if (cmp(prev, q) <= 0) {
                break;
            }

            prev = ngx_queue_prev(prev);

        } while (prev != ngx_queue_sentinel(queue));

        ngx_queue_insert_after(prev, q);
    }
}

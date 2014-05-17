
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
// 参考：http://blog.csdn.net/livelylittlefish/article/details/6607324
// 获得队列的中间节点，(除头节点外)若队列有奇数个节点，则返回中间节点；
// 若队列有偶数个节点，则返回后半个队列的第一个节点;
ngx_queue_t *
ngx_queue_middle(ngx_queue_t *queue)
{
    ngx_queue_t  *middle, *next;

    middle = ngx_queue_head(queue);

    //若只有一个元素
    if (middle == ngx_queue_last(queue)) {
        return middle;
    }

    next = ngx_queue_head(queue);

    // 此循环中，next指针每次移动两个节点，而middle指针每次移动一个节点
    for ( ;; ) {
        middle = ngx_queue_next(middle);

        next = ngx_queue_next(next);

        // 偶数个节点，在此返回后半个队列的第一个节点
        if (next == ngx_queue_last(queue)) {
            return middle;
        }

        next = ngx_queue_next(next);

        // 奇数个节点，在此返回中间节点
        if (next == ngx_queue_last(queue)) {
            return middle;
        }
    }
}


/* the stable insertion sort */
// cmp 指向函数的指针
// 队列排序采用的是稳定的简单插入排序方法，即从第一个节点开始遍历，依次将节点(q)插入前面已经排序好的队列(链表)中
// prev 为已经排序好的queue
void
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *))
{
    ngx_queue_t  *q, *prev, *next;

    q = ngx_queue_head(queue);
    
    // 若只有一个元素，则无须排序
    if (q == ngx_queue_last(queue)) {
        return;
    }

    for (q = ngx_queue_next(q); q != ngx_queue_sentinel(queue); q = next) {

        prev = ngx_queue_prev(q);
        // 也充当了循环条件
        next = ngx_queue_next(q);

        ngx_queue_remove(q);

        do {
            // 指针函数调用
            if (cmp(prev, q) <= 0) {
                break;
            }

            prev = ngx_queue_prev(prev);

        } while (prev != ngx_queue_sentinel(queue));

        ngx_queue_insert_after(prev, q);
    }
}

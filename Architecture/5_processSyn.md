## **进程内的同步，互斥，通信**

### 原子操作
- **原子操作是由CPU实现的.**

### 互斥锁
- **A. 做好发生异常的解锁处理.**
- **B. 不要锁住耗时的代码块，比如：网络IO操作.**
- **C. 控制好锁的颗粒度**

### 读写锁
- **A. 识别读多写少的环节**
- **B. 读操作不阻止读操作，阻止写操作**
- **C. 写操作阻止一切，不管读操作还是写操作。**

### 条件变量 [执行体的同步]
- **代码例子**
```C
#include <pthread.h>
struct msg {
    struct msg *m_next;
    /* ... more stuff here..*/
}
struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
void process_msg(void)
{
    struct msg* mp;
    for(;;)
    {
        pthread_mutex_lock(&qlock);
        while (workq == NULL)
        /*
            【我们把while换成if可不可以呢？】
            if (workq == NULL)
                pthread_cond_wait(&qready, &qlock);

            答案是: 不可以.
            一个生产者可能对应着多个消费者，生产者向队列中插入一条数据之后发出signal，然后各个消费者线程的pthread_cond_wait获取mutex后返回当然
            这里只有一个线程获取到了mutex，然后进行处理，其它线程会pending在这里，处理线程处理完毕之后释放mutex，刚才等待的线程中有一个获取mutex，
            如果这里用if，就会在当前队列为空的状态下继续往下处理，这显然是不合理的。
        */
            pthread_cond_wait(&qready, &qlock);
            /*
                【为什么pthread_cond_wait需要加锁？】
                【pthread_cond_wait(xxx, yyy); 做了两件事】
                A. 释放 qlock.
                B. 将当前线程挂到等待队列.
                
                【AB两个操作必须是原子的,否则造成以下问题】
                A. signal信号丢失 --> 先进行A，其他线程执行pthread_cond_signal(), 再执行B.
                B. 死锁 --> 先进行B，其他线程执行pthread_cond_signal(), 当前线程立即获取mutex, 已经死锁没法再执行A.
            */

        mp = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
    }
}

void enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
    /*
        【signal到底是放在unlock之前还是之后？？】
        A. signal 在unlock之后：其他线程可能在signal之前获取到lock, 导致wait中的线程出现虚假唤醒
        B. signal 在unlock之前：
            1. 如果把signal放在unlock之前，消费者线程会被唤醒，获取mutex发现获取不到，就又去sleep了。浪费了资源
            2. 在LinuxThreads或者NPTL里面，就不会有这个问题，因为在Linux 线程中，有两个队列，分别是cond_wait队列和mutex_lock队列，
                 cond_signal只是让线程从cond_wait队列移到mutex_lock队列，而不用返回到用户空间，不会有性能的损耗。
    */
}
``` 






### 使用pthread实现的多线程排序
 
代码修改自：C语言实现多线程排序

https://github.com/tantao0675/multi_threads_sort



#### Linux编程- pthread_barrier_xxx介绍

https://blog.csdn.net/jackailson/article/details/51052609

pthread_barrier_xxx系列函数在<pthread.h>中定义，用于多线程的同步，它包含下列三个函数：

      —pthread_barrier_init();

      —pthread_barrier_wait();

      —pthread_barrier_destroy();


pthread_barrier_xxx其实只做且只能做一件事，就是充当栏杆（barrier意为栏杆）形象的说就是把先后到达的多个线程挡在同一栏杆前，直到所有线程到齐，然后撤下栏杆同时放行。


      1)、init函数负责指定要等待的线程个数；

      2)、wait()函数由每个线程主动调用，它告诉栏杆“我到起跑线前了”。wait()执行末尾栏杆会检查是否所有人都到栏杆前了，如果是，栏杆就消失所有线程继续执行下一句代码；如果不是，则所有已到wait()的线程停在该函数不动，剩下没执行到wait()的线程继续执行；

      3)、destroy函数释放init申请的资源。


这种“栏杆”机制最大的特点就是最后一个执行wait的动作最为重要，就像赛跑时的起跑枪一样，它来之前所有人都必须等着。

我们可以在init时指定n+1个等待，其中n是线程数。而在每个线程执行函数的首部调用wait()。这样100个pthread_create()结束后所有线程都停下来等待最后一个wait()函数被调用。这个wait()由主进程在它觉得合适的时候调用就好。最后这个wait()就是鸣响的起跑枪。
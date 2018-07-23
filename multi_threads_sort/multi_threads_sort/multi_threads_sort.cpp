#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define NTHR   8             /* number of threads */
#define NUMNUM 8000000L      /* number of numbers to sort */
#define TNUM   (NUMNUM/NTHR) /* number to sort per thread */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t g_pthread_barrier;

#ifdef SOLARIS
#define heapsort qsort
#else
// ∂—≈≈–Ú
extern int heapsort(void*, size_t, size_t, int (*)(const void*, const void*));
#endif

/*
 * Compare two long integers (helper function for heapsort)
 */
int
complong(const void* arg1, const void* arg2)
{
	long l1 = *(long*)arg1;
	long l2 = *(long*)arg2;

	if(l1==l2)
		return 0;
	else if(l1<l2)
		return -1;
	else
		return 1;
}

/*
 * worker thread to sort a portion of the set of numbers
 */
void*
thr_fn(void* arg)
{
	long idx = (long)arg;

	heapsort(&nums[idx], TNUM, sizeof(long), complong);
	pthread_barrier_wait(&g_pthread_barrier);

	/*
	 * Go off and perform more work ...
	 */
	return (void*)0;
}

/*
 * Merge the results of the individual sorted ranges
 */
void
merge()
{
	long idx[NTHR];
	for(int i=0; i<NTHR; i++)
		idx[i] =i*TNUM;


	for(int sidx=0; sidx<NUMNUM; sidx++)
	{
		long minidx;
		long num = LONG_MAX;
		for(int i=0; i<NTHR; i++)
		{
			if((idx[i]<(i+1)*TNUM)&&nums[idx[i]]<num)
			{
				num = nums[idx[i]];
				minidx = i;
			}
		}
		snums[sidx] = nums[idx[minidx]];
		idx[minidx]++;
	}
}

int 
main(int argc, char** argv)
{
	size_t       startusec, endusec;
	size_t          elapsed;
	
	pthread_t       tid;

	/*
	 * Create the initial set of numbers to sort.
	 */
	srand((int)time(0));
	for(unsigned long i=0; i<NUMNUM; i++)
		nums[i] = rand();

	/*
	 * Create 8 threads to sort the numbers.
	 */
	startusec = (size_t)time(NULL);
	pthread_barrier_init(&g_pthread_barrier, NULL, NTHR+1);
	int ret;
	for(int i=0; i<NTHR; i++)
	{
		ret = pthread_create(&tid, NULL, thr_fn, (void*)(i*TNUM));
		if(ret!=0)
		{
			perror("can't create thread");
			exit(1);
		}
	}
	
	pthread_barrier_wait(&g_pthread_barrier);
	merge();
	endusec = (size_t)time(NULL);
	elapsed = endusec - startusec;
	pthread_barrier_destroy(&g_pthread_barrier);
	/*
	 * Print the sorted list.
	 */
	printf("sort took %d microseconds\n", elapsed);
	//for(unsigned int i=0; i<NUMNUM; i++)
	//	printf("%ld\n", snums[i]);

	exit(0);
}
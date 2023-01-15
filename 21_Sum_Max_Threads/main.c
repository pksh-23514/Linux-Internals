/*
Name         : Prabhat Kiran
Date         : 5th January 2022
Description  : WAP to Sum and Maximum of a given array using multiple Threads. Synchronize threads using Mutex.
1. Create an array [N] (min size 500) with 1 to N values and two Global variables →  global_sum & global_max.
2. Create multiple Threads to find Sum of array and Maximum value in Array.
3. Threads will share Array equally among them. Means each Thread will access different part of Array at same time.
4. Create at-least 5 threads. Create and join threads using separate loops.
5. Each Thread will update global_sum and global_max.
6. Since we are accessing Global variable from Threads, do proper Synchronization using Mutex.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS	5

/* Global Variables */
int global_sum, global_max;
/* Structure Declaration */
struct series
{
	int* arr;
	int first, last;
};
/* Mutex Declaration */
pthread_mutex_t mut;

/* Thread Function */
void* sum_max (void* arg)
{
	struct series* num = (struct series*) arg;
	int sum = 0;

	for (int i = num->first; i < (num->last); i++)		//To calculate the Sum and Maximum value for a particular Thread.
	{
		sum += num->arr [i];
		if (global_max < num->arr [i])
			global_max = num->arr [i];
	}

	/* Shared Resource access locked using Mutex */
	pthread_mutex_lock (&mut);
	global_sum += sum;
	pthread_mutex_unlock (&mut);
	
	return NULL;
}

int main ()
{
	int n;
	printf ("Enter the value of N (greater than or equal to 500): ");
	scanf ("%d", &n);

	if (n < 500)	//Error Handling.
	{
		printf ("ERROR: The value of N is Invalid.\n");
		return 0;
	}

	/* Mutex Initialization and Error Handling */
	if ((pthread_mutex_init (&mut, NULL)) != 0)
	{
		printf ("ERROR: Mutex Lock Failed.\n");
		return 0;
	}

	int arr [n];
	for (int i = 1; i <= n; i++)	//Array Creation from 1 to N.
	{
		arr [i-1] = i;
	}
#if 0
	printf ("Array Elements:\n");
	for (int i = 0; i < n; i++)
		printf ("%d ", num.arr [i]);
	printf ("\n");
#endif

	struct series num [5];
	pthread_t tid [THREADS];
	/* Divide the Array into n-parts and create the Ranges of the Array to be used for calculation */
	int diff = n / THREADS;

	for (int i = 0; i < THREADS; i++)
	{
		num [i].arr = arr;
		num [i].first = (i == 0) ? 0 : num [i - 1].last;
		num [i].last = (i == THREADS - 1) ? n : ((i + 1) * diff);

		/* Create the ith Thread by passing the Function and Structure Argument and receive the Thread ID */
		pthread_create (&tid [i], NULL, sum_max, (void*) &num [i]);
	}
	for (int i = 0; i < THREADS; i++)
	{
		/* Wait for the ith Thread to Terminate */
		pthread_join(tid [i], NULL);
	}

	printf ("The Sum of the Array Elements are: %d\n", global_sum);
	printf ("The Maximum of the Array Elements is: %d\n", global_max);

	/* Mutex Destruction */
	pthread_mutex_destroy (&mut);

	return 0;
}

/*
Name         : Prabhat Kiran
Date         : 4th January 2023
Description  : WAP to Print the Factorial of a given Number using Multi-Threads.
1. Modify the Factorial template code using multiple Threads.
2. Create at-least 3 Threads and Share the Task among Threads equally.
3. Create and Join Threads using separate loops.
4. You may have to change the Argument Structure.
5. Declare all Integer variables as "unsigned long int".
*/

#include <stdio.h>
#include <pthread.h>

/* The Number of Threads performing the Process */
#define THREADS 3

/* The Argument defintion passed to the Thread Function */
struct fact_limit
{
	int first;
	int last;
};
typedef struct fact_limit limits;

/* Thread Function */
void* factorial (void* arg)
{
	limits* ptr = (limits*) arg;
	unsigned long fact = 1;

	for (int i = ptr->first; i <= ptr->last; i++)	//To Calculate the Factorial for the given Range.
	{
		fact = fact * i;
	}

	return ((void*) fact);	//Return the Factorial using Hackway method.
}

int main ()
{
	int num;
	printf ("Enter the Number for calculating the Factorial: ");
	scanf ("%d", &num);

	pthread_t tid [THREADS];	//To store all the Thread IDs.
	limits arg [THREADS];		//To store the Range to be passed to each of the Thread Functions.
	unsigned long int result = 1, ret;

	/* Divide the Number into n-parts and create the Ranges of the Number to be used for calculation */
	int diff = num / THREADS;
	for (int i = 0; i < THREADS; i++)
	{
		/* If it is the First Range, Lower limit shall be 1. Otherwise, Lower limit shall be the (Previous Upper Range + 1) */
		arg [i].first = (i == 0) ? 1 : (arg [i - 1].last + 1);
		
		/* If it is the Last Range, Upper limit shall be 'num'. Otherwise, Upper limit shall be [(i+1) * (num / n-parts)] */
		arg [i].last = (i == (THREADS - 1)) ? num : ((i + 1) * diff);
	}

	for (int i = 0; i < THREADS; i++)
	{
		/* Create the ith Thread by passing the Function and Arguments and receive the Thread ID */
		pthread_create (&tid [i], NULL, factorial, (void*) &arg [i]);
		
		/* Wait for the ith Thread to Terminate and receive the Value returned by the Thread Function */
		pthread_join (tid [i], (void**) &ret);	//The address of 'ret' is type-casted to 'void**' to match the Prototype.

		result *= ret;	//To store the Product of all the Factorials from each Threads.
	}

	printf ("Factorial of %d is %lu\n", num, result);

	return 0;
}

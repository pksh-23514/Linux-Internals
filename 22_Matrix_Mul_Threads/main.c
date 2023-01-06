/*
Name         : Prabhat Kiran
Date         : 6th January 2023
Description  : WAP to multiply Two Matrices using multiple Threads.
1. Create Three local Matrices, M1 (MxN) M2 (NxP) and Result (MxP) (M1 columns = M2 rows) where M, N & P values are provided by User.
2. In case (M1 columns != M2 rows) print Error message to User.
3. Create all Matrices using Dynamic allocation.
4. Use Structure to pass arguments to threads.
5. Each Thread will calculate and store Single Row in Result. So, number of Threads shall be equal to the number of Rows in M1.
6. Don't create any global variables.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

/* Matrix Structure Definition */
typedef struct thread_data
{
	short m1_row;
	short m1_col;
	short m2_row;
	short m2_col;
	int **matrix1;
	int **matrix2;
	int **result;
	short cur_row;
} matrix;

/* Thread Function */
void* matrix_mul (void* arg)
{
	matrix* m = (matrix*) arg;
	int cur_row = (m->cur_row)++;	//To store the Current Row of the Result matrix whose values are to be stored.

	/* Multiplication of the 2 Matrices */
	for (short j = 0; j < m->m2_col; j++)
	{
		for (short k = 0; k < m->m1_col; k++)
		{
			m->result [cur_row][j] += m->matrix1 [cur_row][k] * m->matrix2 [k][j];
		}
	}

	return NULL;
}

int main ()
{
	matrix m;	//Structure declaration.

	/* Read the Variables of the Matrices */
	printf ("Enter number of Rows in 1st Matrix : ");
	scanf ("%hd", &(m.m1_row));
	printf ("Enter number of Columns in 1st Matrix : ");
	scanf ("%hd", &(m.m1_col));
	printf ("Enter number of Rows in 2nd Matrix : ");
	scanf ("%hd", &(m.m2_row));
	printf ("Enter number of Columns in 2nd Matrix : ");
	scanf ("%hd", &(m.m2_col));

	if (m.m1_col != m.m2_row)	//If the Matrix Multiplication Rule is not satisfied, an Error message is printed.
	{
		printf ("Matrix multiplication is not possible.\n");
		return 0;
	}

	/* Allocating the Memory to the Matrices dynamically */
	m.matrix1 = (int**) malloc (m.m1_row * sizeof (int*));
	for (short i = 0; i < m.m1_row; i++)
	{
		m.matrix1 [i] = (int*) malloc (m.m1_col * sizeof (int));
	}
	m.matrix2 = (int**) malloc (m.m2_row * sizeof (int*));
	for (short i = 0; i < m.m2_row; i++)
	{
		m.matrix2 [i] = (int*) malloc (m.m2_col * sizeof (int));
	}

	/* Reading the Values into the Matrices */
	printf ("Enter the Elements in the 1st Matrix:\n");
	for (short i = 0; i < m.m1_row; i++)
	{
		for (short j = 0; j < m.m1_col; j++)
		{
			scanf ("%d", &m.matrix1 [i][j]);
		}
	}
	printf ("Enter the Elements in the 2nd Matrix:\n");
	for (short i = 0; i < m.m2_row; i++)
	{
		for (short j = 0; j < m.m2_col; j++)
		{
			scanf ("%d", &m.matrix2 [i][j]);
		}
	}

	/* Decalre the Resultant Matrix with dimensions of ROW1 and COL2 */
	m.result = (int**) malloc (m.m1_row * sizeof (int*));
	for (short i = 0; i < m.m1_row; i++)
	{
		m.result [i] = (int*) malloc (m.m2_col * sizeof (int));
	}

	pthread_t tid [m.m1_row];	//Thread declaration.
	m.cur_row = 0;

	for (short i = 0; i < m.m1_row; i++)
	{
		/* To create the Thread and send the structure as argument */
		pthread_create (&tid [i], NULL, matrix_mul, (void*) &m);
	}
	for (short i = 0; i < m.m1_row; i++)
	{
		/* To wait for all the Threads to terminate */
		pthread_join (tid [i], NULL);
	}

	/* Display the Resultant Matrix */
	for (short i = 0; i < m.m1_row; i++)
	{
		for (short j = 0; j < m.m2_col; j++)
		{
			printf ("%d ", m.result [i][j]);
		}
		printf ("\n");
	}

	return 0;
}

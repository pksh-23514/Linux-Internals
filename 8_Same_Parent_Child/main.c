/*
Name         : Prabhat Kiran
Date         : 20th December 2022
Description  : WAP to create Three Child from the same Parent.
1. Create Three Child Process from Same Parent.
2. Parent has to wait for all Three Child Process.
3. Print Exit status of each Child when they terminates.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main ()
{
	pid_t pid_c1, pid_c2, pid_c3, pid_c;
	int w_status;
	
	pid_c1 = fork ();												//To create the First Child Process.
	if (pid_c1 > 0)													//If the 'pid_c1' is greater than 0; it is the Parent Process.
	{
		printf ("Child 1 with the PID %d created.\n", pid_c1);
		pid_c2 = fork ();											//To create the Second Child Process.
		if (pid_c2 > 0)												//If the 'pid_c2' is greater than 0; it is the Parent Process.
		{
			printf ("Child 2 with the PID %d created.\n", pid_c2);
			pid_c3 = fork ();										//To create the Third Child Process.
			if (pid_c3 > 0)											//If the 'pid_c3' is greater than 0; it is the Parent Process.
			{
				printf ("Child 3 with the PID %d created.\n", pid_c3);

				for (int i = 1; i <= 3; i++)						//The loop shall run for the Parent to Wait for all the Child Processes to terminate.
				{
					pid_c = wait (&w_status);						//To wait for the Child to terminate.

					if (WIFEXITED(w_status))						//To check if the Child has terminated Normally.
					{
						/* If the Child has terminated normally, get the Exit status of the Child Process */
						printf ("Child %d is terminated with Code %d.\n", pid_c, WEXITSTATUS(w_status));
					}
				}
			}
			else if (pid_c3 == -1)									//Error Handling.
				printf ("Child 3 is not created.\n");
		}
		else if (pid_c2 == -1)										//Error Handling.
			printf ("Child 2 is not created.\n");
	}
	else if (pid_c1 == -1)											//Error Handling.
		printf ("Child 1 is not created.\n");

	return 0;
}

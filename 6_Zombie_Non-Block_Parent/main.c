/*
Name         : Prabhat Kiran
Date         : 22nd December 2022
Description  : WAP to avoid Child becoming Zombie without Blocking Parent.
1. Create a Child Process and avoid it to become a Zombie.
2. To avoid Zombie we need to call wait() but this blocks the Parent until Child terminates.
3. So we need to use waitpid() with proper Arguments.
4. When Child is working Parent has to continuously print some messages.
5. When ever Child terminates, Parent has to print Child terminated and print Exit status of Child Process.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main ()
{
	pid_t pid = fork ();										//To create the Child Process.
	int status;													//To store the Child's Exit status.

	if (pid > 0)												//If the 'pid' is greater than 0; it is the Parent Process.
	{
		printf ("A Child is created with the PID %d.\n", pid);	//To display the Child's PID.

		while ((waitpid (pid, &status, WNOHANG)) == 0)			//To wait for the Child Process to terminate without blocking the Parent Process.
		{
			sleep (1);
			printf ("Parent running...\n");						//To display the message continuously while waiting for the Child to terminate.
		}

		printf ("Child %d terminated normally with Exit status %d. Parent Terminating.\n", pid, status);
	}
	else if (pid == 0)											//If the 'pid' is 0; it is the Child Process.
	{
		sleep (5);
	}
	else														//Error Handling.
	{
		printf ("The Child is not created.\n");
	}

	return 0;
}

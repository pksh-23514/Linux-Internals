/*
Name         : Prabhat Kiran
Date         :
Description  :
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
	pid_t pid = fork ();
	int status;

	if (pid > 0)
	{
		printf ("A Child is created with the PID %d.\n", pid);

		while ((waitpid (pid, &status, WNOHANG)) == 0)
		{
			sleep (1);
			printf ("Parent running...\n");
		}

		printf ("Child %d terminated normally with Exit status %d. Parent Terminating.\n", pid, status);
	}
	else if (pid == 0)
	{
		sleep (5);
	}
	else
	{
		printf ("The Child is not created.\n");
	}

	return 0;
}

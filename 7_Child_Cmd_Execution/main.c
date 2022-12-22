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

int main (int argc, char* argv [])
{
	if (argc == 1)
	{
		printf ("No Arguments passed.\nUsage: ./exe_child <command> [options]\n");
		return 0;
	}
	else
	{
		pid_t pid = fork ();
		int status;

		if (pid > 0)
		{
			printf ("Child Process created with PID %d.\n", pid);
			wait (&status);
			printf ("Child terminated with Exit status %d.\n", status);
		}
		else if (pid == 0)
		{
			execvp (argv [1], (argv + 1));
		}
		else
		{
			printf ("The Child Process is not created.\n");
		}
	}

	return 0;
}

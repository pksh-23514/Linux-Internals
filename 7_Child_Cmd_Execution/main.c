/*
Name         : Prabhat Kiran
Date         : 22nd December 2022
Description  : WAP to create a Child Process to execute Command passed through Command Line.
1. Create a Child and execute a Command passed from Command Line Arguments.
2. If no Arguments are passed, print an usage message.
3. In case any wrong Command is passed, print an Error message.
4. After Child terminates, print the Exit status of Child Process.
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
	if (argc == 1)											//If no Commands are passed as CLA; display the Error message.
	{
		printf ("No Arguments passed.\nUsage: ./exe_child <command> [options]\n");
		return 0;
	}
	else													//Otherwise, proceed for the Command execution.
	{
		pid_t pid = fork ();								//To create the Child Process.
		int status;											//To store the Child's Exit status.

		if (pid > 0)										//If the 'pid' is greater than 0; it is the Parent Process.
		{
			printf ("Child Process created with PID %d.\n", pid);

			wait (&status);									//To wait for the Child Process to terminate.
			printf ("Child terminated with Exit status %d.\n", status);
		}
		else if (pid == 0)									//If the 'pid' is 0; it is the Child Process.
		{
			int ret = execvp (argv [1], (argv + 1));		//To execute the Command passed through the CLA in the Child Process.
			if (ret == -1)									//Error Handling.
			{
				perror ("execvp");
			}
		}
		else												//Error Handling.
		{
			printf ("The Child Process is not created.\n");
		}
	}

	return 0;
}

/*
Name         : Prabhat Kiran
Date         : 27th December 2022
Description  : WAP to avoid a Child Process from becoming a Zombie by using Signal Handlers.
1. Create a Child Process such that it terminates before the Parent Process.
2. Use 'sa_flags' from 'struct sigaction' to avoid the Child from becoming a Zombie.
3. Print the Child Exit status inside Handler.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* Signal Handler Function */
void zombie_handler (int sig, siginfo_t* info, void* ucontext)
{
	printf ("Child Terminated with exit status %d.\n", info->si_status);
}

int main ()
{
	struct sigaction act;						//To declare the 'sigaction' Structure to perform Signal Handling.
	memset (&act, 0, sizeof (act));				//To initialize the 'sigaction' Structure members to 0.

	/* To set the 'sa_flags' for using the 'sa_sigaction' as the Handler and prevent the Child Process from becoming Zombie */
	act.sa_flags = SA_SIGINFO | SA_NOCLDWAIT;
	act.sa_sigaction = zombie_handler;			//Update the Signal Handler to 'zombie_handler' from the default function.

	sigaction (SIGCHLD, &act, NULL);			//To change the Action taken if Segmentation Fault is detected.

	int pid = fork ();		//To create a Child Process.
	if (pid > 0)			//If the 'pid' is greater than 0; it is the Parent Process.
	{
		sleep (5);
	}
	else if (pid == 0)		//If the 'pid' is 0; it is the Child Process.
	{
		sleep (2);
	}
	else					//Error Handling.
	{
		printf ("The Child Process is not created.\n");
	}

	return 0;
}

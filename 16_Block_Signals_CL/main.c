/*
Name         : Prabhat Kiran
Date         : 5th January 2023
Description  : WAP to Block certain Signals from being received from Command Line.
1. Write a Signal Handler function for any Signal, say SIGINT.
2. While its running inside Handler (use loop), Block other signals (atleast 3) being received.
3. Use sa_mask from struct sigaction to set signals to be blocked.
4. To create a Signal set use variable of sigset_t.
5. To add or remove Signals from set using the functions.
6. Process will receive Blocked Signals once its out from Handler.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* Signal Handler for SIGINT */
void sigint_handler (int sig)
{
	printf ("SIGINT received\n");
	for (int i = 0; i < 10; i++)
	{
		printf ("Inside the Handler...\n");
		sleep (1);
	}
}

int main ()
{
	printf ("Process %d waiting for a signal...\n", getpid ());

	struct sigaction act;
	memset (&act, 0, sizeof (act));

	act.sa_handler = sigint_handler;		//Update the Signal Handler to 'signal_handler' from the default function.

	sigemptyset (&(act.sa_mask));			//To empty the Signal set.
	/* To include the below Signals to the Mask */
	sigaddset (&(act.sa_mask), SIGINT);		//To block the same Signal from disrupting the Handler execution.
	sigaddset (&(act.sa_mask), SIGUSR1);
	sigaddset (&(act.sa_mask), SIGUSR2);
	sigaddset (&(act.sa_mask), SIGABRT);

	sigaction (SIGINT, &act, NULL);			//To change the Action taken if SIGINT is detected.

	while (1);

	return 0;
}

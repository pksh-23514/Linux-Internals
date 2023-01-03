/*
Name         : Prabhat Kiran
Date         : 27th December 2022
Description  : WAP to print the Address which is causing Segmentation Fault.
1. Write a Signal Handler Function to print the Address which caused Segmentation Fault (SIGSEGV).
2. Use 'sigaction' System Call to register the Signal.
3. Use 'struct siginfo' from 'sa_sigaction' to print the Address.
4. Create a Segmentation Fault from the Code.
5. When an Error occurs, Program should print the Address and Exit.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

/* Signal Handler Function */
void seg_fault_handler (int sig, siginfo_t* info, void* ucontext)
{
	printf ("Segmentation Fault...\n");
	printf ("Address %p caused the error.\n", info->si_addr);

	exit (0);	//To Exit the Process as soon as the Segmentation Fault is detected.

}

int main ()
{
	struct sigaction act;					//To declare the 'sigaction' Structure to perform Signal Handling.
	memset (&act, 0, sizeof (act));			//To initialize the 'sigaction' Structure members to 0.

	act.sa_flags = SA_SIGINFO;				//To use the 'sa_sigaction' defined Signal Handler, the 'sa_flags' shall be set with 'SA_SIGINFO'.
	act.sa_sigaction = seg_fault_handler;	//Update the Signal Handler to 'seg_fault_handler' from the default function.

	sigaction (SIGSEGV, &act, NULL);		//To change the Action taken if Segmentation Fault is detected.

	char* str = "Prabhat";
	str [0] = 'p';

	return 0;
}

/*
Name         : Prabhat Kiran
Date         :
Description  :
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
	
	pid_c1 = fork ();
	if (pid_c1 > 0)
	{
		printf ("Child 1 with the PID %d created.\n", pid_c1);
		pid_c2 = fork ();
		if (pid_c2 > 0)
		{
			printf ("Child 2 with the PID %d created.\n", pid_c2);
			pid_c3 = fork ();
			if (pid_c3 > 0)
			{
				printf ("Child 3 with the PID %d created.\n", pid_c3);

				for (int i = 1; i <= 3; i++)
				{
					pid_c = wait (&w_status);

					if (WIFEXITED(w_status))
					{
						printf ("Child %d is terminated with Code %d.\n", pid_c, WEXITSTATUS(w_status));
					}
				}
			}
			else if (pid_c3 == -1)
				printf ("Child 3 is not created.\n");
		}
		else if (pid_c2 == -1)
			printf ("Child 2 is not created.\n");
	}
	else if (pid_c1 == -1)
		printf ("Child 1 is not created.\n");

	return 0;
}

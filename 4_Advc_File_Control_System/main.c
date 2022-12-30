/*
Name         : Prabhat Kiran
Date         : 26th December 2022
Description  : WAP to understand Advanced File Control System Calls.
1. Using fcntl() System call, synchronize a File between TWO processes (Parent and Child process).
2. Pass the File name using Command-Line Arguments.
3. Before writing to the File, check if the File is Locked. In case it is Locked, the Process must wait until it's Unlocked.
4. If its Unlocked, Lock the File and Continue with the writing.
5. Both the Process will do the same procedure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv [])
{
	if (argc == 1)											//If no Commands are passed as CLA; display the Error message.
	{
		printf ("Error: Insufficient arguments.\nUsage:- ./a.out <file_name>\n");
		return 0;
	}
	else if (argc == 2)										//Otherwise, proceed for the write operation on the File name passed.
	{
		int fd = open (argv [1], O_WRONLY);					//To Open the file in Write-only Mode.
		if (fd == -1)										//Error Handling.
		{
			perror ("open");
			return 0;
		}

		struct flock status;								//To declare the File Lock status Structure.
		memset (&status, 0, sizeof (struct flock));			//Initialize all the Structure members to 0.

		pid_t pid = fork ();								//To create the Child Process.

		if (pid > 0)										//If the 'pid' is greater than 0; it is the Parent Process.
		{
			printf ("PARENT: Locked the file\n");
			status.l_type = F_WRLCK;						//To place a Write Lock on the File.
			fcntl (fd, F_SETLKW, &status);					//To wait if File is Locked by another process and modify the File status to LOCKED.

			/* To write the Prime numbers until 100 in the opened File */
			printf ("PARENT: Writing to the file %s\n", argv [1]);
			write (fd, "Prime Numbers under 100 are: ", 29);
			char buf [5];
			for (int i = 2; i <= 100; i++)
			{
				int count = 0;
				for (int j = 1; j <= i; j++)
				{
					if (i % j == 0)
						count++;
				}

				if (count == 2)
				{
					sprintf (buf, "%d", i);
					write (fd, buf, strlen (buf));
					write (fd, " ", 1);
					memset (buf, 0, 5);
				}
			}
			write (fd, "\n", 1);

			printf ("PARENT: Unlocked the file\n");
			status.l_type = F_UNLCK;						//To release a Lock on the File.
			fcntl (fd, F_SETLKW, &status);					//To wait if File is Locked by another process and modify the File status to UNLOCKED.
		}
		else if (pid == 0)									//If the 'pid' is 0; it is the Child Process.
		{
			printf ("CHILD: Locked the file\n");
			status.l_type = F_WRLCK;						//To place a Write Lock on the File.
			fcntl (fd, F_SETLKW, &status);					//To wait if File is Locked by another process and modify the File status to LOCKED.

			/* To write the Fibonacci numbers until 100 in the opened File */
			printf ("CHILD: Writing to the file %s\n", argv [1]);
			write (fd, "Fibonacci Numbers under 100 are: ", 33);
			char buf [5];
			int first = 0, second = 1, third = 0;
			while (third < 100)
			{
				sprintf (buf, "%d", third);
				write (fd, buf, strlen (buf));
				write (fd, " ", 1);
				memset (buf, 0, 5);

				first = second;
				second = third;
				third = first + second;
			}
			write (fd, "\n", 1);

			printf ("CHILD: Unlocked the file\n");
			status.l_type = F_UNLCK;						//To release a Lock on the File.
			fcntl (fd, F_SETLKW, &status);					//To wait if File is Locked by another process and modify the File status to UNLOCKED.
		}
		else												//Error Handling.
		{
			perror ("fork");
			return 0;
		}

		close (fd);											//To Close the opened File.
	}
	else													//If there are multiple number of Files passed, display the Error message.
	{
		printf ("Error: Incorrect number of arguments.\nUsage:- ./a.out <file_name>\n");
	}

	return 0;
}

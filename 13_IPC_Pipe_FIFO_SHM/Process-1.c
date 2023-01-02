/*
Name         : Prabhat Kiran
Date         : 28th December 2022
Description  : Implement Communication between THREE Processes using PIPE, FIFO and SHM.
1. Create 2 different Process by executing 2 different a.out (executable) files.
2. Create a new Process (Child Process) in Process 1.
3. Create a Pipe between the Parent and Child Process.
4. Create a SHM from any of the Process (either Process-1 or Process-2).
5. In Process 1:
a. Prompt the User to enter a String in the Parent Process.
b. Write the String in the Pipe.
c. Read the String from the Pipe into Child Process.
d. Convert the String into Upper-case.
e. Write the modified String  in the SHM.
6. In Process-2:
a. Read the Data from the SHM.
b. Reverse the read Data from the SHM.
c. Write the Reverse String  in the FIFO.
7. Read the Data from FIFO in the Parent of Process-1.
8. Print the read String on 'stdout'.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main ()
{
	pid_t pid;
	int pipefd [2];							//To store the Read and Write End of the Pipe.
	pipe (pipefd);							//To create the Pipe.

	pid = fork ();							//To create the Child Process.

	if (pid > 0)							//If the 'pid' is greater than 0; it is the Parent Process.
	{
		char str [50];
		printf ("Enter the String to be written on the Pipe: ");
		scanf ("%49[^\n]", str);

		close (pipefd [0]);					//Close the Read End of the Pipe.

		int len = strlen (str);
		write (pipefd [1], str, len);		//Write the String on the Write End of the Pipe.

		close (pipefd [1]);					//Close the Write End of the Pipe.

		int ret = mkfifo ("FIFO", 0664);	//To create the FIFO Special File.
		if (ret == -1)						//Error Handling.
		{
			perror ("mkfifo");
			return 0;
		}

		int fd = open ("FIFO", O_CREAT | O_RDONLY, 0664);	//To Open the FIFO Special File in Read-only Mode.
		if (fd == -1)						//Error Handling.
		{
			perror ("open");
			return 0;
		}

		memset (str, 0, 50);				//To clear the 'str' variable to read the String.
		read (fd, str, len);				//To read the String from FIFO Special File.
		str [len] = '\0';
		printf ("The Modified Input String is: %s\n", str);

		close (fd);							//Close the FIFO Special File.
	}
	else if (pid == 0)						//If the 'pid' is 0; it is the Child Process.
	{
		close (pipefd [1]);					//Close the Write End of the Pipe.

		int shm_id = shmget ('P', 50, IPC_CREAT | 0664);	//To Create a Shared Memory segment with the given File permission.
		if (shm_id == -1)									//Error Handling.
		{
			perror ("shmget");
			return 0;
		}

		char* shm = (char*) shmat (shm_id, NULL, 0);		//To Attach the Child Process with the Shared Memory.
		if (shm == (char*) -1)								//Error Handling.
		{
			perror ("shmat");
			return 0;
		}

		char ch;
		int i = 0;
		while ((read (pipefd [0], &ch, 1) != 0))			//The loop shall run to convert the String to Uppercase and write in the Shared Memory.
		{
			if ((ch >= 'a') && (ch <= 'z'))
				ch = ch - 32;
			shm [i++] = ch;
		}
		shm [i] = '\0';

		shmdt (shm);										//To Detach the Child Process from the Shared Memory.
		close (pipefd [0]);									//Close the Read End of the Pipe.
	}
	else									//Error Handling.
	{
		printf ("The Child Process is not created.\n");
	}

	return 0;
}

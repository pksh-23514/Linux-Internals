/*
Name         : Prabhat Kiran
Date         : 28th December 2022
Description  : Implement Communication between THREE Processes using PIPE, FIFO and SHM.
1. Create 2 different Process by executing 2 different a.out (executable) files.
2. Create a new Process (Child Process) in Process 1.
3. Create a Pipe between the Parent and Child Process.
4. Create a SHM from any of the Process (either Process-1 or Process-2).
5. In Parent Process 1:
a. Prompt the User to enter a String.
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
	int shm_id = shmget ('P', 50, 0);					//To Open the Shared Memory segment already created in the Process-1.
	if (shm_id == -1)									//Error Handling.
	{
		perror ("shmget");
		return 0;
	}

	char* shm = (char*) shmat (shm_id, NULL, 0);		//To Attach the Process-2 with the Shared Memory.
	if (shm == (char*) -1)								//Error Handling.
	{
		perror ("shmat");
		return 0;
	}

	int len = strlen (shm);

	for (int i = 0; i <= (len / 2); i++)				//The loop shall run to reverse the read String from the Shared Memory.
	{
		char temp = shm [i];
		shm [i] = shm [len - i - 1];
		shm [len - i - 1] = temp;
	}

	int fd = open ("FIFO", O_WRONLY);					//To Open the FIFO Special File in Write-only Mode.
	if (fd == -1)										//Error Handling.
	{
		perror ("open");
		return 0;
	}

	write (fd, shm, len);								//To write the Reversed String in the FIFO Special File.

	close (fd);											//Close the FIFO Special File.
	shmdt (shm);										//To Detach the Process-2 from the Shared Memory.
	shmctl (shm_id, IPC_RMID, NULL);					//To Close the Shared Memory created.

	return 0;
}

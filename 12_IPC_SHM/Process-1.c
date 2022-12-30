/*
Name         : Prabhat Kiran
Date         : 23rd December 2022
Description  : Implement Communication between TWO different Processes using Shared Memory (SHM).
1. Create 2 different Process by executing 2 Different a.out (executable) Files.
2. Create SHM from any one of the Process; either Process-1 or Process-2.
3. In Process-1:
a) Prompt the user to enter a String.
b) Convert the String into Upper case.
c) Write the Upper case String in Shared Memory.
4. In Process-2:
a) Read the Data from the Shared Memory.
b) Reverse the Input which is read from SHM.
c) Write the Reverse String in Shared Memory.
5. Read the Data from Shared memory in Process-1.
6. Print the Output string on 'stdout'.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/ipc.h>

int main ()
{
	int shm_id = shmget ('P', 100, IPC_CREAT | 0664);		//To Create a Shared Memory segment with the given File permission.
	if (shm_id == -1)										//Error Handling.
	{
		perror ("shmget");
		return 0;
	}

	char* shm = (char*) shmat (shm_id, NULL, 0);			//To Attach the Process-1 with the Shared Memory.
	if (shm == (char*) (-1))								//Error Handling.
	{
		perror ("shmat");
		return 0;
	}

	printf ("Enter the String to be written: ");
	scanf ("%[^\n]", shm);
	printf ("Writing to the Shared Memory...\n");

	int i = 0;
	while (shm [i] != '\0')									//To Convert the read String into Uppercase.
	{
		if ((shm [i] >= 'a') && (shm [i] <= 'z'))
		{
			shm [i] -= 32;
		}
		
		i += 1;
	}

	sleep (5);												//To Wait for the Process-2 to read and reverse the written String on the SHM.

	printf ("Reading from the Shared Memory: %s\n", shm);

	shmdt (shm);											//To Detach the Process-1 from the Shared Memory.

	shmctl (shm_id, IPC_RMID, NULL);						//To Remove the Shared Memory segment.

	return 0;
}

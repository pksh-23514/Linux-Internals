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
	sleep (5);												//To wait for the Process-1 to write the Uppercase String in the SHM.

	int shm_id = shmget ('P', 100, 0);						//To Get the SHM ID for the Shared Memory created in Process-1.
	if (shm_id == -1)										//Error Handling.
	{
		perror ("shmget");
		return 0;
	}

	char* shm = (char*) shmat (shm_id, NULL, 0);			//To Attach the Process-2 with the Shared Memory.
	if (shm == (char*) (-1))								//Error Handling.
	{
		perror ("shmat");
		return 0;
	}

	printf ("Reading from the Shared Memory: %s\n", shm);

	int len = 0;
	while (shm [++len] != '\0');							//To find the Length of the String read from the SHM.

	printf ("Writing to the Shared Memory...\n");

	for (int i = 0; i <= (len / 2); i++)					//To Reverse the String read from the SHM.
	{
		char temp = shm [i];
		shm [i] = shm [len - i - 1];
		shm [len - i - 1] = temp;
	}

	shmdt (shm);											//To Detach the Process-2 from the Shared Memory.

	return 0;
}

/*
Name         : Prabhat Kiran
Date         :
Description  :
Sample Input :
Sample Output:
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
	int shm_id = shmget ('P', 100, IPC_CREAT | 0664);
	if (shm_id == -1)
	{
		perror ("shmget");
		return 0;
	}

	char* shm = (char*) shmat (shm_id, NULL, 0);
	if (shm == (char*) (-1))
	{
		perror ("shmat");
		return 0;
	}

	printf ("Enter the String to be written: ");
	scanf ("%[^\n]", shm);
	printf ("Writing to the Shared Memory...\n");

	int i = 0;
	while (shm [i] != '\0')
	{
		if ((shm [i] >= 'a') && (shm [i] <= 'z'))
		{
			shm [i] -= 32;
		}
		
		i += 1;
	}

	sleep (10);

	printf ("Reading from the Shared Memory: %s\n", shm);

	shmdt (shm);

	shmctl (shm_id, IPC_RMID, NULL);

	return 0;
}

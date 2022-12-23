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
	sleep (5);

	int shm_id = shmget ('P', 100, 0);
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

	printf ("Reading from the Shared Memory: %s\n", shm);

	int len = 0;
	while (shm [++len] != '\0');

	printf ("Writing to the Shared Memory...\n");

	for (int i = 0; i <= (len / 2); i++)
	{
		char temp = shm [i];
		shm [i] = shm [len - i - 1];
		shm [len - i - 1] = temp;
	}

	shmdt (shm);

	return 0;
}

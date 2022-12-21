/*
Name         : Prabhat Kiran
Date         : 15th December 2022
Description  : WAP to understand usage of 'dup' and 'dup2' System Calls.
1. Using dup or dup2, redirect "printf" to a given file instead of printing to 'stdout'.
2. Pass the file name using command-line arguments.
3. Try using both system calls (dup and dup2).
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv [])
{
	if (argc < 2)	//If the CLAs passed are less than 2; there is no File available.
	{
		printf ("The file name is not passed in the Command Line Arguments.\n");
		return 0;
	}
	else			//If the CLAs passed are equal to 2; proceed for the Duplicate System Call.
	{
		int choice, fd_backup, stdout_backup;

		int fd = open (argv [1], O_WRONLY);						//To Open the File.
		if (fd == -1)											//If the File doesn't exists; open() will return -1.
		{
			printf ("The file name passed in the Command Line Arguments does not exists.\n");
			return 0;
		}
		else													//If the File exists, proceed for the Duplicate System Call.
		{
			fd = open (argv [1], O_WRONLY | O_TRUNC);			//The flag 'O_TRUNC' with 'O_WRONLY' takes a file and truncates its length to 0.
			
			printf ("Menu:\n1. Dup\n2. Dup2\n");				//Display the Menu and ask for the User to choose the System Call to be used.
			printf ("Please enter the Choice: ");
			scanf ("%d", &choice);

			if (choice == 1)									//If the User enters '1'; use the dup() function for the operation.
			{
				stdout_backup = dup (1);						//Duplicate the 'stdout' File Descriptor in a Backup File Descriptor.
				close (1);										//Close the 'stdout' File Descriptor.
				fd_backup = dup (fd);							//Duplicate the File Descriptor for the File from CLA in a Backup File Descriptor.
				printf ("Print this message in the FILE.\n");	//Since 'stdout' is closed and 'fd_backup' is File Descriptor Index '1'; the message is printed on the File.
				fflush (stdout);								//Clear the Output Buffer.

				close (fd_backup);								//Close the 'fd_backup' to free the File Descriptor Index '1'.
				dup (stdout_backup);							//Duplicate the 'stdout_backup' File Descriptor back to the File Descriptor Index '1'.
				printf ("Print this message in the STDOUT.\n");	//The 'stdout' is reinitialized the File Descriptor Index '1'; the message is printed on the Console.
			}
			else if (choice == 2)								//If the User enters '1'; use the dup2() function for the operation.
			{
				stdout_backup = dup2 (1, 5);					//Duplicate the 'stdout' File Descriptor in the File Descriptor Index '5'.
				fd_backup = dup2 (fd, 1);						//Duplicate the File Descriptor for the File from CLA in the File Descriptor Index '1'.
				printf ("Print this message in the FILE.\n");	//Since 'stdout' is closed and 'fd_backup' is File Descriptor Index '1'; the message is printed on the File.
				fflush (stdout);								//Clear the Output Buffer.

				dup2 (stdout_backup, 1);						//Duplicate the 'stdout_backup' File Descriptor in the File Descriptor Index '1'.
				printf ("Print this message in the STDOUT.\n");	//The 'stdout' is reinitialized the File Descriptor Index '1'; the message is printed on the Console.
				close (fd_backup);								//Close the 'fd_backup'.
			}

			close (fd);											//Close the File passed in CLA.
		}
	}

	return 0;
}

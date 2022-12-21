/*
Name         : Prabhat Kiran
Date         : 14th December 2022
Description  : Implement a "cp" (Copy) Command with "–p" Option.
1. Copy source file to destination file which passed through cmd-line arguments. After copying both files  must have equal size, even it’s a zero sized file.
Eg:  ./my_copy source.txt dest.txt
2. If arguments are missing show the usage (help) info to use.
3. Implement a my_copy() function where you have to pass two file descriptors. int my_copy(int source_fd, int dest_fd).
4. If –p option passed copy permissions as well to destination (refer ‘fstat’ man page).  Eg: - ./my_copy –p source.txt dest.txt.
5. If the destination file is not present then create a new one with given name. In case if it is present show a confirmation from user to overwrite.
Eg: - ./my_copy source.txt destination.txt .File  “destination.txt” is already exists. Do you want to overwrite (Y/n)
If user type ‘Y/n’ or enter key overwrite the destination with new file. In n/N don’t overwrite and exit from program.
6. Program should able handle all possible error conditions.
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
	if (argc < 2)				//If the CLAs passed are less than 2; there is no Source or Destination File available.
	{
		printf ("cp: missing file operand\n");
		return 0;
	}
	else if (argc < 3)			//If the CLAs passed are less than 3; there is no Destination File available.
	{
		printf ("cp: missing destination file operand after '%s'\n", argv [1]);
		return 0;
	}
	else if (argc == 3)			//If the CLAs passed are equal to 3; proceed for the Copy operation.
	{
		int source_fd = open (argv [1], O_RDONLY);		//To Open the Source File.
		if (source_fd == -1)							//If the Open is unsuccessful, display the Error message.
		{
			perror ("open");							//To display the Error message.
			return 0;									//Terminate the Copy operation.
		}

		char choice, buff [50];
		int ret = 0;

		int dest_fd = open (argv [2], O_CREAT | O_WRONLY | O_EXCL, 0664);	//To Open the Destination File.
		if (dest_fd == -1)													//If the Open is unsuccessful, check for the reason of the generated Error.
		{
			if (errno == EEXIST)											//If the File already exists, give a Choice to the User.
			{
				printf ("The file already exists\n");
				printf ("Do you want to Overwrite the file contents? ");
				scanf ("%c", &choice);										//Ask for the User choice.

				if ((choice == 'y') || (choice == 'Y'))						//If the User enters 'y'/'Y'; the Destination File shall be Overwritten.
				{
					dest_fd = open (argv [2], O_WRONLY | O_TRUNC);			//The flag 'O_TRUNC' with 'O_WRONLY' takes a file and truncates its length to 0.
				}
				else														//If the User enters 'n'/'N'; terminate the Copy operation.
				{
					return 0;
				}
			}
			else															//If the Open is unsuccessful due to some other Error; display the Error message.
			{
				perror ("open");											//To display the Error message.
				return 0;													//Terminate the Copy operation.
			}
		}

		do												//The loop shall run to Copy the Source File data to Destination File.
		{
			ret = read (source_fd, buff, 50);			//To read a maximum of 50 bytes of Data from the Source File.
			write (dest_fd, buff, ret);					//To write the 'ret' bytes of Data read from the Source File to the Destination File.
		} while (ret == 50);							//The loop shall terminate if the value of 'ret' becomes less than 50.

		close (source_fd);								//Close the Source File.
		close (dest_fd);								//Close the Destination File.
	}
	else if (argc == 4)			//If the CLAs passed are equal to 3; proceed for the Copying operation keeping the File permissions same for Souce and Destination Files.
	{
		if ((strncmp (argv [1], "-p", 3)) != 0)			//If the 2nd CLA doesn't matches the flag "-p"; the CLAs passed are incorrect and display the Error message.
		{
			printf ("The flag '-p' is not present in the Command Line Arguments.\n");
			return 0;									//Terminate the Copy operation.
		}
		else											//If the 2nd CLA matches the flag "-p"; proceed for the Copy operation.
		{
			int source_fd = open (argv [2], O_RDONLY);	//To Open the Source File.
			if (source_fd == -1)						//If the Open is unsuccessful, display the Error message.
			{
				perror ("open");						//To display the Error message.
				return 0;								//Terminate the Copy operation.
			}

			char choice, buff [50];
			int ret = 0;
			struct stat f_details;						//To store the Source File Stats.

			ret = fstat (source_fd, &f_details);		//To retrieve the information about the Source File.
			if (ret == -1)								//Error Handling.
			{
				perror ("fstat");						//To display the Error message.
				return 0;								//Terminate the Copy operation.
			}

			ret = 0;
			int dest_fd = open (argv [3], O_CREAT | O_WRONLY | O_EXCL, f_details.st_mode);		//To Open the Destination File with the Source File Mode.
			if (dest_fd == -1)																	//If the Open is unsuccessful, check for the reason of the generated Error.
			{
				if (errno == EEXIST)										//If the File already exists, give a Choice to the User.
				{
					printf ("The file already exists\n");
					printf ("Do you want to Overwrite the file contents? ");
					scanf ("%c", &choice);									//Ask for the User choice.

					if ((choice == 'y') || (choice == 'Y'))					//If the User enters 'y'/'Y'; the Destination File shall be Overwritten.
					{
						dest_fd = open (argv [3], O_WRONLY | O_TRUNC);		//The flag 'O_TRUNC' with 'O_WRONLY' takes a file and truncates its length to 0.
						fchmod (dest_fd, f_details.st_mode);				//Change the Permissions for the Destination File same as the Source File.
					}
					else													//If the User enters 'n'/'N'; terminate the Copy operation.
					{
						return 0;
					}
				}
				else														//If the Open is unsuccessful due to some other Error; display the Error message.
				{
					perror ("open");										//To display the Error message.
					return 0;												//To terminate the Copy operation.
				}
			}

			do																//The loop shall run to Copy the Source File data to Destination File.
			{
				ret = read (source_fd, buff, 50);							//To read a maximum of 50 bytes of Data from the Source File.
				write (dest_fd, buff, ret);									//To write the 'ret' bytes of Data read from the Source File to the Destination File.
			} while (ret == 50);											//The loop shall terminate if the value of 'ret' becomes less than 50.

			close (source_fd);												//Close the Source File.
			close (dest_fd);												//Close the Destination File.
		}
	}

	return 0;
}

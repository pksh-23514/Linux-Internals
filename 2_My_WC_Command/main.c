/*
Name         : Prabhat Kiran
Date         : 16th December 2022
Description  : Implement a WC (Word-Count) command with "–l" "–w" "-c" Options.
1. Count the Number of Words, Lines and Characters (bytes) from Files passed through Command Line.
2. If more than one Files are passed, print Individual Count values and File Name and Calculate the Total of all Values and print at the end.
3. If no File are passed, "wc" will read from Standard Input till End of File (Ctrl + D) and then count Lines, Words and Characters (bytes).
4. Implement a word_count() function where you have to pass 'fd' and '3 integer addresses' (Pass by Reference). Prototype: int word_count (int fd, int *lines, int *words, int *bytes);
5. The word_count function will read from the 'fd' and Calculates Lines, Words and Characters, then stores into respective Addresses passed (don’t Print values inside the Function).
6. Main function will Open the Files in a Loop and Call word_count function depending upon Number of Files passed. Print the values after calling the Function in main().
7. If Options passed [-l –w –c], Print only the respective values.
8. Program should able to handle all the possible Error conditions.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void word_count (int file, int* c_count, int* w_count, int* l_count)
{
	int flag = 0;												//To keep a check on the Word Count in case of Multiple Spaces occurence.
	char ch;

	while ((read (file, &ch, 1)) != 0)							//To read one Character from the File.
	{
		*c_count += 1;											//Increment the Character count for every valid Character.

		if ((ch == ' ') || (ch == '\n') || (ch == '\t'))		//If a Space, Tab or New-line is encountered.
		{
			if (flag == 0)										//If the occurence is first time, increment the Word count and set the Flag as 1.
			{
				*w_count += 1;
				flag = 1;
			}
		}
		else													//If the Character is anything other than Space, Tab or New-line, reset the Flag to 0.
		{
			flag = 0;
		}

		if (ch == '\n')											//If a New-line is encountered, increment the Line count.
			*l_count += 1;
	}

	if ((ch != '\n') && (ch != ' ') && (ch != '\t'))			//If no ending Space or Tab or New-line is encountered, increment the Word Count by 1 to include the Last Word encountered.
		*w_count += 1;
}

int main (int argc, char* argv [])
{
	int c_count = 0, w_count = 0, l_count = 0;
	if (argc == 1)												//If no Command Line Arguments are passed, read the Input from Standard Input.
	{
		word_count (0, &c_count, &w_count, &l_count);			//To calculate the Character, Word and Line Counts.
		printf ("\n%d\t%d\t%d\n", l_count, w_count, c_count);
	}
	else														//If the Command Line Arguments are passed.
	{
		int c_flag = 0, w_flag = 0, l_flag = 0, ret = 0, fd;

		while ((ret = getopt (argc, argv, "lwc")) != -1)		//To check which all Options are passed in the CLA.
		{
			switch (ret)
			{
				case 'l' : l_flag = 1;
						   break;
				case 'w' : w_flag = 1;
						   break;
				case 'c' : c_flag = 1;
						   break;
			}
		}

		if ((l_flag == 0) && (w_flag == 0) && (c_flag == 0))	//If no Options are passed in CLA, display all the 3 parameters.
		{
			l_flag = w_flag = c_flag = 1;
		}

		/* The getopt() function is a Command Line Parser to retrieve all the Options passed as per the 'opstring'. The variable 'optind' is the index of the next element of argv[] to be processed.
		   By default, getopt() permutes the contents of argv as it scans, so that eventually all the Non-options are at the end of argv[].
		   'optind' is initialized to 1, and getopt() updates it as it processes each element of argv[]. So, 'optind' will point to the arguments other than Options. */

		if (argv [optind] == NULL)								//If no File is passed in the CLA, read the Input from Standard Input.
		{
			word_count (0, &c_count, &w_count, &l_count);		//To calculate the Character, Word and Line Counts.

			/* Display the Output based on the Options passed in the CLA */
			printf ("\n");
			if (l_flag == 1)
				printf ("%d\t", l_count);
			if (w_flag == 1)
				printf ("%d\t", w_count);
			if (c_flag == 1)
				printf ("%d", c_count);
			printf ("\n");
		}
		else													//If the Files are passed in the CLA, read the Input from each of the Files.
		{
			int total_l = 0, total_w = 0, total_c = 0, f_count = 0;
			while (argv [optind] != NULL)						//The loop shall run till all the Files are read.
			{
				f_count += 1;									//To check if the Total Count shall be displayed or not.
				l_count = w_count = c_count = 0;
				fd = open (argv [optind], O_RDONLY);			//To Open the File passed in the CLA.
				if (fd == -1)									//Error Handling.
				{
					perror ("open");
					return 0;
				}

				word_count (fd, &c_count, &w_count, &l_count);	//To calculate the Character, Word and Line Counts.

				/* Update the Total Count of the Characters, Words and Lines with each Counts from the File */
				total_l += l_count;
				total_w += w_count;
				total_c += c_count;

				/* Display the Output based on the Options passed in the CLA */
				if (l_flag == 1)
					printf ("%d\t", l_count);
				if (w_flag == 1)
					printf ("%d\t", w_count);
				if (c_flag == 1)
					printf ("%d\t", c_count);
				printf ("%s\n", argv [optind]);

				close (fd);										//To Close the File passed in the CLA.
				optind += 1;									//Increment the 'optind' to move the next File in the CLA.
			}

			if (f_count > 1)									//If there are more than 1 Files present, display the Total Count of the Characters, Words and Lines.
			{
				/* Display the Output based on the Options passed in the CLA */
				if (l_flag == 1)
					printf ("%d\t", total_l);
				if (w_flag == 1)
					printf ("%d\t", total_w);
				if (c_flag == 1)
					printf ("%d\t", total_c);
				printf ("Total\n");
			}
		}
	}

	return 0;
}

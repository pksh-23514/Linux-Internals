/*
Name         : Prabhat Kiran
Date         : 16th December 2022
Description  : Implement a WC (Word-Count) command with "–l" "–w" "-c" Options.
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
	int flag = 0;
	char ch;

	while ((read (file, &ch, 1)) != 0)
	{
		*c_count += 1;

		if ((ch == ' ') || (ch == '\n') || (ch == '\t'))
		{
			if (flag == 0)
			{
				*w_count += 1;
				flag = 1;
			}
		}
		else
			flag = 0;

		if (ch == '\n')
			*l_count += 1;
	}

	if ((ch != '\n') && (ch != ' '))
		*w_count += 1;
}

int main (int argc, char* argv [])
{
	int c_count = 0, w_count = 0, l_count = 0;
	if (argc == 1)
	{
		word_count (0, &c_count, &w_count, &l_count);
		printf ("\n%d %d %d\n", l_count, w_count, c_count);
	}
	else
	{
		int c_flag = 0, w_flag = 0, l_flag = 0, ret = 0, fd;

		while ((ret = getopt (argc, argv, "lwc")) != -1)
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

		if (ret == 0)
		{
			l_flag = w_flag = c_flag = 1;
		}

		if (argv [optind] == NULL)
		{
			word_count (0, &c_count, &w_count, &l_count);

			if (l_flag == 1)
				printf ("%d\t", l_count);
			if (w_flag == 1)
				printf ("%d\t", w_count);
			if (c_flag == 1)
				printf ("%d\n", c_count);
		}
		else
		{
			while (argv [optind] != NULL)
			{
				fd = open (argv [optind], O_RDONLY);
				if (fd == -1)
				{
					perror ("open");
					return 0;
				}

				word_count (fd, &c_count, &w_count, &l_count);

				if (l_flag == 1)
					printf ("%d\t", l_count);
				if (w_flag == 1)
					printf ("%d\t", w_count);
				if (c_flag == 1)
					printf ("%d\n", c_count);

				close (fd);
				optind += 1;
			}
		}
	}
}

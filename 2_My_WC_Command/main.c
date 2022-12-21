/*
Name         : Prabhat Kiran
Date         : 16th December 2022
Description  : Implement a WC (Word-Count) command with "–l" "–w" "-c" Options.
Sample Input :
Sample Output:
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
	char ch;
	int c_count = 0, w_count = 0, l_count = 0, flag = 0;

	if (argc == 1)
	{
		while ((read (0, &ch, 1)) != 0)
		{
			c_count++;

			if ((ch == ' ') || (ch == '\n') || (ch == '\t'))
			{
				if (flag == 0)
				{
					w_count++;
					flag = 1;
				}
			}
			else
				flag = 0;

			if (ch == '\n')
				l_count++;
		}

		if ((ch != '\n') && (ch != ' '))
			w_count++;
		printf ("\n%d %d %d\n", c_count, w_count, l_count);
	}
	else
	{
		int c_flag = 0, w_flag = 0, l_flag = 0, ret;

		while ((ret = getopt (argc, argv, "lwc") != -1))
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

		if (argv [optind] == NULL)
		{
			while ((read (0, &ch, 1)) != 0)
			{
				c_count++;

				if ((ch == ' ') || (ch == '\n') || (ch == '\t'))
				{
					if (flag == 0)
					{
						w_count++;
						flag = 1;
					}
				}
				else
					flag = 0;

				if (ch == '\n')
					l_count++;
			}

			if ((ch != '\n') && (ch != ' '))
				w_count++;
			printf ("\n%d %d %d\n", c_count, w_count, l_count);

		}
		return 0;
	}
}

/*
Name         : Prabhat Kiran
Date         : 27th December 2022
Description  : WAP to implement Alarm with Snooze for given Time and Date using SIGALRM.
1. User gives the Time and Date from Command Line Arguments.
2. Validate the Time. Do not go behind the current Time.
3. Date is optional. If User is not providing the Date, consider it as Today.
4. In Signal Handler, avoid all User communications (printf, scanf etc) and loops. Make it as minimal as possible.
5. After the Alarm expires, Display an ALARM message.
6. Prompt the User whether to Stop or Snooze.
7. If User selects Stop, Terminate the Program.
8. If User selects Snooze, Prompt for Snooze Time in Seconds. If User enters the Time, Reset the Alarm to the entered Time in Seconds.
*/

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

/* Signal Handler for SIGALARM */
void handler (int);

/* Global Variable */
int flag;

int main (int argc, char* argv[])
{
	signal (SIGALRM, handler);	//To change the Action taken if SIGALARM is detected.
	
	unsigned int result;
	
	if (argc == 1)		//If no CLA are passed, display an Error message.
	{
		printf ("Error: No arguments passed\nUsage: ./alarm  [dd/mm/yy]\n");
		return 0;
	}
	else if (argc > 3)	//If more than 3 arguments are passes in CLA, display an Error message.
	{
		printf("Error: More arguments passed\nUsage: ./alarm  [dd/mm/yy]\n");
	}
	else	//Otherwise, procees for the Time processing.
	{
		struct tm *st, user;	//Time structure to store the various parameters from the Input.

		time_t user_time, sec_time;

		sec_time = time (NULL);			//To Get current Epoch Time.
		st = localtime (&sec_time);		//Structure Pointer to hold Address of parameters having Local Time.

		memset (&user, 0, sizeof (struct tm));

		if (argc == 2)	//If only Time is passed in CLA.
		{
			/* Read the Time value and store in the Structure */
			strptime (argv[1],"%H:%M:%S",&user);	//To Convert a String representation of Time to a Time 'tm' Structure.

			/* Store the Today's Date in the Structure */
			user.tm_mday = st->tm_mday;
			user.tm_mon = st->tm_mon;
			user.tm_year = st->tm_year;
			
			/* To Convert the Structure 'tm' to a Time since the Epoch value */
			user_time = mktime (&user);
		}
		else	//If the Time and Data values are passed in CLA.
		{
			/* Read the Time value and store in the Structure */
			strptime (argv[1],"%H:%M:%S",&user);
			/* Read the Date value and store in the Structure */
			strptime (argv[2],"%d-%m-%Y",&user);

			/* To Convert the Structure 'tm' to a Time since the Epoch value */
			user_time = mktime (&user);
		}

		if (user_time > sec_time)	//To check if given Time is Future Time w.r.t. Local Time.
		{
			int choice = 0, min;
			result = user_time - sec_time;	//To find the Difference between the given Time and Local current Time.

			/* To Set an Alarm with Epoch difference */
			alarm (result);		//Arranges for a SIGALRM signal to be Delivered to the Calling Process.
			printf ("Alarm set for %02d:%02d:%02d %02d/%02d/%d\n", user.tm_hour, user.tm_min, user.tm_sec, user.tm_mday, (user.tm_mon + 1), (user.tm_year + 1900));

			while (1)	//To continuously Set the Alarm until the User chooses to Stop.
			{
				if (flag == 1)	//If the SIGALARM signal is received.
				{
					flag = 0;	//Reset the 'flag' to 0.

					/* Display the Alarm message and ask for the Option to Snooze or Stop */
					printf ("Wakeup...Alarm...!!\n1. Snooze\n2. Exit\nEnter your Choice: ");
					scanf ("%d", &choice);

					if (choice == 1)	//If the User chooses the Snooze option.
					{
						printf ("Enter Snooze time (in Seconds): ");
						scanf ("%d", &result);	//To read the Snooze Time from the User for setting the next Alarm.

						alarm (result);	//To Set an Alarm with Epoch difference.
						
						signal (SIGALRM, handler);	//To change the Action taken if SIGALARM is detected.
					}
					else if (choice == 2)	//If the User chooses the Stop option.
					{
						exit (0);	//Terminate the Program.
					}
				}
			}
		}
		else	//Otherwise, the Time is from the Past and Alarm cannot be set.
		{
			printf ("Error: Invalid Time\n");
			return 0;
		}
	}

	return 0;
}

void handler (int signum)
{
	flag = 1;	//If SIGALARM is received, set the 'flag' as 1.
}

/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h> 
#include <signal.h>
#include <sys/types.h>
//#include <linux/inotify.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "wiringPi.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define DIRECTORY "/var/relay"
#define DIREND "/var/relay/remove"

int running;

void signal_callback_handler(int signum)
{
	printf("Caught signal %d\n",signum);
	printf("Shutting Down Cleanly\n");
	
	running = 0;
	
	struct stat st = {0};

	if (stat(DIREND, &st) == -1)
	{
		mkdir(DIREND, 0777);
	}
	rmdir(DIREND);
	
	//return 0;
	
}


int main( )
{
	int length, i = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];
	DIR *d;
	struct dirent *dir;
	int count;
  
	signal(SIGTERM, signal_callback_handler);

	/*creating the INOTIFY instance*/
	fd = inotify_init();
	
	running = 1;
	
	wiringPiSetup();
	pinMode (0, OUTPUT) ;
	digitalWrite (0,  LOW) ;


	/*checking for error*/
	if ( fd < 0 )
	{
		perror( "inotify_init" );
	}

	/*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
	wd = inotify_add_watch( fd, DIRECTORY, IN_CREATE | IN_DELETE );

	printf("Starting Relay Monitor\n");

	while(running == 1)
	{
	
		length = 0;
		i = 0;
		
		/*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 
		length = read( fd, buffer, EVENT_BUF_LEN ); 

		/*checking for error*/
		if ( length < 0 ) {
			perror( "read" );
		}  
		
		/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
		while ( i < length )
		{
			struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
			if ( event->len )
			{
				count = 0;
				d = opendir(DIRECTORY);
				if (d)
				{			
					while ((dir = readdir(d)) != NULL)
					{
						count++;
					}
				}
				closedir(d);
				
				if(count == 2)
				{
					printf( "Turning OFF Relay\n");	
					digitalWrite (0, LOW);				
				}
				else
				{
					printf( "Turning ON Relay\n");
					digitalWrite (0,  HIGH) ;
				}
			
			}
			
			i += EVENT_SIZE + event->len;
		}
	}
	
	/*removing the “/tmp” directory from the watch list.*/
	inotify_rm_watch( fd, wd );
	
	/*closing the INOTIFY instance*/
	close( fd );
	
	printf("Shut down\n");
	
	return 0;

}
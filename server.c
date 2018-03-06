#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<netdb.h>
#define RECEIVER_HOST "127.0.0.1" /* Server machine */
#define BUFSIZE 100

/* Declaring errno */
extern int errno;
/* Function for printing error */
void report_error(char *s)
{
 printf("receiver: error in%s, errno = %d\n", s, errno);
 exit(1);
}

int main()
{
 int s;//to create socket
 int pid;
 char msg[BUFSIZE];
 int news;//to accept connection
 //int pid;
 struct sockaddr_in sa = {0},r_sa = {0} ;//server receive on this address
 int len;
 int r_sa_l;
 int backlog = 5;
 int length;
 char connection[BUFSIZE];
/* Copying the string m into response as couldnot initialize
variable-sized array */
 /* Creating the socket and returns error if unsuccesfull */
 s=socket(AF_INET,SOCK_STREAM,0);
 printf("socket: %d",s);
 //initialize the socket addresses
 memset(&sa,0,sizeof(sa));
 sa.sin_family=AF_INET;
 sa.sin_addr.s_addr=htonl(INADDR_ANY);
 sa.sin_port=htons(9975 + 20000);

/* Binding the socket */
 bind(s,(struct sockaddr *)&sa, sizeof(sa));
 printf("\nserver started\n");
 listen(s,5);
 length = sizeof(r_sa);

 while(1)
 {
  /*parent process*/ 
  printf("\nserver open for new clients:\n");
  r_sa_l=sizeof(r_sa);
  news=accept(s,(struct sockaddr*)&r_sa,&r_sa_l);
  printf("client connected\n");
  inet_ntoa(r_sa.sin_addr);
  /*child process*/
  pid=fork();
  if(pid==0)//child process rec and send
  {
   //rceive from client
   while(1)
   {
    /* Receiving message from client*/
    len=recv(news,msg,BUFSIZE,0);
    if(len==0)
    {
     close(news);
     break;
    }
    	
     if(msg[0]=='c'&& msg[1]=='d')
    	{
	   printf(" Change Directory\n");
	   int a,i=0;
	   int out =2;
	   char dir[BUFSIZE] = "\0";
	   char new_dir[BUFSIZE] = "\0";
	   char path[BUFSIZE];
    	   printf("CURRENT DIRECTORY is:\n\n  %s \n", getcwd(path,BUFSIZE));	
	   strncpy(dir,msg+3,strlen(msg));
	   strncpy(new_dir,dir,strlen(dir)-1);
	   a = chdir(new_dir);
	   printf("NEW DIRECTORY is:\n\n %s \n", getcwd(path,BUFSIZE));      
	}
     
	else if(msg[0]=='l'&& msg[1]=='s')
    	{
	   printf(" LISTING FILES \n");
	   int a;
	   int out =1;
	   pid = fork();
 	   if(pid == 0)
	   {
	  
	   char *args[] = {"ls", NULL};
	   a = execv("/bin/ls", args);
           exit(1);
	   }
	   else
	   {
	      wait(NULL);
	   }
	}

	else if(msg[0] == 'q')
	{
	   close(news);
           exit(1);
	}

	else 
	{
	   char message1[BUFSIZE] = "enter commands";
	   int i = strlen(message1);
	   message1[i]=0;
          /* Sending response to client */
	   send(news,message1,i,0);
	   continue;
	}

	
        char message[BUFSIZE] = "to client                  ";
 	int i = strlen(message);
	message[i]=0;
	msg[i]=0;        
        /* Sending response to client */
        send(news,message,i,0);
        printf("command received:%s\n",msg); 
     }
  exit(0);
  }
  else
  {
   close(news);//socket is closed
  }
 }
 return 0;
}



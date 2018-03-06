#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/time.h>
#include<errno.h>
#include<arpa/inet.h>


#define RECEIVER_HOST "127.0.0.1" /* Server machine */
/* Declaring errno */
extern int errno;

/* Function for error */
void report_error(char *s)
{
 printf("sender: error in %s, errno = %d\n",s,errno);
 exit(1);
}

/* Giving 'size' of message dynamically as argument */
int main(int argc, char *argv[])
{
 int s,n;
 int BUFSIZE = atoi(argv[1]);
 char msg[BUFSIZE];
 char received[BUFSIZE];
 struct sockaddr_in sa = {0};
 int length = sizeof(sa);
 
 /* Creating the socket*/
 s=socket(AF_INET,SOCK_STREAM,0);
 printf("socket: %d",s);
 //initialize the socket addresses
 memset(&sa,0,sizeof(sa));
 sa.sin_family=AF_INET;
 sa.sin_addr.s_addr=inet_addr(RECEIVER_HOST);
 sa.sin_port=htons(9975 + 20000); /* define port number based on student ID*/

 //client connecting to server
 connect(s,(struct sockaddr *)&sa,sizeof(sa));
 printf("\nclient established connection\n");
 //send to sever and receive from server
 while(1)
 {
  printf("\nmessage to server:\n");
  fgets(msg,BUFSIZE,stdin);
  if(msg[0]=='#')
   break;
  

  n=strlen(msg)+1;
   /* Sending the message to server */
  send(s,msg,n,0);
  //if(sendto(s, msg, BUFSIZE, 0, (struct sockaddr *) &sa, length)== -1)
  if(msg[0]=='q')
   {
	printf("close socket\n");
    close(s);
	break;
    }
  /* Receives message from server */
  // recvfrom(s, received, BUFSIZE, 0, (struct sockaddr *) &sa, &length);
  n=recv(s,received,BUFSIZE,0);

  printf("Message received from server %s\n",received);
  //printf("length of received msg is %d\n",strlen(msg2));
 }

 return 0;
}



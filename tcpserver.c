#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#define bufsize 1024

void eatZombies(int n){
  // This function removes the zombie process state left
  // after the child exits. Learn about this in NWEN 301!

  wait3(NULL,WNOHANG,NULL); // Nom Nom
}

int main(int argc, char *argv[]){
  int sock, length, msgsock, status, newsockfd;
  struct sockaddr_in server, client;
  int data;
  char buf[bufsize];


  // for forking, and cleaning up zombie child state afterwards
  // You must not change this code.

  pid_t id;
  signal(SIGCHLD, &eatZombies);

  // OK, NWEN 243 code starts here.

  // Create a socket (see Lab 2) - it is exactly the same for a server!
  int sockfd = socket(AF_INET, SOCK_STREAM, 1);
  
  if(sockfd < 0)
  {
    fprintf(stderr,"Failed creating socket\n");
  }
  elseif (sockfd >= 0){
    fprintf(stderr,"created socket\n");
  }

  // Next, create the socket addressing structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // this time 1st arg is port#

  // Next you need to BIND your socket.
  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    fprintf(stderr,"Failed to bind\n");
  }
  fprintf(stderr,"bound socket\n");

  // Right, now we are in the main server loop.
  // YOUR CODE HERE

  // Start by listening
  listen(sockfd, 5);
  fprintf(stderr,"listening\n");

  //while(1){

  // you need to accept the connection request
  int clientlen = sizeof(client);
  if(newsockfd = accept(sockfd, (struct sockaddr *) &client, &clientlen) < 0)
  {
    fprintf(stderr,"ERROR accepting\n");
  }
  if(newsockfd == -1)
  {
    fprintf(stderr,"Failed to accept connection\n");
  }
  else{
    fprintf(stderr,"accepted connection\n");
  }
  // the next call makes a new child process that will actually handle the client.
  id = fork();
  fprintf(stderr,"forked\n");
  // when id == 0, this is the child and needs to do the work for the server.
  if(id == 0)
  {
    data = read(newsockfd, buf, bufsize);
    //printf("Message: %s\n",buf);
    if(data > 0){
      data = write(newsockfd,"Message Recieved",18);
    }
    if (data < 0)
    {
      error("ERROR writing to socket");
      exit(0);
    }
  }

  // when if > 0, this is the parent, and it should just loop around,
  // when id < 0, we had an error.
  if(id < 0)
  {
    fprintf(stderr,"ERROR when trying to accept\n");
    exit(0);
  }

  // Your code here for the child process, that will read from the connection socket, process the data
  // write back to the socket, and then close and finally call exit(0) when done.

  // Note:  make sure the parent process (id > 0) does not execute this code, but immediately loops back
  // around (via the while) to get another connection request.

  //  }
}

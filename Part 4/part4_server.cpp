// part4_server.cpp: tcpServer
// Nathan Bush 007463099
// Base code provided by Dr. Bilal Khan
// CSUSB - CSE 4600(01) S22
// Compile: g++ -o part4_server part4_server.cpp
// Execute: ./part4_server.cpp

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <string>
#define MAX 80
#define SA struct sockaddr

char *server_name;

// function designed for chat between client and server
void openChat(int sockfd){
  char buff[MAX], clientName[MAX];
  int n;

  // infinite loop for chat
  for(;;){
    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    read(sockfd, buff, sizeof(buff));

    // print buffer which contains the client contents
    printf("From client: %s, \n %s: ", buff, server_name);
    bzero(buff, MAX);
    n = 0;

    // copy server message in the buffer
    while ((buff[n++] = getchar()) != '\n');

    // send buffer to client
    write(sockfd, buff, sizeof(buff));
    printf("Message sent. Waiting for client.\n");

    // if message containes "Exit" then server exit and chat ended.
    if(strncmp("exit", buff, 4) == 0) {
      printf("Server Exit...\n");
      break;
    }
  }
}

void* serve_port(void *data){
  int port = *((int*)data);
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;
  socklen_t addr_size;

   // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    printf("Socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created...\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port); // add cli arg port here

  // binding newly created socket to given IP and verification
  if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    printf("Socket %i bind failed...\n", port);
    exit(0);
  }
  else
    printf("Socket %i successfully binded...\n", port);

  // Now server is ready to listen and verification
  if((listen(sockfd, 5)) != 0){
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening on port %i...\n", port);
  len = sizeof(cli);

  // Accept the data packed from client and verification
  connfd = accept(sockfd, (SA*)&cli, &addr_size);
  if(connfd < 0) {
    printf("Server accept failed...\n");
    exit(0);
  }
  else
    printf("Server accepted the client...\n");

  // Open chat
  openChat(connfd);

  // After chatting close the socket
  close(sockfd);

  return nullptr;
}


// Driver function
int main(int argc, char*argv[])
{
  pthread_t threads[argc-1];
  char thread_buffer[100];
  server_name = argv[1];

  // create threads for each port passed in by argument
  for(int i = 2; i < argc; i++){
    int current_port = atoi(argv[i]);
    pthread_create(&threads[i-2], NULL, serve_port, (void *)&current_port);
    sleep(1);
  }

  // join threads
  for(int i = 0; i < (argc - 2); i++){
    pthread_join(threads[i], NULL);
  }

  return 0;
}
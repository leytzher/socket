#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){

  // create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock==-1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // define the server address
  struct sockaddr_in server_address;

  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(6666);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // server is in localhost
  //

  if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
    perror("Connect to server failed");
    exit(EXIT_FAILURE);
  }

  // send data to the server
  const char *message = "Hi Server, this is the client";
  if (send(sock, message, strlen(message),0) == -1){
    perror("Send failed");
    exit(EXIT_FAILURE);
  }

  // receive data from the server
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));

  if (recv(sock, buffer, sizeof(buffer) , 0)== -1) {
    perror("Receive failed");
    exit(EXIT_FAILURE);
  }

  printf("Server response %s\n", buffer);

  close(sock);
  return 0;

}

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>


int main() {

  // create a TCP socket
  int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

  // create a socketaddr struct to hold server address info. 
  struct sockaddr_in server_addr;

  // create a socketaddr struct to hold client address info.
  struct sockaddr_in client_addr;
  // we also need the client address length
  socklen_t client_addr_len = sizeof(client_addr);

  if (tcp_socket == 1) {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  // initialize address structs with zeros (both for client and server)
  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, client_addr_len);

  // fill up server struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(6666);  // convert hosts byte order to network byte order
  server_addr.sin_addr.s_addr = INADDR_ANY;  // binds socket to all available interfaces

  // Bind socket to address. We need to cast sockaddr_in to socketaddr struct
  if (bind(tcp_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  // start listening. The maximum number of pending connections is 5
  if (listen(tcp_socket, 5)==-1){
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  // accept connections from the client
  int client_socket = accept(tcp_socket, (struct sockaddr *) &client_addr, &client_addr_len);
  if (client_socket == -1) {
    perror("Accept failed.");
    exit(EXIT_FAILURE);
  }

  // Make a buffer to accept data from the client
  char buffer[1024];

  // initialize the memory for the buffer to zero
  memset(buffer, 0, sizeof(buffer));


  // receive data from the client
  ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
  printf("Bytes received %ld", bytes_received);

  if (bytes_received == -1) {
    perror("Receive failed");
    exit(EXIT_FAILURE);
  } else if (bytes_received == 0) {
    printf("Client disconnected");
  } else {
    printf("Received %ld bytes from client \n", bytes_received);
    printf("Message: %s\n", buffer);

    // send data back to the client
    const char *message = "Hello from the server";
    ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);

    if (bytes_sent == -1){
      perror("Send failed");
      exit(EXIT_FAILURE);
    } else {
      printf("Sent %ld bytes to client\n", bytes_sent);
    }
  }

  if (close(client_socket)== -1) {
    perror("Client close failed\n");
    exit(EXIT_FAILURE);
  }

  if (close(tcp_socket)== -1) {
    perror("Server close failed\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}

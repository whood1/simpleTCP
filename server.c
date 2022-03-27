#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {

  int socket_desc, client_sock;
  socklen_t size;
  struct sockaddr_in server_addr, client_addr;
  char server_message[2000], client_message[2000];

  memset(server_message, '\0', sizeof(server_message));
  memset(server_message, '\0', sizeof(server_message));

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
    printf("Socket option reuse failed");
  }
  
  printf("Socket options set\n");
  
  
  if(socket_desc < 0) {
    printf("Socket not created \n");
    return -1;
  }
  printf("Socket created \n");

  //Set Port and IP
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2002);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  //Bind to the Port and IP

  //printf("server_addr.sin_family: %d  \n", server_addr.sin_family);
  
  if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    printf("Binding failed \n");
    close(socket_desc);
    return -1;
  }

  printf("Bound Port and IP \n");

  //Listen for Client
  if(listen(socket_desc, 1) < 0){
    printf("No client found \n");
    return -1;
  }

  printf("Listening for clients \n");

  //Accept Client
  size = sizeof(client_addr);
  client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &size);

  if (client_sock < 0) {
    printf("Cant accept client \n");
    return -1;
  }
  printf("Client Accepted \n");

  //Get Client Message
  if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
    printf("Message Not Received \n");
    return -1;
  }
  printf("Client Message: %s\n", client_message);

  //Send Server Message
  strcpy(server_message, "Message from server. \n");
  
  if (send(client_sock, server_message, strlen(server_message), 0) < 0) {
    printf("Server Message not sent \n");
    return -1;
  }
  
  //Close socket
  printf("Closing Sockets \n");
  close(client_sock);
  close(socket_desc);
  printf("Sockets Closed \n");
  
  return 0;
}

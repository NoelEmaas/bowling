#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "memory.h"

#define SHM_NAME "socketchat"
#define SIZE 1024

// Wrapper function for error handling
#define TRY(expr, condition, error_msg) \
  do { \
    if ((expr) condition) { perror(error_msg); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)


// prototypes
void receive_message(int client_socket);
void send_message(int client_socket);
int init_shared_memory();
int open_shared_memory();

int main(int argc, char* argv[]) {
  // Check if the passed arguments is valid
  TRY(argc, < 3, "Usage: ./client host_name port_number\n");
  char* host_name = argv[1];
  int port_number = atoi(argv[2]);
 
  // create a socket
  printf("Client starting ...\n");
  int client_socket;
  TRY(client_socket = socket(AF_INET, SOCK_STREAM, 0), < 0, "Error Creating socket!\n");
  printf("Socket created successfully!\n");
  
  // find host
  printf("Looking for host %s ...\n", host_name);
  struct hostent* server;
  TRY(server = gethostbyname(host_name), == NULL, "Host not found!\n");
  printf("Host found!\n");
  
  // establish connection to server
  struct sockaddr_in server_addr;
  memset((char*)&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port_number);
  memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  
  // try to connect to the server
  TRY(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), < 0, "Connection Failed!\n");
  printf("Connected to Server!\n\n");
  printf("Start chatting:\n\n");

  // Open the shared memory object
  int shm_fd = open_shared_memory();

  void *shm_ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  // send and receive message to and from server
  int pid = fork();
  while (1) {
    if (pid == 0) send_message(client_socket);
    else receive_message(client_socket);
  }

  printf("Closing connection ...\n");
  close(client_socket);
  
  return 0;
}


void receive_message(int client_socket) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // receiving message from server
    int bytes_received;
    TRY(bytes_received = recv(client_socket, buffer, 255, 0), < 0, "Receiving Failed!\n");

    // move cursor to the beginning of the line and clear the line
    printf("\033[1G\033[2K");

    // display the received message
    printf("[server] > %s", buffer);

    // prompt for a new input
    printf(">> ");
    fflush(stdout);
}

void send_message(int client_socket) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // move cursor to the beginning of the line and clear the line
    printf("\033[1G\033[2K");

    // prompt for user input
    printf(">> ");
    fgets(buffer, 255, stdin);

    // sending message to server
    int bytes_sent;
    TRY(bytes_sent = send(client_socket, buffer, strlen(buffer), 0), < 0, "Sending Failed!\n");
}

int init_shared_memory() {
    // Create the shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    // Configure size of shared memory
    ftruncate(shm_fd, SIZE);
    return shm_fd;
}

int open_shared_memory() {
    // Open the shared memory object
    return shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
}
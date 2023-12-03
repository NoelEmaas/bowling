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

int main (int argc, char* argv[]) {
  // Check if the passed arguments is valid
  TRY(argc, < 2, "Usage: ./server port_number\n");
  int port_number = atoi(argv[1]);

  // create a socket
  printf("Server starting ...\n");
  int server_socket;
  TRY(server_socket = socket(AF_INET, SOCK_STREAM, 0), < 0, "Error creating socket!\n");
  printf("Socket created successfully!\n");
  
  // configure the socket's address information
  struct sockaddr_in server_addr;
  memset((char*)&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port_number);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  
  // binds the socket to the configured server address
  TRY(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), < 0, "Binding Failed!\n");
  printf("Socket binded successfully!\n");
  
  // server listens and can queue up to 10 requests
  listen(server_socket, 5);
  printf("Server Listening to port %d\n", port_number);

  // accept client connection 
  int client_socket, client_size;
  struct sockaddr_in client_addr;
  client_size = sizeof(client_addr);

  // try to connect to client
  TRY(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size), < 0, "Accepting Failed!\n");
  printf("Client Connected Successfully!\n\n");
  printf("Start Chatting: \n\n");

  // Create shared memory
  int shm_fd = init_shared_memory();

  // Map the shared memory object on memory
  void *shm_ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  // send and receive message to and from client
  int pid = fork();
  while(1) {
    if (pid == 0) {
      send_message(client_socket);
    } else {
      receive_message(client_socket);
    }
  }

  printf("Closing connection ...\n");
  close(client_socket);
  close(server_socket);

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
    printf("[client] > %s", buffer);

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
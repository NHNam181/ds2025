#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" 
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    address.sin_family = AF_INET;
    address.sin_port = htons(SERVER_PORT);

    // Convert from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &address.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Send file
    FILE *file = fopen("man.txt", "rb");
    if (file == NULL) {
        perror("File open failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }
    printf("File sent successfully.\n");

    fclose(file);

    // Close the socket
    close(client_socket);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int sockfd;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE];

void send_message(const char *target_id, const char *message) {
    if (target_id) {
        snprintf(buffer, sizeof(buffer), "%s %s", target_id, message);
    } else {
        snprintf(buffer, sizeof(buffer), "%s", message);
    }
    
    write(sockfd, buffer, strlen(buffer));
}

void *receive_messages(void *arg) {
    char received_msg[BUFFER_SIZE];
    while (1) {
        memset(received_msg, 0, sizeof(received_msg));
        int bytes_received = read(sockfd, received_msg, sizeof(received_msg) - 1);
        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        received_msg[bytes_received] = '\0';
        printf("%s\n", received_msg);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    char *target_id = NULL;
    if (argc == 3 && strcmp(argv[1], "-p") == 0) {
        target_id = argv[2];
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer));
    printf("%s", buffer);

    pthread_t receiver_thread;
    pthread_create(&receiver_thread, NULL, receive_messages, NULL);

    while (1) {
        char message[BUFFER_SIZE];
        
        if (target_id) {
            printf("Send message to ID %s: ", target_id);
        } else {
            printf("Global chat: ");
        }

        fgets(message, sizeof(message), stdin);
        sscanf(message, "%[^\n]", message);

        send_message(target_id, message);
    }

    close(sockfd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_USERS 100
#define BUFFER_SIZE 1024

typedef struct {
    int socket_fd;
    char id[10];
} User;

User users[MAX_USERS];
int user_count = 0;
pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;

void *server_thread(void *arg);
void send_message_to_user(const char *receiver_id, const char *message, const char *sender_id);
int find_user_by_id(const char *id);

void *server_thread(void *arg) {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        if ((new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&users_mutex);
        if (user_count < MAX_USERS) {
            User new_user;
            new_user.socket_fd = new_sock;
            snprintf(new_user.id, sizeof(new_user.id), "%d", user_count + 1);
            users[user_count++] = new_user;
            printf("User %s connected.\n", new_user.id);
        }
        pthread_mutex_unlock(&users_mutex);

        char id_message[50];
        snprintf(id_message, sizeof(id_message), "Your ID: %s\n", users[user_count - 1].id);
        write(new_sock, id_message, strlen(id_message));

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int read_size = read(new_sock, buffer, sizeof(buffer));
            if (read_size <= 0) {
                if (read_size == 0) {
                    printf("User %s disconnected\n", users[user_count - 1].id);
                } else {
                    perror("Read failed");
                }
                break;
            }

            char receiver_id[10], message[BUFFER_SIZE];
            if (sscanf(buffer, "%s %[^\n]", receiver_id, message) == 2) {
                send_message_to_user(receiver_id, message, users[user_count - 1].id);
            } else {
                send_message_to_user("ALL", buffer, users[user_count - 1].id);
            }
        }
        close(new_sock);
    }

    close(server_fd);
    return NULL;
}

int find_user_by_id(const char *id) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void send_message_to_user(const char *receiver_id, const char *message, const char *sender_id) {
    if (strcmp(receiver_id, "ALL") == 0) {
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].id, sender_id) != 0) {
                char formatted_message[BUFFER_SIZE];
                snprintf(formatted_message, sizeof(formatted_message), "[%s] : %s", sender_id, message);
                write(users[i].socket_fd, formatted_message, strlen(formatted_message));
            }
        }
    } else {
        int user_index = find_user_by_id(receiver_id);
        if (user_index != -1) {
            char formatted_message[BUFFER_SIZE];
            snprintf(formatted_message, sizeof(formatted_message), "[%s] : %s", sender_id, message);
            write(users[user_index].socket_fd, formatted_message, strlen(formatted_message));
        } else {
            printf("User with ID %s not found.\n", receiver_id);
        }
    }
}

int main() {
    pthread_t server;
    pthread_create(&server, NULL, server_thread, NULL);
    pthread_join(server, NULL);
    return 0;
}

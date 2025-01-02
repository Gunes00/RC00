// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *client_socket);
void broadcast_message(char *message, int sender_socket);
void remove_client(int client_socket);
void add_client(int client_socket);

int main() {
    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket oluşturulamadı");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bağlama hatası");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Dinleme hatası");
        exit(1);
    }

    printf("Server başlatıldı, port %d dinleniyor...\n", PORT);

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

        if (client_socket < 0) {
            perror("Client kabul edilemedi");
            continue;
        }

        printf("Yeni client bağlandı: %d\n", client_socket);

        add_client(client_socket);

        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    close(server_socket);
    return 0;
}

void *handle_client(void *client_socket) {
    int socket = *(int *)client_socket;
    char buffer[1024];
    int read_size;

    while ((read_size = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        broadcast_message(buffer, socket);
    }

    if (read_size == 0) {
        printf("Client bağlantısı kesildi: %d\n", socket);
        fflush(stdout);
    } else if (read_size == -1) {
        perror("Recv hatası");
    }

    remove_client(socket);
    close(socket);
    return NULL;
}

void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_socket) {
            if (send(client_sockets[i], message, strlen(message), 0) == -1) {
                perror("Mesaj gönderilemedi");
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int client_socket) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = 0;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void add_client(int client_socket) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == 0) {
            client_sockets[i] = client_socket;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

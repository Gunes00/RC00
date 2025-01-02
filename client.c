#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 4444

int socket_desc;

void *receive_message(void *socket_desc);
void send_message(char *nickname);

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    pthread_t recv_thread;

    if (argc != 2) {
        printf("Kullanım: ./client <nickname>\n");
        return -1;
    }

    char *nickname = argv[1];

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror("Socket oluşturulamadı");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("45.87.173.173");

    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sunucuya bağlanılamadı");
        return -1;
    }

    printf("Sunucuya bağlanıldı. %s ile sohbet edebilirsiniz.\n", nickname);

    pthread_create(&recv_thread, NULL, receive_message, (void *)&socket_desc);

    while (1) {
        send_message(nickname);
    }

    close(socket_desc);
    return 0;
}

void *receive_message(void *socket_desc) {
    int socket = *(int *)socket_desc;
    char buffer[1024];

    while (1) {
        int read_size = recv(socket, buffer, sizeof(buffer), 0);
        if (read_size > 0) {
            buffer[read_size] = '\0';
            printf("%s\n", buffer);
        } else if (read_size == 0) {
            printf("Bağlantı kapatıldı.\n");
            break;
        } else {
            perror("Mesaj alınamadı");
            break;
        }
    }

    return NULL;
}

void send_message(char *nickname) {
    char message[1024];

    fgets(message, sizeof(message), stdin);
    message[strlen(message) - 1] = '\0';

    char full_message[1048];
    snprintf(full_message, sizeof(full_message), "[%s]: %s", nickname, message);

    if (send(socket_desc, full_message, strlen(full_message), 0) == -1) {
        perror("Mesaj gönderilemedi");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define MAX_MSG_SIZE 256
#define PORT 3000

int main(int argc, char* argv[])
{
    if (argc != 2) {
        perror("Must enter IP address to send to");
        exit(1);
    }

    int connfd;
    struct sockaddr_in servaddr;
    char sendmsg[MAX_MSG_SIZE], recvmsg[MAX_MSG_SIZE];

    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Unable to create socket");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(PORT);

    if (connect(connfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Unable to connect to server");
        exit(1);
    }

    printf("%s", "Connected to the socket. Type your message: ");

    while (fgets(sendmsg, MAX_MSG_SIZE, stdin) != NULL) {
        send(connfd, sendmsg, strlen(sendmsg), 0);

        if (recv(connfd, recvmsg, MAX_MSG_SIZE, 0) == 0) {
            perror("The server terminated");
            exit(1);
        }

        printf("%s", "String received from the server: ");
        fputs(recvmsg, stdout);
        memset(recvmsg, '\0', sizeof(char)*MAX_MSG_SIZE);
    }

    close(connfd);
}

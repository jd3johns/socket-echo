#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 3000
#define MAX_MSG_SIZE 256
#define LISTEN_QUEUE 4

int main()
{
    int listenfd, connfd, n;
    socklen_t clilen;
    char msg[MAX_MSG_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // Create server address structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTEN_QUEUE);

    printf("%s\n", "Server running. Waiting for connections...");

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n", "Received request");

        while ((n = recv(connfd, msg, MAX_MSG_SIZE, 0)) > 0) {
            printf("%s", "String received from and resent to the client: ");
            fputs(msg, stdout);
            send(connfd, msg, n, 0);
            memset(msg, '\0', sizeof(char)*MAX_MSG_SIZE);
        }

        if (n < 0) {
            perror("Read error");
            exit(1);
        }
        close(connfd);
    }

    close(listenfd);
    exit(0);
}

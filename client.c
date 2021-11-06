/*

Client is a simple user interface to get the data
and send it to the server by socket.

Every client can do the following commands:
    1. Start
    2. Ping 
    3. Stop

*/
// socket libraries
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    // TODO: How does socket create?
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // TODO: What is memory cell?
    memset(&serv_addr, '0', sizeof(serv_addr));

    // TODO: What is address family
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    // TODO: Why do we convert IPv4 and IPv6 to binary
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // User commands
    int valread;
    char buffer[1024] = {0};
    char command[50];

    while (1)
    {
        printf("> ");
        gets(command);

        send(sock, command, strlen(command), 0);

        if (strcmp(command, "stop") == 0)
        {
            printf("Disconnected\n");
            break;
        }

        valread = read(sock, buffer, sizeof(buffer));

        if (valread < 0)
        {
            perror("Reading failed");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", buffer);
    }
    
    return 0;
}
/* 

Using sockets to connect to our server.

Our server which will accept clients and give response to them.
Our main input command are: 
    1. Start [name]
    2. Ping [server] | [user] 
    3. Stop
Server responses:
    1. Init user
    2. Pong [status]
    3. Release client

*/

// Socket libraries
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//
#include <string.h>
#include <ctype.h>
#include <time.h>
// Thread library
#include <pthread.h>

void *client_handler(void *vargp)
{
    int *temp = (int *)vargp;
    int client_socket = *temp;

    clock_t begin = clock();

    int valread;
    char buffer[1024] = {0};
    char response[1024] = {0};

    while (1)
    {
        valread = read(client_socket, buffer, sizeof(buffer));
        if (valread < 0)
        {
            perror("Empty read");
            exit(EXIT_FAILURE);
        }

        buffer[valread] = '\0';

        if (strcmp(buffer, "stop") == 0)
        {
            printf("Client %d: disconnected\n", client_socket);
            break;
        }

        double time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
        snprintf(response, sizeof(response), "Online time %f", time_spent * 1000);

        send(client_socket, response, sizeof(response), 0);

        fflush(stdout);
        buffer[0] = '\0';
        response[0] = '\0';
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    // TODO: What is a socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // TODO: What is this do
    if (server_fd == 0)
    {
        perror("Socket faild");
        exit(EXIT_FAILURE);
    }

    // TODO: What the fuck are these??
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));
    const int addrlen = sizeof(address);

    // TODO: What is binding??
    if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // TODO: What is backlog?
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen faild");
        exit(EXIT_FAILURE);
    }

    printf("Listening on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // Accepting client
    while (1)
    {
        int client_socket;
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept faild");
            exit(EXIT_FAILURE);
        }

        printf("Accepted client %s:%d id:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port), client_socket);

        // Using thread to handle the client
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_handler, (void *)&client_socket);
    }

    return 0;
}

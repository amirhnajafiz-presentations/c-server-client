# C-ServerClient

<br />

Creating a server-client application with C socket.

<p align="center">
  <img src="https://www.codeproject.com/KB/IP/586000/network4_thumb_4_.png" width="500" />
</p>

## How to use?
Clone the project and cd in to the main directory.<br />
Open a terminal and setup your server with the following command:
```shell
./server [port]
```

You should get a result like this:
```shell
Listening on 0.0.0.0:8080
```

After that connect to the server with your client using this command:
```shell
./client [IP] [port]
```

## Socket programming
Socket programming is a way of connecting two nodes on a network to communicate with each other. One socket(node) listens on a particular port at an IP, while other socket reaches out to the other to form a connection. Server forms the listener socket while client reaches out to the server.

### Creating the server socket
```c
int sockfd = socket(domain, type, protocol)
```

- sockfd: socket descriptor, an integer (like a file-handle)
- domain: integer, communication domain e.g., AF_INET (IPv4 protocol) , AF_INET6 (IPv6 protocol)
- type: communication type
  - SOCK_STREAM: TCP(reliable, connection oriented)
  - SOCK_DGRAM: UDP(unreliable, connectionless)
- protocol: Protocol value for Internet Protocol(IP), which is 0. This is the same number which appears on protocol field in the IP header of a packet.(man protocols for more details)

### Binding the socket
```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

After creation of the socket, bind function binds the socket to the address and port number specified in addr(custom data structure). In the example code, we bind the server to the localhost, hence we use INADDR_ANY to specify the IP address.

### Set the server to listen 
```c
int listen(int sockfd, int backlog);
```

It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED.

### Accepting clients
```c
int new_socket = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.


## Client connecting
```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

The connect() system call connects the socket referred to by the file descriptor sockfd to the address specified by addr. Server’s address and port is specified in addr.

## Multi-client server
```c
pthread_t thread_id;
pthread_create(&thread_id, NULL, client_handler, (void *)&client_socket);
```

Using pthread to create a thread for each of our clients and handle them with a specific method.

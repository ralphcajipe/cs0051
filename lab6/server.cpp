/*
 * Filename: server.cpp
 * Author: Ralph Cajipe
 * Date: March 8, 2023
 * Description: A C++ server that accepts two numbers from a client, computes their sum,
 *              and sends the result back to the client.
 *
 * This program listens on port 8080 for incoming client connections. When a client connects,
 * it spawns a new thread to handle the connection and continues listening for new connections.
 * The server uses a detached thread to handle each client connection, which allows it to handle
 * multiple connections simultaneously. The program uses the setsockopt function to enable reuse
 * of the server address, which allows the server to quickly restart if it crashes or is terminated
 * unexpectedly. The program also uses the shutdown function to close the listening socket before
 * exiting, which ensures that all client connections are properly closed before the program terminates.
 */

// Contains constants and structures needed for internet domain addresses.
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <vector>

#define PORT 8080


/**
 * It reads two numbers from the client, computes their sum, and sends the result back to the client
 *
 * @param new_socket The socket descriptor for the accepted connection.
 */
void acceptClient(int new_socket)
{
    int valread;
    char buffer[1024] = {0};
    const char *hello = "Hello from server";

    valread = read(new_socket, buffer, 1024);

    // Compute the sum of the two numbers received from the client
    int num1, num2;
    sscanf(buffer, "%d%d", &num1, &num2);
    int sum = num1 + num2;
    char result[1024];
    sprintf(result, "%d", sum);

    // Send the result back to the client
    send(new_socket, result, strlen(result), 0);
    printf("Result sent to the client: %d\n", sum);

    // Closing the connected socket
    close(new_socket);
}


/**
 * AcceptClient() is a function that is called asynchronously by a thread. It accepts a client
 * connection, reads the client's message, and sends a response back to the client
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The command line arguments passed to the program.
 *
 * @return The server is returning the number of bytes sent.
 */
int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;

    // `opt` is a variable that is used to set the socket options.
    int opt = 1;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // This is a function that prints a message to the standard error output stream.
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // This is a constant that specifies the address family.
    address.sin_family = AF_INET;

    // A special address that tells the server to accept connections from any network interface.
    address.sin_addr.s_addr = INADDR_ANY;

     // Converts the unsigned short integer hostshort from host byte order to network byte order.
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /* This is a function that marks the socket referred to by the file descriptor `server_fd` as a
    passive socket, that is, as a socket that will be used to accept incoming connection requests
    using `accept`. The second argument, `3`, is the backlog, which specifies the maximum number of
    connections that can be queued for this socket. */
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Printing the message "Server running..." and "Accepting Client connection..." to the console. */
    printf("Server running...\n");
    printf("Accepting Client connection...\n");

    std::vector<std::thread> threads;
    while (true)
    {
        // Accept a new client connection
        int new_socket;
        struct sockaddr_in client_address;
        int addrlen = sizeof(client_address);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Launch acceptClient() as an ASYNCHRONOUS thread
        threads.emplace_back(acceptClient, new_socket);

        // Join finished threads
        for (auto it = threads.begin(); it != threads.end();)
        {
            if (it->joinable())
            {
                it->join();
                it = threads.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    // Closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
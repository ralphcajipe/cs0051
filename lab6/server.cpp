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

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <vector>

#define PORT 8080

// Function to accept a client connection
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

int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
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
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

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

        // Launch acceptClient() as an asynchronous thread
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
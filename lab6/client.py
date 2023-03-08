"""
Filename: client.py
Author: Ralph Cajipe
Date: March 8, 2023
Description: A Python client that prompts the user for two numbers, sends them to a server,
             and receives the sum of the numbers from the server.

This program connects to a server running on localhost port 8080. It prompts the user to enter
two numbers, which are sent to the server as a single string. The program uses a socket to send
and receive data to and from the server. The program includes error handling to catch common
exceptions that can occur when connecting to or communicating with the server. The program uses
a try-finally block to ensure that the socket is always closed, even if an exception occurs.
"""

import socket

try:
    # Connect to the server
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(("localhost", 8080))

    # Prompt the user for two numbers
    while True:
        try:
            num1 = int(input("Enter the first number: "))
            break
        except ValueError:
            print("Invalid input, please enter an integer.")
    while True:
        try:
            num2 = int(input("Enter the second number: "))
            break
        except ValueError:
            print("Invalid input, please enter an integer.")

    # Send the numbers to the server
    sock.sendall(f"{num1} {num2}".encode())

    # Receive the result from the server
    result = sock.recv(1024).decode().strip()
    print(f"Result received from the server: {result}")

except ConnectionRefusedError:
    print("Failed to connect to the server.")
except OSError:
    print("Error occurred while communicating with the server.")
finally:
    # Close the socket
    if "sock" in locals():
        sock.close()

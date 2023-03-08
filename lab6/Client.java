/*
 * Filename: Client.java
 * Author: Ralph Cajipe
 * Date: March 8, 2023
 * Description: A Java client that prompts the user for two numbers, sends them to a server,
 *              and receives the sum of the numbers from the server.
 *
 * This program connects to a server running on localhost port 8080. It prompts the user to enter
 * two numbers, which are sent to the server as a single string. The program uses a PrintWriter to
 * send the numbers to the server and a BufferedReader to receive the result from the server. The
 * program also includes error handling to catch common exceptions that can occur when connecting
 * to or communicating with the server. The program uses try-with-resources to automatically close
 * the socket and other resources when they are no longer needed.
 *
 * The functions are:
 *    main()
 *
 * The variables are:
 *   socket
 *   input
 *   num1
 *   num2
 *   output
 *   writer
 *   inputFromServer
 *   reader
 *   result
 */


// Importing the java.io package, which contains the BufferedReader and PrintWriter classes.
import java.io.*;

// Importing the java.net package, which contains the Socket class.
import java.net.*;

public class Client {
    public static void main(String[] args) {
        try {
            // Connect to the server
            Socket socket = new Socket("localhost", 8080);

            // Prompt the user for two numbers
            BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
            int num1 = 0, num2 = 0;
            while (true) {
                try {
                    System.out.print("Enter the first number: ");
                    num1 = Integer.parseInt(input.readLine());
                    break;
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input, please enter an integer.");
                }
            }
            while (true) {
                try {
                    System.out.print("Enter the second number: ");
                    num2 = Integer.parseInt(input.readLine());
                    break;
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input, please enter an integer.");
                }
            }

            // Send the numbers to the server
            OutputStream output = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(output, true);
            writer.println(num1 + " " + num2);

            // Receive the result from the server
            InputStream inputFromServer = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(inputFromServer));
            String result = reader.readLine();
            System.out.println("Result received from the server: " + result);

            // Close the socket
            socket.close();
        } catch (ConnectException e) {
            System.out.println("Failed to connect to the server.");
        } catch (IOException e) {
            System.out.println("Error occurred while communicating with the server.");
        }
    }
}

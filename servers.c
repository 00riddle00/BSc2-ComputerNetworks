#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for close
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

void send_msg(char* sender_name, char* client_message, int server_port);

int main() {

    char welcome_msg[256] = "You have reached the ";

    /* SERVER FIRST ------------------------------------------------------------- */

    /* ----- ACT AS A SERVER ------- */

    char server1_name[256] = "server01";

    // create the server socket
    int server1_socket;
    server1_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in server1_address;
    server1_address.sin_family = AF_INET;
    server1_address.sin_port = htons(10001);
    server1_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // bind the socket to our specified IP and port
    bind(server1_socket, (struct sockaddr*) &server1_address, sizeof(server1_address));

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, but doesn't matter

    listen(server1_socket, 5);

    int client1_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    // DOES NOT PROCEED FURTHER UNTIL IT GETS A CONNECTION
    printf("here0\n");
    client1_socket = accept(server1_socket, NULL, NULL);
    printf("here1\n");

    // print the welcoming message
    printf("[%s] %s%s!\n", server1_name, welcome_msg, server1_name);

    // receive data from the client
    char client1_message[256];
    recv(client1_socket, &client1_message, sizeof(client1_message), 0);

    // print out the client's msg
    printf("[%s] The client's message is: %s\n", server1_name, client1_message);

	// convert client msg to upper case
	int i = 0;
    while(client1_message[i]) {
      client1_message[i] = toupper(client1_message[i]);
      i++;
    }

    printf("[%s] The client's modified message is: %s\n", server1_name, client1_message);
    close(server1_socket);




    /* INTERMEDIARY SERVERS ------------------------------------------------------ */

    for (i = 2; i <= 10; i++) {

        char server_current_name[256] = "server0";
        char server_prev_name[256] = "server0";
        sprintf(server_current_name, "%s%d", server_current_name, (char)i);
        sprintf(server_prev_name, "%s%d", server_prev_name, (char)i-1);

        // create the server socket
        int server_socket;
        server_socket = socket(AF_INET, SOCK_STREAM, 0);

        // define the server address
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(10000+i);
        server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        // bind the socket to our specified IP and port
        bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

        // 2nd arg - backlog: how many connections can be waiting for this socket.
        // Set 5, bet doesn't matter
        listen(server_socket, 5);

        int client_socket;
        // 2nd param - struct that contains address of the client connection,
        // 3rd - sizeof it. We'll leave it at NULL
        printf("here21\n");

        // ####################################################

        send_msg(server_prev_name, client1_message, 10000+i);

        // ####################################################

        client_socket = accept(server_socket, NULL, NULL);
        printf("here22\n");

        // print the welcoming message
        printf("[%s] %s%s!\n", server_current_name, welcome_msg, server_current_name);

        // receive data from the client
        char client2_message[256];
        recv(client_socket, &client2_message, sizeof(client2_message), 0);

        // print out the client's msg
        printf("[%s] The received client's message is: %s\n", server_current_name, client1_message);

        if (i == 10) {

            char modified_client1_message[256];
            // convert client
            int j = 0;
            int k = 0;
            while(client1_message[j]) {
                modified_client1_message[k++] = client1_message[j];
                modified_client1_message[k++] = client1_message[j++];
            }

            printf("[%s] The client's modified message is: %s\n", server_current_name, modified_client1_message);

            send_msg(server_current_name, modified_client1_message, 10000);
        }

        close(server_socket);
    }

    return 0;
}

void send_msg(char* sender_name, char* client_message, int server_port) {

    // create a socket
    int client_socket;

    // protocol = 0 (default: TCP)
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;

    // convert integer to network byte order
    server_address.sin_port = htons(server_port);

    // sin_addr - a struct that contains another struct
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // cast server_address to different structure
    int connection_status = connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    // check for error with the connection
    // 0 for no errors
    if (connection_status == -1) {
        printf("[%s] There was an error making a connection to the remote socket \n\n", sender_name);
    }
    // send data to the server
    send(client_socket, client_message, sizeof(client_message), 0);

    // and then close the socket
    close(client_socket);
}


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

void waitFor (unsigned int secs);

int main() {

    // global vars
    char welcome_msg[256] = "You have reached the ";
    int i,j;

    /* SERVER FIRST ------------------------------------------------------------- */

    /* ----- ACT AS A SERVER ------- */

    char server_first_name[256] = "server01";

    // create the server socket
    int server_first_socket;
    server_first_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in server_first_address;
    server_first_address.sin_family = AF_INET;
    server_first_address.sin_port = htons(10001);
    server_first_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // bind the socket to our specified IP and port
    bind(server_first_socket, (struct sockaddr*) &server_first_address, sizeof(server_first_address));

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, bet doesn't matter
    listen(server_first_socket, 5);

    int client_first_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    client_first_socket = accept(server_first_socket, NULL, NULL);

    // print the welcoming message
    printf("[%s] %s%s!\n", server_first_name, welcome_msg, server_first_name);

    // receive data from the client
    char client_first_message[256];
    recv(client_first_socket, &client_first_message, sizeof(client_first_message), 0);

    // print out the client's msg
    printf("[%s] The client's message is: %s\n", server_first_name, client_first_message);

	// convert client msg to upper case
	i = 0;
    while(client_first_message[i]) {
      client_first_message[i] = toupper(client_first_message[i]);
      i++;
    }

    printf("[%s] The client's modified message is: %s\n", server_first_name, client_first_message);
    close(server_first_socket);

    /* ----- ACT AS A CLIENT ------- */

    // TODO




    /* INTERMEDIARY SERVERS ------------------------------------------------------ */



    /* SERVER LAST ------------------------------------------------------------- */

    /* ----- ACT AS A SERVER ------- */

    char server_last_name[256] = "server LAST";

    // create the server socket
    int server_last_socket;
    server_last_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in server_last_address;
    server_last_address.sin_family = AF_INET;
    server_last_address.sin_port = htons(10010);
    server_last_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // bind the socket to our specified IP and port
    bind(server_last_socket, (struct sockaddr*) &server_last_address, sizeof(server_last_address));

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, bet doesn't matter
    listen(server_last_socket, 5);

    int client_last_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    client_last_socket = accept(server_last_socket, NULL, NULL);

    // print the welcoming message
    printf("[%s] %s%s [Last one]!\n", server_last_name, welcome_msg, server_last_name);

    // receive data from the client
    char client_last_message[256];
    recv(client_last_socket, &client_last_message, sizeof(client_last_message), 0);

    // print out the client's msg
    printf("[%s] The received client's message is: %s\n", server_last_name, client_last_message);

    char modified_client_last_message[256];
	// convert client
    i = 0;
    j = 0;
    while(client_last_message[i]) {
      modified_client_last_message[j++] = client_last_message[i];
      modified_client_last_message[j++] = client_last_message[i++];
    }

    printf("[%s] The client's modified message is: %s\n", server_last_name, modified_client_last_message);

    close(server_last_socket);

    /* ----- ACT AS A CLIENT ------- */

    // TODO







    /* -------------------------------------------------------------------- */

    return 0;
}



void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

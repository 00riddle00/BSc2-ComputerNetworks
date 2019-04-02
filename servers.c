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

    char in_server1_name[256] = "server01";

    // create the server socket
    int in_server1_socket;
    in_server1_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in in_server1_address;
    in_server1_address.sin_family = AF_INET;
    in_server1_address.sin_port = htons(10001);
    in_server1_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // bind the socket to our specified IP and port
    bind(in_server1_socket, (struct sockaddr*) &in_server1_address, sizeof(in_server1_address));

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, but doesn't matter
    listen(in_server1_socket, 5);

    int in_client1_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    in_client1_socket = accept(in_server1_socket, NULL, NULL);

    // print the welcoming message
    printf("[%s] %s%s!\n", in_server1_name, welcome_msg, in_server1_name);

    // receive data from the client
    char in_client1_message[256];
    recv(in_client1_socket, &in_client1_message, sizeof(in_client1_message), 0);

    // print out the client's msg
    printf("[%s] The client's message is: %s\n", in_server1_name, in_client1_message);

	// convert client msg to upper case
	i = 0;
    while(in_client1_message[i]) {
      in_client1_message[i] = toupper(in_client1_message[i]);
      i++;
    }

    printf("[%s] The client's modified message is: %s\n", in_server1_name, in_client1_message);
    close(in_server1_socket);

    /* ----- ACT AS A CLIENT ------- */

    // TODO




    /* INTERMEDIARY SERVERS ------------------------------------------------------ */



    /* SERVER LAST ------------------------------------------------------------- */

    /* ----- ACT AS A SERVER ------- */

    // serverL = server last
    char in_serverL_name[256] = "server LAST";

    // create the server socket
    int in_serverL_socket;
    in_serverL_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in in_serverL_address;
    in_serverL_address.sin_family = AF_INET;
    in_serverL_address.sin_port = htons(10010);
    in_serverL_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // bind the socket to our specified IP and port
    bind(in_serverL_socket, (struct sockaddr*) &in_serverL_address, sizeof(in_serverL_address));

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, bet doesn't matter
    listen(in_serverL_socket, 5);

    // in_clientL = client last
    int in_clientL_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    in_clientL_socket = accept(in_serverL_socket, NULL, NULL);

    // print the welcoming message
    printf("[%s] %s%s [Last one]!\n", in_serverL_name, welcome_msg, in_serverL_name);

    // receive data from the client
    char in_clientL_message[256];
    recv(in_clientL_socket, &in_clientL_message, sizeof(in_clientL_message), 0);

    // print out the client's msg
    printf("[%s] The received client's message is: %s\n", in_serverL_name, in_clientL_message);

    char modified_in_clientL_message[256];
	// convert client
    i = 0;
    j = 0;
    while(in_clientL_message[i]) {
      modified_in_clientL_message[j++] = in_clientL_message[i];
      modified_in_clientL_message[j++] = in_clientL_message[i++];
    }

    printf("[%s] The client's modified message is: %s\n", in_serverL_name, modified_in_clientL_message);

    close(in_serverL_socket);

    /* ----- ACT AS A CLIENT ------- */

    // TODO







    /* -------------------------------------------------------------------- */

    return 0;
}



void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

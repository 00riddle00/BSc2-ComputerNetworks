#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for close

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <time.h>

void waitFor (unsigned int secs);

int main() {

    char welcome_msg[256] = "You have reached the ";
    char server_name[256] = "client-server";

    /* ----- ACT AS A CLIENT --------------------------------------------------- */

    // create a socket
    int out_client_socket;

    // protocol = 0 (default: TCP)
    out_client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in out_server_address;
    out_server_address.sin_family = AF_INET;

    // convert integer to network byte order
    out_server_address.sin_port = htons(10001);

    // sin_addr - a struct that contains another struct
    out_server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // cast server_address to different structure
    int connection_status = connect(out_client_socket, (struct sockaddr *) &out_server_address, sizeof(out_server_address));

    // check for error with the connection
    // 0 for no errors
    if (connection_status == -1) {
        printf("[%s] There was an error making a connection to the remote socket \n\n", server_name);
    }

    char out_client_message[256] = "hello";

    // send data to the server
    send(out_client_socket, out_client_message, sizeof(out_client_message), 0);

    // and then close the socket
    close(out_client_socket);

    /* ----- ACT AS A SERVER --------------------------------------------------- */

//    waitFor(1);

    // create the server socket
    int in_server_socket;
    in_server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in in_server_address;
    in_server_address.sin_family = AF_INET;
    in_server_address.sin_port = htons(10000);
    in_server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // bind the socket to our specified IP and port
    bind(in_server_socket, (struct sockaddr*) &in_server_address, sizeof(in_server_address));

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, but doesn't matter
    listen(in_server_socket, 5);

    int in_client_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    printf("[0]here0\n");
    in_client_socket = accept(in_server_socket, NULL, NULL);
    printf("[0]here1\n");

    // print the welcoming message
    printf("[%s] %s%s!\n", server_name, welcome_msg, server_name);

    // receive data from the client
    char in_client_message[256];
    recv(in_client_socket, &in_client_message, sizeof(in_client_message), 0);

    // print out the client's msg, which is the original message modified
    printf("[%s] The modified message that reached the client: %s\n", server_name, in_client_message);

    close(in_server_socket);

    /* -------------------------------------------------------------------- */

    return 0;
}


void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

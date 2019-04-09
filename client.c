#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for close
#include <unistd.h> // for close

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {

    char welcome_msg[256] = "You have reached the ";
    char server_name[256] = "client-server";

    int hops;

    while (1) {
        printf("[Enter the number of hops to traverse]> ");
        if (scanf("%d", &hops) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        } else if (hops < 0 || hops > 9) {
            printf("The number is out of bounds. Try again.\n");
            continue;
        }
        break;
    }

    /* ----- SETUP ---------*/

    // vars which will be params for getaddrinfo()
    struct addrinfo hints;
    struct addrinfo *res; // will point to the results

    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;

    /* ----- ACT AS A CLIENT --------------------------------------------------- */

    // create a socket
    int out_client_socket;

    getaddrinfo("::1", "10001", &hints, &res);

    // protocol = 0 (default: TCP)
    out_client_socket = socket(res->ai_family, res->ai_socktype, 0);

    // connect
    int connection_status = connect(out_client_socket, res->ai_addr, res->ai_addrlen);

    // check for error with the connection
    // 0 for no errors
    if (connection_status == -1) {
        printf("[%s] There was an error making a connection to the remote socket \n\n", server_name);
    }

    char out_client_message[256] = "hello";

    char hops_to_string[2];
    sprintf(hops_to_string, "%d", hops);

    // send data to the server
    send(out_client_socket, out_client_message, sizeof(out_client_message), 0);
    send(out_client_socket, hops_to_string, sizeof(out_client_message), 0);

    // and then close the socket
    close(out_client_socket);

    /* ----- ACT AS A SERVER --------------------------------------------------- */

    // create the server socket
    int in_server_socket;

    getaddrinfo("::1", "10000", &hints, &res);

    in_server_socket = socket(res->ai_family, res->ai_socktype, 0);

    // bind the socket to our specified IP and port
    bind(in_server_socket, res->ai_addr, res->ai_addrlen);

    // 2nd arg - backlog: how many connections can be waiting for this socket.
    // Set 5, but doesn't matter
    listen(in_server_socket, 5);

    int in_client_socket;
    // 2nd param - struct that contains address of the client connection,
    // 3rd - sizeof it. We'll leave it at NULL
    in_client_socket = accept(in_server_socket, NULL, NULL);

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


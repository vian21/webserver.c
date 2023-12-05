#include "headers.h"
#include "library.h"
#include <pthread.h>

#define SERVER_PORT 5000
#define THREAD_MAX 128

int main()
{
    int server_socket = new_webserver(SERVER_PORT);

    // accept connections
    struct sockaddr_in client_address;
    int client_socket;
    int address_len = sizeof(client_address);

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&address_len)))
    {
        /////////////////////single threaded////////////////////
        handle_connection(client_socket);

        ////////////////////Multi-threaded///////////////////////
        // pthread_t thread;
        // int *client_s = malloc(sizeof(int));
        // *client_s = client_socket;

        // pthread_create(&thread,NULL,handle_connection_worker, client_s);
    }
}

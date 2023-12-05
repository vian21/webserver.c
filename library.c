#include "library.h"
#include "headers.h"
#include <string.h>
#include "time.h"
#include <locale.h>

#define NOT_FOUND_ERROR_MESSAGE "<p>Page not found</p>\n"
#define SERVER_ERROR_MESSAGE "<p>Internal server error</p>\n"

size_t n_requests = 0;
char response_buffer[1024];
char request[REQUEST_LEN];
char time_buffer[20];

int new_webserver(int port)
{
    // create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // set socket options
    int true = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&true, sizeof(true)) < 0)
    {
        perror("Couldn't set server options");
        exit(EXIT_FAILURE);
    }

    // bind socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY; /* Address to accept any incoming messages.  */

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        perror("Failed to bind address");
        exit(EXIT_FAILURE);
    }
    // listen
    if (listen(server_socket, 10) < 0)
    {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port: %d\n", port);

    return server_socket;
}
void close_exit(int socket)
{
    close(socket);
    exit(EXIT_FAILURE);
}

void format_response(char *response_buffer, Status_code code, char *path, char *body)
{
    sprintf(response_buffer, "HTTP/1.1 %d \r\nContent-Length: %ld \r\n\r\n%s", code, strlen(body), body);
}

char *read_page(char *file_name)
{
    FILE *file = fopen(file_name, "r");

    fseek(file, 0L, SEEK_END);
    long numbytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char *text = (char *)calloc(numbytes, sizeof(char));

    fread(text, sizeof(char), numbytes, file);
    fclose(file);

    return text;
}
char *get_page(char *path)
{
    if (strcmp("/", path) == 0 || strcmp("/index.html", path) == 0)
    {
        return read_page("www/index.html");
    }

    if (strcmp("/about.html", path) == 0)
    {
        return read_page("www/about.html");
    }

    return NULL;
}

/**
 * returns current system time string
 * `Format`: 2023-10-26 19:53:14
 *
 */
char *get_time()
{
    time_t now = time(NULL);
    struct tm *tm_local = localtime(&now);

    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %X", tm_local);

    return time_buffer;
}

void handle_request(char *request, char *response_buffer)
{

    // tokenize
    char *save_ptr = request; // pointer to be used in strtok_r to be thread safe. See: man strtok_r
    char *tokens = strtok_r(save_ptr, " ", &save_ptr);

    char *request_type = tokens;
    char *path = strtok_r(save_ptr, " ", &save_ptr);

    if (path == NULL)
        return;

    char *time = get_time();

    (void)printf("[LOG] Request ID: %ld Time:%s %s Path: %s\n", ++n_requests, time, request_type, path);

    char *body = get_page(path);

    if (body == NULL)
    {
        error_page(NOT_FOUND, response_buffer);
        return;
    }

    format_response(response_buffer, OK, path, body);

    free(body);
}

void *handle_connection_worker(void *client_sock)
{
    int client_socket = *(int *)client_sock;
    free(client_sock);

    handle_connection(client_socket);

    return NULL;
}

void handle_connection(int client_socket)
{
    // get client request
    int request_size = read(client_socket, request, REQUEST_LEN);

    if (request_size <= 0)
    {
        perror("Empty request");
    }
    else
    {
        handle_request(request, response_buffer);

        (void)write(client_socket, response_buffer, strlen(response_buffer));
    }

    (void)close(client_socket);
}

void error_page(Status_code code, char *response_buffer)
{
    switch (code)
    {
    case NOT_FOUND:
        format_response(response_buffer, code, "", NOT_FOUND_ERROR_MESSAGE);
        break;

    case SERVER_ERROR:
        format_response(response_buffer, code, "", SERVER_ERROR_MESSAGE);
        break;
    default:
        break;
    }
}

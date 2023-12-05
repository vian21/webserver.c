#ifndef LIBRARY_H
#define LIBRARY_H

#define REQUEST_LEN 1024

typedef enum HTTP_Verb
{
    GET,
    POST,
    PUT,
    DELETE
} HTTP_Verb;

typedef enum Status_code
{
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 503
} Status_code;

/*
 * - Create a new webserver socket
 * - Quits the program if there is error while initialising the socket
 *
 * returns socket descriptor(int)
 */
int new_webserver(int port);

/**
 * close socket connection and exit program
 */
void close_exit(int);

/*
 * Format HTTP response by appending required headers
 *
 * HTTP/1.1 200 OK
 * Content-Length: strlen(body)
 */
void format_response(char *response_buffer, Status_code code, char *path, char *body);

/*
 * Handles raw HTTP request and writes response to response buffer
 *
 */
void handle_request(char *request, char *response_buffer);

/**
 * write error page to response buffer
 */
void error_page(Status_code code, char *response_buffer);

/**
 * Handle HTTP request given created client socket
 */
void handle_connection(int client_socket);

/*
 * handle client connection task (Thread)
 */
void *handle_connection_worker(void *client_socket);

#endif
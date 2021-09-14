#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <sys/un.h>
#include <signal.h>
#include <stdarg.h>

// Prototypes for internal functions and utilities
void error(const char *fmt, ...);
int runClient(char *clientName, int numMessages, char **messages);
int runServer();
void serverReady(int signal);

bool serverIsReady = false;

// Prototypes for functions to be implemented by students
void server();
void client(char *clientName, int numMessages, char *messages[]);

void verror(const char *fmt, va_list argp)
{
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");
}

void error(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    verror(fmt, argp);
    va_end(argp);
    exit(1);
}

int runServer(int port) {
    int forkPID = fork();
    if (forkPID < 0)
        error("ERROR forking server");
    else if (forkPID == 0) {
        server();
        exit(0);
    } else {
        printf("Main: Server(%d) launched...\n", forkPID);
    }
    return forkPID;
}

int runClient(char *clientName, int numMessages, char **messages) {
    fflush(stdout);
    printf("Launching client %s...\n", clientName);
    int forkPID = fork();
    if (forkPID < 0)

        error("ERROR forking client %s", clientName);
    else if (forkPID == 0) {
        client(clientName, numMessages, messages);
        exit(0);
    }
    return forkPID;
}

void serverReady(int signal) {
    serverIsReady = true;
}

#define NUM_CLIENTS 2
#define MAX_MESSAGES 5
#define MAX_CLIENT_NAME_LENGTH 10

struct client {
    char name[MAX_CLIENT_NAME_LENGTH];
    int num_messages;
    char *messages[MAX_MESSAGES];
    int PID;
};

// Modify these to implement different scenarios
struct client clients[] = {
        {"Uno", 3, {"Mensaje 1-1", "Mensaje 1-2", "Mensaje 1-3"}},
        {"Dos", 3, {"Mensaje 2-1", "Mensaje 2-2", "Mensaje 2-3"}}
};

int main() {
    signal(SIGUSR1, serverReady);
    int serverPID = runServer(getpid());
    while(!serverIsReady) {
        sleep(1);
    }
    printf("Main: Server(%d) signaled ready to receive messages\n", serverPID);

    for (int client = 0 ; client < NUM_CLIENTS ; client++) {
        clients[client].PID = runClient(clients[client].name, clients[client].num_messages,
                                        clients[client].messages);
    }

    for (int client = 0 ; client < NUM_CLIENTS ; client++) {
        int clientStatus;
        printf("Main: Waiting for client %s(%d) to complete...\n", clients[client].name, clients[client].PID);
        waitpid(clients[client].PID, &clientStatus, 0);
        printf("Main: Client %s(%d) terminated with status: %d\n",
               clients[client].name, clients[client].PID, clientStatus);
    }

    printf("Main: Killing server (%d)\n", serverPID);
    kill(serverPID, SIGINT);
    int statusServer;
    pid_t wait_result = waitpid(serverPID, &statusServer, 0);
    printf("Main: Server(%d) terminated with status: %d\n", serverPID, statusServer);
    return 0;
}



//*********************************************************************************
//**************************** EDIT FROM HERE *************************************
//#you can create the global variables and functions that you consider necessary***
//*********************************************************************************

#define PORT_NUMBER 44142

bool serverShutdown = false;

void shutdownServer(int signal) {
 //Indicate that the server has to shutdown
 serverShutdown = true;

 //Wait for the children to finish
 int status, childPID;
 while (childPID = wait(&status) > 0) {
    // printf("Main: Client(%d) terminated with status: %d", childPID, status);
 }
 //Exit
 exit(0);
}

void client(char *clientName, int numMessages, char *messages[])
{
    char buffer[256];
    int sockfd, n; //declare int for socker file descriptor
    struct sockaddr_in server_address; //declare sockaddr_in variable for all connection info

    for (int i = 0; i < numMessages; i++) {
        //Open the socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            error("Client: Error opening socket.\n");
        }

        bzero((char *) &server_address, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT_NUMBER);
        inet_aton("localhost", &server_address.sin_addr.s_addr);

        //Connect to the server
        if (connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
            error("Client: Error connecting\n");
        }

        //For each message, write to the server and read the response
        char *message = messages[i];
        n = write(sockfd, message, strlen(message));
        if (n < 0) {
            error("Client: Error writing to server.\n");
        }

        bzero(buffer, 256); //clear buffer
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            error("Client: Error reading response from server.\n");
        }

        printf("%s:%d:%s\n", clientName, getpid(), buffer); //print received message

        // printf("Client %s(%d): Return message: %s\n", clientName, getpid(), buffer); //DELETE???
        fflush(stdout);
        sleep(1);
    }

    //Close socket
    close(sockfd);
}

void server()
{
    char buffer[256];
    int sockfd, newsockfd, n; //declare int for socker file descriptor
    struct sockaddr_in server_address, client_address; //declare sockaddr_in variable for all connection info
    int clilen = sizeof(client_address);

    //Handle SIGINT so the server stops when the main process kills it
    signal(SIGINT, shutdownServer);

    //Open the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Server: Error opening socket.\n");
    }

    //Set up connection info
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUMBER);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //Bind the socket
    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        error("Server: Error binding the socket\n");
    }

    listen(sockfd, 3); //enable use of accept() and set backlog to 3

    //Signal server is ready
    int parentPID = getppid(); 
    serverReady(0);
    kill(parentPID, SIGUSR1);

    //Accept connection and create a child proccess to read and write 
    while (!serverShutdown && ((newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &clilen)) >= 0)) {
        if (newsockfd < 0) {
            error("Server: Error accepting connection from client.\n");
        }

        int newPID = fork();
        if (newPID == 0) { //child server
            bzero(buffer, 256); //clear buffer
            n = read(newsockfd, buffer, 255);
            if (n < 0) {
                error("Server: Error reading from socket.\n");
            }

            printf("Server:%d:%s\n", getpid(), buffer); //print received message
            fflush(stdout);

            n = write(newsockfd, buffer, strlen(buffer));
            if (n < 0) {
                error("Server: Error writing to client.\n");
            }
            exit(0);
        } else if (newPID > 0) { //server
            close(newsockfd); //close the mailbox
        }
    }

    //Close socket
    close(sockfd);
}
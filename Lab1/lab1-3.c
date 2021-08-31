#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHARS 10000

// void printMessage(char **message) { //og
void printMessage(char *message[]) { //testing
    int size = sizeof(message)/sizeof(char*);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c", message[i][j]);
        }
    }
    printf("\n");
}

char** processCmd(int size, char cmd[]) {
    char** message = malloc(size * sizeof(char*)); //'size' amount of memory locations pointing to char* ptrs
    int msgPtr = 0;
    for (int i = 0; i < size; i++) {
        char* column = malloc(size * sizeof(char)); //'size' memory locations that point to a char
        for (int j = 0; j < size; j++) {
            column[j] = cmd[msgPtr++];
        }
        message[i] = column;
    }

    //getting experimentaL!
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c", message[i][j]);
        }
    }
    return message;
}

int main() {

    char cmd[MAX_CHARS];
    int t;
    scanf("%d\n", &t);
    char *messages = malloc(sizeof(char)); //trying to make it be multi-input
    int total = t;
    int *totalPointer = 0;
    while(t--) {
        fgets(cmd, MAX_CHARS, stdin);	    //cmd has current string 
        int msgSize = 0; 
        for (int i = 0; i < MAX_CHARS; i++) { //count how many characters in the message
            if (cmd[i] == (char)'\n') {
                printf("DEBUGGING counting - msgSize:%d; cmd[i]:%d; i:%d\n", msgSize, cmd[i], i);               
                break;
            } else {
                msgSize++; 
                printf("DEBUGGING counting else - msgSize:%d; cmd[i]:%d; i:%d\n", msgSize, cmd[i], i);               
            }
        }
        if (msgSize != 0) {
            float root = sqrt((float)msgSize);
            int intRoot = root;
            if (intRoot == root) { //it's a perfect sqare
                char **message = processCmd(root, cmd);
                messages[t-1] = message; //trying to make it be multi-input

                //testing: --v
                // printMessage(root, message);
            } else {
                messages[t] = NULL; //for multi-input
                printf("DEBUGGING - invalid - msgSize:%d; root:%f; intRoot:%d\n", msgSize, root, intRoot);
                // printf("INVALID\n"); //testing
            }
        }
    }
    if (total != 0) { //print all messages
        for (int i = total; i > 0; i--) {
            if (messages[i] != NULL) {
                printMessage(messages[i]);
            } else {
                printf("INVALID\n");
            }
        }
    }
    return 0;
}

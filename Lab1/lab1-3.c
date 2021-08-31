#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHARS 10000

void printMessage(char *messages) {
    int i = 0;
    while (messages[i]) {
        printf("%c", messages[i]);
    }
}

void addMessage(char cmd[], int *ptr, char *messages, int size) {
    char** message = malloc(size * sizeof(char*)); //'size' amount of memory locations pointing to char* ptrs
    int msgPtr = 0;
    for (int i = 0; i < size; i++) { //create matrix
        char* column = malloc(size * sizeof(char)); //'size' memory locations that point to a char
        for (int j = 0; j < size; j++) {
            column[j] = cmd[msgPtr++];
        }
        message[i] = column;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            messages[*ptr++] = message[i][j]; //add message to bigger messages list
            printf("DEBUGGING - addMessage - messages[*ptr]:%c", messages[*ptr - 1]);
        }
    }
    messages[*ptr++] = '\n'; //add newline character to differentiate messages
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
                // printf("DEBUGGING counting - msgSize:%d; cmd[i]:%d; i:%d\n", msgSize, cmd[i], i);               
                break;
            } else {
                msgSize++; 
                // printf("DEBUGGING counting else - msgSize:%d; cmd[i]:%d; i:%d\n", msgSize, cmd[i], i);               
            }
        }
        if (msgSize != 0) {
            float root = sqrt((float)msgSize);
            int intRoot = root;
            if (intRoot == root) { //it's a perfect sqare
                // printf("DEBUGGING - valid - msgSize:%d; root:%f; intRoot:%d\n", msgSize, root, intRoot);
                messages = (char*)realloc(messages, (sizeof(messages)/sizeof(char))+msgSize+1); //increase size to accomodate new message
                // messages = realloc(messages, sizeof(messages)+msgSize+1); //increase size to accomodate new message
                addMessage(cmd, totalPointer, messages, intRoot);
                // printf("DEBUGGING - valid - done adding message");
            } else {
                // printf("DEBUGGING - invalid - msgSize:%d; root:%f; intRoot:%d\n", msgSize, root, intRoot);
                messages = (char*)realloc(messages, (sizeof(messages)/sizeof(char))+8); //increase size to accomodate new message
                char invalid[8] = {'I','N','V','A','L','I','D','\n'};
                for (int i = 0; i < 8; i++) {
                    messages[*totalPointer++] = invalid[i];
                }
            }
        }
    }
    if (total != 0) { //print all messages
        printf("print messages??");
        printMessage(messages);
    }
    return 0;
}

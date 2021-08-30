#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHARS 10000

void printMessages(int** messages, int t) {
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d", message[i][j]);
        }
    }
}

int** processCmd(int size, char cmd[]) {
    int** message = malloc(size, sizeof(int)); 
    int msgPtr = 0;
    for (int i = 0; i < size; i++) {
        int* column = malloc(size, sizeof(int));
        for (int j = 0; j < size; j++) {
            column[j] = cmd[msgPtr++];
        }
        message[i] = column;
    }
    return message;
}

int main() {

    char cmd[MAX_CHARS];
    int t;
    scanf("%d\n", &t);
    int** messages = malloc(t, sizeof(int));
    while(t--) {
        fgets(cmd, MAX_CHARS, stdin);	    //cmd has current string 
        int msgSize = 0; 
        for (int i = 0; i < MAX_CHARS; i++) { //count how many characters in the message
            if (cmd[i] == "/n") {
                break;
            } else {
                msgSize++;
            }
        }
        if (msgSize != 0) {
            float root = sqrt((float)msgSize);
            int intRoot = root;
            if (intRoot == root) { //it's a perfect sqare
                int** message = processCmd(root, cmd);
                messages[t] = message;
            }
        }
    }
    return 0;
}

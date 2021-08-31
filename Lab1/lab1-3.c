#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHARS 10000

int main() {

    char cmd[MAX_CHARS];
    int t;
    scanf("%d\n", &t);
    while(t--) {
        fgets(cmd, MAX_CHARS, stdin);	    //cmd has current string 
        int msgSize = 0; 
        for (int i = 0; i < MAX_CHARS; i++) { //count how many characters in the message
            if (cmd[i] == (char)'\n') {
                break;
            } else {
                msgSize++; 
            }
        }
        if (msgSize != 0) {
            float root = sqrt((float)msgSize);
            float ceilRoot = ceil(root);
            if (ceilRoot == root) { //it's a perfect sqare(ish)
                for (int i = 0; i < ceilRoot; i++) {
                    for (int j = 0; j < ceilRoot; j++) {
                        printf("%c", cmd[(j*(int)ceilRoot)+i]);
                    }
                }
                printf("\n");
            } else {
                printf("INVALID\n");
            }
        }
    }
    return 0;
}

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TOKEN_DELIMETERS " \t\r\n\a"
#define DEFAULT_SIZE 128

char *history[DEFAULT_SIZE];
char *argv[DEFAULT_SIZE];
int hist_count = 0;
int hist_num = 0;
int position = 0;
short isHistNum = 0;

void read_line(char *line){
    if(isHistNum){
        line = memcpy(line,history[hist_num-1],DEFAULT_SIZE);
        printf("\n");
    }
    else{
        gets(line);
    }
    isHistNum = 0;
    memcpy(history[hist_count],line,DEFAULT_SIZE);
    hist_count++;
}

void parse_line(char *line,char *argv[]){
    char *token;
    position = 0;
    token = strtok(line,TOKEN_DELIMETERS);
    while(token!=NULL){
        argv[position] = token;
        position++;
        token =strtok(NULL,TOKEN_DELIMETERS);
    }
}

int nat_history(char *argv[]){
    if(position==2){
        hist_num = atoi(argv[1]);
        for(int k = 1; k<=hist_count;k++){
            if(hist_num == k){
                isHistNum = 1;
            }
        }
    }
    if(isHistNum==0){
        for(int i =0; i <hist_count;i++)
            printf(" %d %s\n",(i+1),history[i]);
    }
    return 1;
}

void execute(char *argv[]){

    // Check if command is valid as a native command
    char exitWord[10]; strcpy(exitWord, "exit");
    char hist[10]; strcpy(hist, "history");
    char help[10]; strcpy(help, "help");
    char cd[10];   strcpy(cd, "cd");

    //Native commands
    if(strcmp(argv[0], exitWord) == 0){
        printf("Exiting shell!\n");
        exit(0);
    }else if(strcmp(argv[0], hist) == 0){
        nat_history(argv);
    }else if(strcmp(argv[0], help) == 0){
        printf("\n~~ Shell Help: ~~\n\n - Enter 'exit' to close the shell.\n - Enter 'help' to list this page.\n - Enter 'cd <path>' to switch to the listed path.\n - Enter 'history [<number>]' to see the previously entered commands.\n - Enter any other non-native commands and they'll be carried out with a child process.\n\n");
    }else if(strcmp(argv[0], cd) == 0) {
        chdir(argv[1]);
    }else{
        //Execute any other command. Uses child process.
        pid_t childPID = fork();
        if (childPID == 0) { //child
            execvp(argv[0], argv);
        } else if (childPID > 0) {
            wait(NULL);
        }
    }
}

int main(int argc, char *argv[]){
    int valid = 0;
    char *line = (char*)malloc(DEFAULT_SIZE);
    for(int i = 0;i<DEFAULT_SIZE;i++)
        history[i] = (char*)malloc(DEFAULT_SIZE);
    long size;
    char *buf;
    char *ptr;
    while(1){
        printf("Shell->");
        read_line(line);
        parse_line(line,argv);
        execute(argv);
        for(int j = 0; j < DEFAULT_SIZE;j++){
            argv[j] = NULL;
        }
    }
}
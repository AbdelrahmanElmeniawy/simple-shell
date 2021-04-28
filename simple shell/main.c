#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <time.h>

#define Max_Strings 20
#define Max_Chars 200

// take user input
int input(char* args[], char line[]){
    //input all user input in one string
    fgets(line, Max_Chars, stdin);

    //separate each word
    args[0] = strtok(line, " \n");
    int i = 0;
    while(args[i] != NULL){
        i++;
        args[i] = strtok(NULL, " \n");
    }

    // exit condition
    if (i == 1 && strcmp(args[0], "exit") == 0){
        exit(0);
    }

    // check of existing &
    int flag = 0;
    i = 0;
    while(args[i] != NULL){
        if (strcmp(args[i], "&") == 0){
            flag = 1;
        }
        if (flag){
            args[i] = args[i + 1];
        }
        i++;
    }
    return flag;
}

// logging children termination
void log(){
    FILE* lg = fopen("log.txt", "a");
    if (lg == NULL){
        fprintf(stderr, "could not open log file!");
        return;
    }
    time_t now;
    time(&now);
    fprintf(lg, "Child terminated! %s", ctime(&now));
    fclose(lg);
}


int main()
{
    signal(SIGCHLD, log);

    while(1){

        char line[Max_Chars];
        char* args[Max_Strings];

        printf(" >> ");
        int flag = input(args, line);

        int pid = fork();

        if (pid < 0){
            fprintf(stderr, "Fork Failed!");
            return 1;
        }
        else if (pid == 0){
            // child process
            execvp(args[0], args);
            printf("command not found!\n");
            exit(0);
        } else {
            // parent process
            if (!flag) wait(pid, 0);
        }
    }
    return 0;
}

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char** argv) {
    
    char in[20];
    const char* delim = " ";
    char command[20] = "/usr/bin/";
    char* portion1;
    char* portion2;
  
    
    pid_t fork_pid;

    printf("Welcome to the A1 Simple Shell\n");

    while (1){
        fork_pid = -1;
        printf(":: ");
        fgets(in, 20, stdin);

        
        for(int i = 0; i <20; i++) {
            if(in[i] == '\n') {
                in[i] = '\0';
                break;
            }
        }
        
        for(int i = 9; i <20; i++) {
           command[i] = in[i-9];     
        }
       
        portion1 = strtok(command, delim);
        portion2 = strtok(NULL, delim);

        if (portion2 == NULL){
            for(int i = 0; i <20; i++){
                if(portion1[i] == '\n') {
                    portion1[i] = '\0';
                    break;
                }
            }
        }
        else{
            for(int i = 0; i <20; i++){
                if(portion2[i] == '\n') {
                    portion2[i] = '\0';
                    break;
                }
            }
        }
       


       if (strcmp(in, "help") == 0){
            printf("\nHelp Page\n\n");
            printf("help -lists commands for A1 Shell\n");
            printf("exit - exits the shell\n");
            printf("Usr/bin commands are available as well\n");
        }
        else if (strcmp(in, "exit") == 0) {
            printf("Exiting the A1 shell");
            break;
        }
        else { //command from /usr/bin
            fork_pid = fork();
            switch (fork_pid)
            {
                case 0:
                    execl(portion1, portion1, portion2, (char*)NULL);
                    printf("not a command\n");
                    exit(1);
                    
                case -1: //this is the case where the function fails, there is no child here
                    printf("\n error");
                    break;

                default:
                    int state = 0;
                    waitpid(fork_pid, &state, 0); //this esures that the parent prints before the child 
                    break;
            }
        }
        
        }
   
    return 0;
}

 // Kanato Sato
 // V00948631


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LINE 256
#define MAX_NUM_TOKENS 20

void DoCommand(char dict[10][MAX_INPUT_LINE], char **tokendInput, int num_tokens, char *command_type); // Implement a command typed in prompt
void default_command(char *com, char *option); // Implement default command (ie. command without OR or PP)
void OR_command(char *com, char *option, char *file); // Implement OR command
void PP1_command(char *com, char *com2, char *option, char *option2); // Implement command with single pipe
void PP2_command(char *com, char *com2, char *com3, char *option, char *option2, char *option3); // Implement command with double pipes
    

char tokenize(char *input, char **token); // Tokenize command typed in prompt
char *check_command_type(char **input,int num); // Check command is default, OR, or PP
char check_path(char *com); // Check if command is in the directories provided in .sh360rc file



int main(int argc, char *argv[]) {
    char input[MAX_INPUT_LINE]; // Store command typed in prompt
    // char output[MAX_INPUT_LINE];
    char *tokendInput[MAX_NUM_TOKENS]; // Array stores each word of command typed in prompt
    int num_tokens; // Number of tokens in tokendInput
    int i = 0;

// -------------------------------------------------------------------------------------------
// Getting prompt word/message from .sh360rc file

    FILE* fp;
    // Make sure change the file name when submitting!!!!!!
    fp = fopen(".sh360rc", "r");
    if (fp == NULL){
        perror("Failed: ");
        return 1;
    }

    char buffer[MAX_INPUT_LINE];
    char dict[10][MAX_INPUT_LINE]; 

    // Store directories in .sh360rc into array
    while (fgets(buffer, MAX_INPUT_LINE - 1, fp)){
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(dict[i],buffer);
        i++;
    }

    fclose(fp);
// -------------------------------------------------------------------------------------------

    for(;;) {
        char command_type[10];

        fprintf(stdout, "%s ", dict[0]); // printing prompt for user
        fflush(stdout);
        fgets(input, MAX_INPUT_LINE, stdin);

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
            
        }

    
        if (strcmp(input, "exit") == 0) {
            exit(0);
        } 

        num_tokens = tokenize(input, tokendInput);

        strcpy(command_type,check_command_type(tokendInput,num_tokens - 1));

        DoCommand(dict,tokendInput,num_tokens,command_type);
    }
    return 0;
}




void DoCommand(char dict[10][MAX_INPUT_LINE], char **tokendInput, int num_tokens, char *command_type){

    int count;
    int count2;
    int count3;

    char dict_and_com[MAX_INPUT_LINE];
    char dict_and_com2[MAX_INPUT_LINE]; // 2nd path when piping
    char dict_and_com3[MAX_INPUT_LINE]; // 3rd path when piping

    char command[MAX_INPUT_LINE] = "";
    char command2[MAX_INPUT_LINE] = ""; // 2nd command when piping
    char command3[MAX_INPUT_LINE] = ""; // 3rd command when piping
    char option[80] = "";
    char option2[80] = ""; // 2nd option when piping
    char option3[80] = ""; // 3rd option when piping
    char file[MAX_INPUT_LINE] = "";

// OR command implementation
    if (strcmp(command_type, "OR") == 0){
            int ind;
            strcat(command,tokendInput[1]);

            for (ind = 2; ind < num_tokens; ind++){

                if (strcmp(tokendInput[ind],"->") == 0){
                    strcat(file,tokendInput[ind+1]);
                    break;
                }
                
                strcat(option,tokendInput[ind]);
        
                strcat(option," ");
                
            }

            for (count=1; count<10; count++){
        
                strcpy(dict_and_com,dict[count]);
                strcat(dict_and_com,"/");
                strcat(dict_and_com, command);

                if (check_path(dict_and_com) == 0){
                break;
                }
            }
            if (check_path(dict_and_com) == 1){
                printf("%s: No such file.\n",command2);
            }
            else {
                OR_command(dict_and_com, option, file);
            }

    }

// PP with a single pipe implementation
    else if (strcmp(command_type, "PP1") == 0){
        int ind;
        strcat(command,tokendInput[1]);

        for (ind = 2; ind < num_tokens; ind++){

                if (strcmp(tokendInput[ind],"->") != 0){
                    strcat(option,tokendInput[ind]);
                    strcat(option, " ");
                    continue;
                }
                if (strcmp(tokendInput[ind],"->") == 0){
                    break;
                }
                
            }
        ind += 1;
        strcat(command2,tokendInput[ind]);
        
        for (ind += 1; ind < num_tokens; ind++){
            strcat(option2, tokendInput[ind]);
            strcat(option2, " ");
        }

        for (count=1; count<10; count++){
            strcpy(dict_and_com,dict[count]);
            strcat(dict_and_com,"/");
            strcat(dict_and_com, command);
            if (check_path(dict_and_com) == 0){
                break;
            }
        }

        if (check_path(dict_and_com) == 1){
            printf("%s\n",dict_and_com);
            printf("%s: No such file.\n",command);
        }

        else {
            for (count2=1; count2<10; count2++){
                strcpy(dict_and_com2,dict[count2]);
                strcat(dict_and_com2,"/");
                strcat(dict_and_com2, command2);
                if (check_path(dict_and_com2) == 0){
                    break;
                }
            }   

            if (check_path(dict_and_com2) == 1){
                printf("%s: No such file.\n",command2);
            }

            else {
                PP1_command(dict_and_com, dict_and_com2, option, option2);
            }
        }
    }

// PP with double pipes implementaion
    else if (strcmp(command_type, "PP2") == 0){

        int ind;
        strcat(command,tokendInput[1]);

        for (ind = 2; ind < num_tokens; ind++){

                if (strcmp(tokendInput[ind],"->") != 0){
                    strcat(option,tokendInput[ind]);
                    strcat(option, " ");
                    continue;
                }
                if (strcmp(tokendInput[ind],"->") == 0){
                    break;
                }
                
            }
        ind += 1;
        strcat(command2,tokendInput[ind]);
        
        for (ind += 1; ind < num_tokens; ind++){
            if (strcmp(tokendInput[ind],"->") != 0){
                strcat(option2,tokendInput[ind]);
                strcat(option2, " ");
                continue;
            }
            if (strcmp(tokendInput[ind],"->") == 0){
                break;
            }
        }

        ind += 1;
        strcat(command3,tokendInput[ind]);
        
        for (ind += 1; ind < num_tokens; ind++){
            strcat(option3,tokendInput[ind]);
            strcat(option3, " ");
        }

        
        for (count=1; count<10; count++){
            strcpy(dict_and_com,dict[count]);
            strcat(dict_and_com,"/");
            strcat(dict_and_com, command);
            if (check_path(dict_and_com) == 0){
                break;
            }
        }

        if (check_path(dict_and_com) == 1){
                printf("%s: No such file.\n",command);
            }

        else{

            for (count2=1; count2<10; count2++){
                strcpy(dict_and_com2,dict[count2]);
                strcat(dict_and_com2,"/");
                strcat(dict_and_com2, command2);
                if (check_path(dict_and_com2) == 0){
                    break;
                }
            }

            if (check_path(dict_and_com2) == 1){
                printf("%s: No such file.\n",command2);
            }

            else {
                for (count3=1; count3<10; count3++){
                    strcpy(dict_and_com3,dict[count3]);
                    strcat(dict_and_com3,"/");
                    strcat(dict_and_com3, command3);
                    if (check_path(dict_and_com3) == 0){
                        break;
                    }

                }
                if (check_path(dict_and_com3) == 1){
                    printf("%s: No such file.\n",command3);
                }
                else{
                    
                    PP2_command(dict_and_com, dict_and_com2, dict_and_com3, option, option2, option3);
                }
            }
        }

    }

// Some error occured....    
    else if (strcmp(command_type, "Error") == 0) {
        printf("Error occured\n");
    }

// Default command
    else {
            int ind;
            strcat(command,tokendInput[0]);

            for (ind = 1; ind < num_tokens; ind++){
                
                strcat(option,tokendInput[ind]);

                strcat(option," ");

            }

            for (count=1; count<10; count++){
        
                strcpy(dict_and_com,dict[count]);
                strcat(dict_and_com,"/");
                strcat(dict_and_com, command);
                if (check_path(dict_and_com) == 0){
                    break;
                }
            }

            if (check_path(dict_and_com) == 1){
                printf("%s: No such file.\n",command);
            }
            else {
                default_command(dict_and_com, option);
            }

            
    }
    
    
}


void default_command(char *com, char *option){

    option[strcspn(option, " ")] = 0;

    char *args[] = { com, option, 0 }; // middle 0 for default command, -l,-1,so on for additional command
    char *envp[] = { 0 };
    int pid;
    int status;

    if (strcmp(option,"") == 0){
        args[1] = 0;
    }


    if ((pid = fork()) == 0) {
        execve(args[0], args, envp);
        exit(0);
        
    }
    if (pid < 0) {
        printf("Fork failed\n");
        exit(0);
    }

    while (wait(&status) > 0) {
        
    }
}


void OR_command(char *com, char *option, char *file){
    option[strcspn(option, " ")] = 0;

    char *args[] = { com, option, 0 }; // middle 0 for default command, -l,-1,so on for additional command
    char *envp[] = { 0 };
    int pid, fd;
    int status;

    if (strcmp(option,"") == 0){
        args[1] = 0;
    }


    if ((pid = fork()) == 0) {
      
        fd = open(file, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
        if (fd == -1) {
            fprintf(stderr, "cannot open output.txt for writing\n");
            exit(1);
        }
        dup2(fd, 1);
        dup2(fd, 2); 
        execve(args[0], args, envp);
        exit(0);

    }


    waitpid(pid, &status, 0);

}

void PP1_command(char *com, char *com2, char *option, char *option2){

    option[strcspn(option, " ")] = 0;
    option2[strcspn(option2, " ")] = 0;

    char *cmd_head[] = { com, option, 0 };
    char *cmd_tail[] = { com2, option2, 0 };
    char *envp[] = { 0 };
    int status;
    int pid_head, pid_tail;
    int fd[2]; // 1st command --> 2nd command

    if (strcmp(option,"") == 0){
        cmd_head[1] = 0;
    }

    if (strcmp(option2,"") == 0){
        cmd_tail[1] = 0;
    }

    if (pipe(fd) == -1){
        printf("bad pipe\n");
        exit(0);
    }

    if ((pid_head = fork()) == 0) {
        // printf("child (head): re-routing plumbing; STDOUT to pipe.\n");
        dup2(fd[1], 1);
        close(fd[0]);
        execve(cmd_head[0], cmd_head, envp);
        close(fd[0]);
        close(fd[1]);
        // exit(0);
        // printf("child (head): SHOULDN'T BE HERE.\n");
        exit(0);
    }

    if ((pid_tail = fork()) == 0) {
        // printf("child (tail): re-routing plumbing; pipe to STDIN.\n");
        dup2(fd[0], 0);
        // close(fd[0]);
        close(fd[1]);
        execve(cmd_tail[0], cmd_tail, envp);
        close(fd[0]);
        close(fd[1]);
        // exit(0);
        // printf("child (tail): SHOULDN'T BE HERE.\n");
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(pid_head, &status, 0);

    waitpid(pid_tail, &status, 0); 
}

void PP2_command(char *com, char *com2, char *com3, char *option, char *option2, char *option3){
    option[strcspn(option, " ")] = 0;
    option2[strcspn(option2, " ")] = 0;
    option3[strcspn(option3, " ")] = 0;
    
    char *cmd_head[] = { com, option, 0 };
    char *cmd_mid[] = { com2, option2, 0 };
    char *cmd_tail[] = { com3, option3, 0 };

    char *envp[] = { 0 };
    int status;
    int pid_head, pid_mid, pid_tail;
    int fd[2]; // 1st command --> 2nd command
    int fd1[2]; // 2nd command --> 3rd command

    if (strcmp(option,"") == 0){
        cmd_head[1] = 0;
    }
    if (strcmp(option2,"") == 0){
        cmd_mid[1] = 0;
    }
    if (strcmp(option3,"") == 0){
        cmd_tail[1] = 0;
    }

    // create pipe1
    if (pipe(fd) == -1) {
        perror("bad pipe1");
        exit(0);
    }

  // fork (command1)
    if ((pid_head = fork()) == -1) {
        perror("bad fork1");
        exit(1);
        }
    else if (pid_head == 0) {
    // stdin --> command1 --> pipe1

    // input from stdin (already done)
    // output to pipe1
        dup2(fd[1], 1);
    // close fds
        close(fd[0]);
        close(fd[1]);

        execve(cmd_head[0], cmd_head, envp);
        exit(0);

  }
  // parent
    
  // create pipe2
    if (pipe(fd1) == -1) {
        perror("bad pipe2");
        exit(0);

    }

  // fork (command2)
    if ((pid_mid = fork()) == -1) {
        perror("bad fork2");
        exit(0);
    }
    else if (pid_mid == 0) {
    // pipe1 --> command2 --> pipe2
 
    // input from pipe1
        dup2(fd[0], 0);
    // output to pipe2
        dup2(fd1[1], 1);

        close(fd[0]);
        close(fd[1]);
        close(fd1[0]);
        close(fd1[1]);

        execve(cmd_mid[0], cmd_mid, envp);
        exit(0);

    }
    // parent
    
    // close unused fds
    close(fd[0]);
    close(fd[1]);

    // fork (command3)
    if ((pid_tail = fork()) == -1) {
        perror("bad fork3");
        exit(0);
    }
    else if (pid_tail == 0) {
    // pipe2 --> command3 --> stdout

    // input from pipe2
        dup2(fd1[0], 0);
    // output to stdout (already done)
    // close fds
        close(fd1[0]);
        close(fd1[1]);

        execve(cmd_tail[0], cmd_tail, envp);
        exit(0);
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd[0]);
    close(fd[1]);
    
    waitpid(pid_head, &status, 0);
    waitpid(pid_mid, &status, 0); 

}

// This function tokenize the first argument by a single space and store each word into the second argument
// And returns the total number of words.
char tokenize(char *input, char **token) {
    int count_tokens;
    char *t;

    count_tokens = 0;
    t = strtok(input, " ");
    while (t != NULL && count_tokens < MAX_NUM_TOKENS) {
        token[count_tokens] = t;
        count_tokens++;
        t = strtok(NULL, " ");
    }

    return count_tokens;
}


// This function check if the input command starts with OR, PP, somethind else
char *check_command_type(char **input, int num){
    int i;
    int count = 0; // count # of ->

    for (i = 1; i<= num; i++){
        if (strcmp(input[i],">") == 0 || strcmp(input[i],"PP") == 0 || strcmp(input[i],"OR") == 0 || strcmp(input[i],"|") == 0 ){
            printf("Invalid characters in command\n");
            return "Error";
        }
    }

    if (strcmp(input[0],"OR") == 0){
        if (strcmp(input[num-1],"->") == 0){
            return "OR";
        }
        printf("Invalid OR\n");
        return "Error";
        
    }

    else if (strcmp(input[0],"PP") == 0){
        if (strcmp(input[num],"->") == 0){
            printf("invalid PP\n");
            return "Error";
        }

        for (i = 0; i <= num; i++){
            if (strcmp(input[i],"->") == 0){
                if (strcmp(input[i+1],"->") == 0){
                    printf("invalid PP\n");
                    return "Error";
                }
                count += 1;
            }
        }
        if (count == 0){
            printf("invalid PP\n");
            return "Error";
        }
        if (count == 1){
            return "PP1";
        }
        else if (count == 2){
            return "PP2";
        }
        else {
            return "None";
        }
        
    }
    else {
        return "None";
    }
}

// Check if path exist
char check_path(char *com){
    struct stat buf;
    if (stat(com,&buf) == 0){
        return 0;
    }
    else {
        return 1;
    }
}

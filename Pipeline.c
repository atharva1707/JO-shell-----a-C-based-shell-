#include"header.h"

void piping_bypass(char* instruction){
    int saved_stdout = dup(1);
    int saved_stdin = dup(1);

    char**piped_commands =(char**) malloc(sizeof(char*)*100) ;
    for(int i=0;i<100;i++){
        piped_commands[i] =(char*) malloc(sizeof(char)*100) ;
        strcpy(piped_commands[i],"\0");
    }
    int pipe_count = 0 ;

    strcpy(instruction,isolate_pipeline(instruction));
    while (instruction != NULL) {        
        strcpy(piped_commands[pipe_count],instruction);
        pipe_count++;
        instruction = strtok(NULL, "|");
    }
    char**token =(char**) malloc(sizeof(char*)*100) ;
    int token_count=0;
        
    for(int i=0;i<100;i++){
        token[i] =(char*) malloc(sizeof(char)*100) ;
        strcpy(token[i],"\0");
    }

    if(pipe_count==1){
        strcpy(piped_commands[0],MakeToken(piped_commands[0]));
        while(piped_commands[0]){
            strcpy(token[token_count],piped_commands[0]);
            token_count++;
            piped_commands[0]= strtok(NULL," ");
        }
        RunInstruction(token,token_count);
        return;}

    
    fopen ("p_file1.txt", "w+");
    fopen ("p_file2.txt", "w+");
    for(int i=0;i<pipe_count;i++){
        if(i%2==0){
            if(i!=0)open_input_file("p_file1.txt");
            open_output_file("p_file2.txt",0);
        }
        if(i%2==1){
            if(i!=0)open_input_file("p_file2.txt");
            open_output_file("p_file1.txt",0);
        }


        char* temp = (char*) malloc (sizeof(char)*100) ;
        strcpy(temp,MakeToken(piped_commands[i]));
        while(temp){
            strcpy(token[token_count],temp);
            token_count++;
            temp= strtok(NULL," ");
        }
        free(temp);
        RunInstruction(token,token_count);
        for(int i=0;i<100;i++){
            strcpy(token[i],"\0");
        }
        token_count = 0 ;
    }
    dup2(saved_stdout, 1);
    close(saved_stdout);
    dup2(saved_stdin, 0);
    close(saved_stdin);
    if(pipe_count%2==0){print_output_file("p_file1.txt");}
    if(pipe_count%2==1){print_output_file("p_file2.txt");}
    fflush(stdout);
    fflush(stdin);
    remove("p_file1.txt");
    remove("p_file2.txt");
}

char* MakeToken(char* s){
    char* token = strtok(s, " ");
    return token;
}














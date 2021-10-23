#include"header.h"



int open_input_file(char* filename) {
        int f = open(filename, O_RDONLY, S_IRWXU);
        if (f < 0) {
                perror(filename);
                
        }
        dup2(f, STDIN_FILENO);
        close(f);
        return f;
}

int open_output_file(char* filename,int append) {
        int f;
        if(!append){f = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);}
        else {f = open(filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);}
        if(f < 0) {
                perror(filename);
        }
        dup2(f,STDOUT_FILENO);
        close(f);
        return f;
}



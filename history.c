#include"header.h"

char history[20][1000];
int num_history = 0 ;

void maintain_history(char* input){
    if(strcmp(input,history[0])==0&&num_history>0){return;}
    else{
        for(int i=19;i>0;i--){
            strcpy(history[i],history[i-1]);
        }
        strcpy(history[0],input);
        if(num_history!=20){num_history++;}
        update_history();
    }
}

void update_history(){
    char cwd[500];
    getcwd(cwd,500);
    ChangeDir("~");
    FILE* fp = fopen ("history.txt", "w");
    fprintf(fp,"%d\n",num_history);
    for(int i=0;i<num_history;i++){
        fprintf(fp,"%s\n",history[i]);
    }
    fclose(fp);
    ChangeDir(cwd);
}

void read_history(){
    FILE* fp = fopen ("history.txt", "r");
    if(fp==NULL){
        int fd = open("history.txt", O_WRONLY | O_CREAT,0777);
        if(fd>=0){
        write(fd,"0\n",2);
        }
        else{
            perror("Cant read history");
        }
        close(fd);
        num_history=0;
        return;
    }
    fscanf(fp,"%d",&num_history);
    for(int i=0;i<num_history;i++){
        fgets( history[i], 1000, fp );
        history[i][strlen(history[i])-1]='\0';
    }
    fclose(fp);
}

void print_history(char** token , int token_count){
    int n=0 ;
    if(token_count==1){n=20;}
    else{n=atoi(token[1]);}
    if(n>num_history){n=num_history;}
    for(int i=0;i<n;i++){
        printf("%s\n",history[i]);
    }
}
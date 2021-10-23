#include"header.h"


char home[1000] ;
char prev_directory[2][1000];



void SetHome(){
    getcwd(home, sizeof(home));
    getcwd(prev_directory[0], sizeof(prev_directory[0]));
    getcwd(prev_directory[1], sizeof(prev_directory[1]));
}

void SetPrevDir(){
    strcpy(prev_directory[1],prev_directory[0]);
    getcwd(prev_directory[0], sizeof(prev_directory[0]));
}

int ChangeDir(char* s){
    if(strcmp(s,".")==0){
        return 1;
        }
    if(strcmp(s,"..")==0){
        int c = chdir(s);
        if(c==-1){
            perror("Cant open Directory: ");
            return 0;
        }
        return 1;
    }
    if(strcmp(s,"~")==0){
        int c = chdir(home);
        if(c==-1){
            perror("Cant open Directory: ");
            return 0;
        }
        return 1;
    }
    if(strcmp(s,"-")==0){
        int c = chdir(prev_directory[1]);
        if(c==-1){
            perror("Cant open Directory: ");
            return 0;
        }
        return 1;
    }
    int c = chdir(s);
    if(c==-1){
        perror("Cant open Directory: ");
        return 0;
    }
    return 1 ;
}

void PrintDir(){
    char current_directory[1000];
    getcwd(current_directory,sizeof(current_directory));
    int flag=1;
    for(int i=0;i<strlen(home);i++){
        if(current_directory[i]!=home[i]){flag=0;break;}
    }
    if(flag==0){
        for(int i=0;i<strlen(current_directory);i++){
            printf("%c",current_directory[i]);
        }
    }
    if(flag==1){
        printf("~");
        for(int i=strlen(home);i<strlen(current_directory);i++){
            printf("%c",current_directory[i]);
        }
    }
}
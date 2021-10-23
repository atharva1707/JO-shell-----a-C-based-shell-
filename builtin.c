#include"header.h"



void echo(char** token,int token_count){
    int saved_stdout = dup(1);

    int output_flag = 0 , output_file_no = 0 ;
    int append=0;
    char* output_file = (char*)malloc(sizeof(char)*100);
    for(int i=0 ; i<token_count-1;i++){
        if(strcmp(token[i],">")==0){output_flag=1;strcpy(output_file,token[i+1]);i++;}
        else if(strcmp(token[i],">>")==0){output_flag=1;append=1;strcpy(output_file,token[i+1]);i++;}
    }
    if(output_flag){output_file_no = open_output_file(output_file,append);token_count-=2;}


     if(strcmp("<",token[1])!=0){
        for(int j=1;j<token_count;j++) {
            int flag=1;
            for(int i=0;i<strlen(token[j]);i++){
                if(token[j][i]==' '){
                    if(!flag){printf(" ");}
                    flag=1;
                    continue;
                }
                if(token[j][i]=='\t'){
                    if(!flag){printf(" ");}
                    flag=1;
                    continue;
                }
                printf("%c",token[j][i]);
                flag=0;
            }
            printf("%c",' ');
        }
        printf("\n");}
    else{
        FILE* fp = fopen(token[2],"r");
        char c ;
        int flag = 1 ;
        while((c = fgetc(fp))!=EOF){
            if(c==' '){
                    if(!flag){printf(" ");}
                    flag=1;
                    continue;
                }
                if(c=='\t'){
                    if(!flag){printf(" ");}
                    flag=1;
                    continue;
                }
                printf("%c",c);
                flag=0;
        }
        printf("\n");
    fclose(fp);
    }
    dup2(saved_stdout, 1);
    close(saved_stdout);
    fflush(stdout);
}

void pwd(char** token,int token_count){
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("%s\n", cwd);
    } 
    else {
       perror("getcwd() error");
    }
}

int cd(char** token,int token_count){
    SetPrevDir();
    char* word = strtok(token[1], "/");
    while (word != NULL) {
        ChangeDir(word);
        word = strtok(NULL, "/");
    }
}

//
void ls(char** token,int token_count){
    int a_flag = 0 ;
    int l_flag = 0 ;
    int num_dir=0 ;
    char dir[100][100];
    for(int i=1;i<token_count;i++){
        if(strcmp(token[i],"-a")==0){a_flag=1;}
        else if(strcmp(token[i],"-la")==0){a_flag=1;l_flag=1;}
        else if(strcmp(token[i],"-al")==0){a_flag=1;l_flag=1;}
        else if(strcmp(token[i],"-l")==0){l_flag=1;}
        else{strcpy(dir[num_dir],token[i]);num_dir++;}
    }
    char cwd[500];
    char curr_cwd[500];
    getcwd(cwd, sizeof(cwd));
    getcwd(curr_cwd, sizeof(curr_cwd));
    int zero_flag=0;
    if(num_dir==0){zero_flag=1;;num_dir++;}
    for(int i=0;i<num_dir;i++){
        if(!zero_flag){ChangeDir(dir[i]);}
        getcwd(cwd, sizeof(cwd));
        printf("%s\n",cwd);
        DIR *dh = opendir(cwd);
        struct dirent *d;
        while ((d = readdir(dh)) != NULL)
        {
            if(l_flag==1&&a_flag==0 && d->d_name[0] == '.'){continue;}
            else if(l_flag==1){ls_with_l(d->d_name);}
            if (a_flag==0 && d->d_name[0] == '.')
                continue;
            printf("%s ", d->d_name);
            if(l_flag){printf("\n");}
        }
        printf("\n");
        if(!zero_flag){ChangeDir(curr_cwd);}
    }
}


void system_command(char** token,int token_count){
    int flag=0;
    char* arg[100];
    int n = 0 ;
    for(int i=0;i<token_count;i++){
        if(strcmp(token[i],"&")==0){flag=1;break;}
        arg[n]=(char*)malloc(sizeof(char)*100);
        strcpy(arg[n],token[i]);
        n++;
    }
    arg[n]=NULL;
    run_process(arg,n,flag);
    for(int i=0;i<token_count;i++){strcpy(token[i],"\0");}


}


void clear_input(){
    while(getchar());
}

void ls_with_l(char* arg){
    struct stat fileStat;
    if(stat(arg, &fileStat) < 0)    
        return ;

    

    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\t%ld\t", fileStat.st_nlink);
    printf("%ld", fileStat.st_ino);
    printf("\t%ld\t", fileStat.st_size);
    


}




#include"header.h"



void ScanInput(){
    fflush(stdout);
    fflush(stdin);
    char *input;
    input = (char*)malloc(sizeof(char)*1000);    
    for(int i=0;i<1000;i++){input[i]='\0';}
    fgets( input, 1000, stdin );
    if(strcmp(input,"\n")==0){return;}
    input[strlen(input)-1]='\0';  
    if(strcmp(input,"exit")==0){exit(0);}  
    maintain_history(input);
    strcpy(input,Getinstructions(input));
    char commands[100][1000];
    int command_count = 0 ;
    while (input != NULL) {        
        strcpy(commands[command_count],input);
        command_count++;
        input = strtok(NULL, ";");
    }
    
    for(int i=0;i<command_count;i++){
        piping_bypass(commands[i]);
    }
    free(input);
}

char* Getinstructions(char* s){
    char* token = strtok(s, ";");
    return token;

}

char* isolate_pipeline(char* s){
    char* token = strtok(s, "|");
    return token;
}



int find_repeat(char** token){
    int num_r=0;
    char s1[100],s2[100];
    strcpy(s1,token[0]);
    strcpy(s2,token[1]);
    if(strcmp(s1,"repeat")==0){
        num_r=atoi(s2);
        strcpy(token[0],"\0");
        strcpy(token[1],"\0");
        token+=2;
    }
    return num_r ;
}

int getfilename(char* a, char* s , int i){
    while(s[i]&&s[i]==' '){i++;}
    if(!s[i]){printf("no file specified");}
    int j = 0 ;
    while(s[i]&&s[i]!=' '){a[j]=s[i];j++;i++;}
    a[j]='\0';
    return j;
}


void RunInstruction(char** token,int token_count){
    int saved_stdout = dup(1);
    int saved_stdin = dup(1);
    int num_r = 1 ;
    if(token_count>1) { num_r = find_repeat(token);if(num_r!=0){token=&token[2];token_count-=2;} if(num_r==0){num_r=1;} }
    int input_flag = 0 , input_file_no = 0 ;
    char* input_file = (char*)malloc(sizeof(char)*100);
    int output_flag = 0 , output_file_no = 0 ;
    int append=0;
    char* output_file = (char*)malloc(sizeof(char)*100);
    
    if(strcmp(token[0],"echo")==0){
        for(int repeat=0;repeat<num_r;repeat++){
            echo(token,token_count);

        }
        for(int i=0;i<token_count;i++){strcpy(token[i],"\0");}
        return;
    }


    for(int i=0 ; i<token_count-1;i++){
        if(strcmp(token[i],"<")==0){input_flag=1;strcpy(input_file,token[i+1]);i++;}
        else if(strcmp(token[i],">")==0){output_flag=1;strcpy(output_file,token[i+1]);i++;}
        else if(strcmp(token[i],">>")==0){output_flag=1;append=1;strcpy(output_file,token[i+1]);i++;}
    }
    
    if(output_flag){output_file_no = open_output_file(output_file,append);token_count-=2;}
    if(input_flag){input_file_no = open_input_file(input_file);token_count-=2;}
    for(int repeat=0;repeat<num_r;repeat++){
        ProcessInstruction(token,token_count);
        
    }
    for(int i=0;i<token_count;i++){strcpy(token[i],"\0");}

    dup2(saved_stdout, 1);
    close(saved_stdout);
    dup2(saved_stdin, 0);
    close(saved_stdin);
    fflush(stdout);
    fflush(stdin);
}

void ProcessInstruction(char** token,int token_count){
    if(strcmp(token[0],"echo")==0){echo(token,token_count);}
    else if(strcmp(token[0],"pwd")==0){pwd(token,token_count);}
    else if(strcmp(token[0],"cd")==0){cd(token,token_count);}
    else if(strcmp(token[0],"ls")==0){ls(token,token_count);}
    else if(strcmp(token[0],"pinfo")==0){pinfo(token,token_count);}
    else if(strcmp(token[0],"jobs")==0){jobs(token,token_count);}
    else if(strcmp(token[0],"sig")==0){sig(token,token_count);}
    else if(strcmp(token[0],"fg")==0){fg(token,token_count);}
    else if(strcmp(token[0],"bg")==0){bg(token,token_count);}
    else if(strcmp(token[0],"exit")==0){exit(0);}
    else if(strcmp(token[0],"history")==0){print_history(token,token_count);}
    else{system_command(token,token_count);}
    for(int i=0;i<token_count;i++){strcpy(token[i],"\0");}

} 


void print_output_file(char *file_name){
    FILE* fp = fopen(file_name,"r");
    char c ;
    while((c = fgetc(fp))!=EOF){
        printf("%c",c);
    }
    fclose(fp);
}

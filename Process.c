#include"header.h"






int run_process(char* arg[100],int n , int background){
    pid_t pid; 
    pid = fork(); 
    if(pid < 0){  
        printf("FORK UNSUCCESSFUL\n");
        return -1 ;
    }
    
    if (pid == 0){
        if (execvp(arg[0],arg) < 0) {     
            printf("ERROR: command not found\n");
            return -1;
        }
    }
    else if (!background){
        tcsetpgrp(shell, getpid());
        foreground_process_id = getpid();
        wait(NULL);
        foreground_process_id = shell ;
        tcsetpgrp(shell, shell);
    }
    if(background){
    printf("%d\n",pid);
    struct bg_process current_bg_process ;
    strcpy(current_bg_process.command,arg[0]);
    current_bg_process.index = num_process+1;
    current_bg_process.pid = pid;
    current_bg_process.status = 1 ;
    store_process(current_bg_process);
    num_process++;
    }
    for(int i=0;i<n;i++){strcpy(arg[i],"\0");}
    
}



void pinfo(char** token , int token_count) {
        pid_t current_pid;
        
        if(token_count == 1){current_pid = getpid();}
        else current_pid = atoi(token[1]);
   
        char path[10000];
        char file_read[10000];
        
        printf("pid -- %d\n", current_pid);
        
        sprintf(path, "/proc/%d/status", current_pid);

        FILE* fp ;
        
        int length = 0 ;
        char c ;
        sprintf(path, "/proc/%d/statm", current_pid);
        fp = fopen(path, "r");
        if(!fp) {
                perror("Error while opening the proc/pid/statm file\n");

        }
        else {
                while((c=fgetc(fp))!=' '){
                    file_read[length] = c;
                    length++;
                }
                file_read[length] = '\0';
                printf("Memory -- %s\n", file_read);
                fclose(fp);
        }
        sprintf(path, "/proc/%d/status", current_pid);
        fp = fopen(path, "r");
        if(!fp) {
                perror("Error while opening the proc/pid/status file\n");

        }
        else{
            char state='o';
            char dummy;
            fgets(file_read,256, fp);
            fgets(file_read, 256, fp);
            fgets(file_read, 256, fp);
            state = file_read[7] ;
            printf("Process Status -- %c\n", state);
            fclose(fp);
        }
        sprintf(path, "/proc/%d/exe", current_pid);
        length = -1;
        length = readlink(path, file_read, 9999);
        if(length==-1) {
                perror("Error while opening the proc/pid/exe file\n");
        }
        else {
                file_read[length] = '\0';
                printf("Executable Path -- %s\n", file_read);
        }
}



void signal_handler(int sig){
    int index = 0 ;
    int current_pid ;
    int status ;
    if(sig==SIGINT){
        if(foreground_process_id==shell){
            signal(SIGINT,SIG_IGN);              
            signal(SIGINT, signal_handler);      
        }
        else{
            kill(foreground_process_id,9);
            signal(SIGINT,SIG_IGN);              
            signal(SIGINT, signal_handler); 
        }
    }
    if(sig==SIGTSTP){
        if(foreground_process_id==shell){
            signal(SIGTSTP,SIG_IGN);              
            signal(SIGTSTP, signal_handler);      
        }
        else{
            kill(foreground_process_id,20);
            signal(SIGTSTP,SIG_IGN);              
            signal(SIGTSTP, signal_handler); 
        }
    }
     while((current_pid = waitpid(-1,&status,WNOHANG))>0){
        int flag=0;
        for(int i=0;i<num_bg_process;i++){
            if(flag){process_list[i-1]=process_list[i];}
            if(process_list[i].status==0){continue;}
            if(process_list[i].pid==current_pid){index=i;flag=1;}
        }
        if(flag){num_bg_process--;}
        if(!flag){return;}
        if(WIFEXITED(status)){          
            printf("\n%s with pid %d exited normally\n", process_list[index].command, process_list[index].pid);
        }
        else{
            printf("\n%s with pid %d exited abnormally\n", process_list[index].command, process_list[index].pid);
        }
    }
}


void store_process(struct bg_process a){
    num_bg_process++;
    process_list = realloc(process_list,sizeof(struct bg_process)*num_bg_process);
    process_list[num_bg_process-1]=a;
    int j = num_bg_process - 1 ;
    while(j&&strcmp(process_list[j].command,process_list[j-1].command)<0){
        struct bg_process temp = process_list[j];
        process_list[j] = process_list[j-1] ;
        process_list[j-1] = temp ;
        j--;
    }

}


void jobs(char** token , int token_count){
    int r=1,s=1;
    if(token_count>1){
        if(strcmp(token[1],"-r")==0){s=0;}
        if(strcmp(token[1],"-s")==0){r=0;}
    }
    for(int i=0;i<num_bg_process;i++){
        char * path = (char*) malloc(sizeof(char)*100);
        char * buf = (char*) malloc(sizeof(char)*1000);
        sprintf(path, "/proc/%d/status", process_list[i].pid);
        FILE* fp = fopen(path, "r");
        char state='o';
        char dummy;
        fgets(buf,256, fp);
        fgets(buf, 256, fp);
        fgets(buf, 256, fp);
        state = buf[7] ;
        fclose(fp);

        if(state=='T'&&s){
        printf("[%d]    Stopped %s [%d]\n",process_list[i].index,process_list[i].command,process_list[i].pid);
        }
        else if(r){
        printf("[%d]    Running %s [%d]\n",process_list[i].index,process_list[i].command,process_list[i].pid);
        }
    }
}

void sig(char** token , int token_count){
    int job_number , sig_number ;
    if(token_count>1){
        job_number = atoi(token[1]);
    }
    else{
        printf("Job number not specified\n");
        return;
    }
    if(token_count>2){
        sig_number = atoi(token[2]);
    }
    else{
        printf("signal number not specified\n");
        return;
    }
    int flag=0,pid;
    for(int i=0;i<num_bg_process;i++){
        if(process_list[i].index==job_number){
            pid=process_list[i].pid;
            flag=1;
            
        }
    }
    if(!flag){
        printf("No process with specified job number");
        return;
    }
    
    printf("sig_number = %d\njob_number = %d\npid = %d\n",sig_number,job_number,pid);
    kill(pid,sig_number);
}


void fg(char** token , int token_count) {
    if(token_count==1) {
        printf("No job specified\n");
        return;
    }       
    int index ;
    int pid ;
    int flag = 0 ;
    int  job_number = atoi(token[1]);
    int status;
    for(int i=0;i<num_bg_process;i++){
        if(flag){process_list[i-1]=process_list[i];}
        if(process_list[i].index==job_number){
            pid = process_list[i].pid;
            index = i ;
            flag=1;
            break;
        }
    }
    if(!flag){
        printf("No process with specified job number");
        return;
    }
    num_bg_process--;
    int  pgid;
    pgid = getpgid(pid);                
    tcsetpgrp(shell, pgid);                 
    
    if(killpg(pgid, SIGCONT) < 0)           
            perror("Can't get in foreground!\n");
    foreground_process_id = getpid();
    waitpid(pid, &status, WUNTRACED);       
    foreground_process_id = shell ;
    tcsetpgrp(shell, shell);
    if(!WIFSTOPPED(status)) {               
            process_list[index].status = 0;
    }
    tcsetpgrp(shell, shell);                
}

void bg(char** token , int token_count) {
    if(token_count==1) {
        printf("No job specified\n");
        return;
    }       
    int index ;
    int pid ;
    int flag = 0 ;
    int  job_number = atoi(token[1]);
    int status;
    for(int i=0;i<num_bg_process;i++){
        if(process_list[i].index==job_number){
            pid = process_list[i].pid;
            index = i ;
            flag=1;
            break;
        }
    }
    if(!flag){
        printf("No process with specified job number");
        return;
    }
    kill(process_list[index].pid,SIGCONT);        
}
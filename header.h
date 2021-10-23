#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include<sys/utsname.h>
#include <sys/stat.h>
#include <time.h>


#include<fcntl.h>
#include<errno.h>
#include <unistd.h>


//********************************************PROCESS*********************************************
struct bg_process{
    pid_t pid;
    int index;
    int status;
    char command[100];
};

struct bg_process * process_list ;
int num_bg_process ;
int num_process ;
int shell ; 
int foreground_process_id ; 
void pinfo(char** token , int token_count) ;
void store_process(struct bg_process a);
void signal_handler(int sig);    
void jobs(char** token , int token_count);
int run_process(char ** ,int n , int background);
void sig(char** token , int token_count);
void fg(char** token , int token_count) ;
void bg(char** token , int token_count) ;

//********************************************TAKEINPUT*********************************************


void ScanInput();
void ProcessInstruction(char** token,int token_count);
void RunInstruction(char** token,int token_count);
void piping_bypass(char* instruction);
void print_output_file(char* s);
char* isolate_pipeline(char* s);
char* MakeToken(char* s);
char* Getinstructions(char* s);

//********************************************BUILTIN*********************************************


void echo(char** token,int token_count);
void pwd(char** token,int token_count);
int cd(char** token,int token_count);
void ls(char** token,int token_count);
void ls_with_l(char* arg);
void system_command(char** token,int token_count);

//********************************************REDIRECTION*********************************************


int open_input_file(char* filename);
int open_output_file(char* filename,int append);

//********************************************HISTORY*********************************************


void maintain_history(char* input);
void print_history(char** token , int token_count);
void read_history();
void update_history();

//********************************************DIRECTORY*********************************************


int ChangeDir(char* s);
void SetPrevDir();
void SetHome();
void PrintDir();

//********************************************PRINTBASE*********************************************


void PrintUsername();
void PrintSysname();
void PrintBase();

//*********************************************MAIN***************************************************
void initialize();
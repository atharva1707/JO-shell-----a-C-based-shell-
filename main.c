#include"header.h"


int main(){
  initialize();
  while(1){
    PrintBase();
    ScanInput();
  }
}

void initialize(){
  num_bg_process = 0 ;
  num_process = 0 ;
  shell = getpid();
  foreground_process_id = shell ;
  SetHome();
  read_history();
  signal(SIGCHLD,signal_handler);
  signal(SIGINT,signal_handler);
  signal(SIGTSTP,signal_handler);

}
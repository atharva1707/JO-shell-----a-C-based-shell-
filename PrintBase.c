#include"header.h"

void PrintUsername(){
  char *buf;
    buf=(char *)malloc(10*sizeof(char));
    buf=getlogin();
    printf("%s",buf);
}

void PrintSysname() {
  struct utsname detect;
  uname(&detect);
  printf("%s", detect.nodename);

}

void PrintBase(){
    printf("\033[0;32m");
    PrintUsername();
    printf("@");
    PrintSysname();
    printf("\033[0;37m");
    printf(":");
    printf("\033[0;34m");
    PrintDir();
    printf("\033[0;37m");
    printf("$");
}
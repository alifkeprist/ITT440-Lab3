#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

int main(void){
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void sigquit_handler(int sig);
void eof_handler(int sig);
char s[200];
signal(SIGINT, sigint_handler);
signal(SIGQUIT, sigquit_handler);
signal(SIGTSTP, sigtstp_handler);
signal(EOF, eof_handler);

printf("Enter a string :\n");
if (fgets(s,200, stdin) ==NULL)
printf("this is a special signal handler for EOF\n");
else
printf("you entered: %s\n",s);
return 0;
}
void sigint_handler (int sig){
printf("This is a special signal hanler for SIGINT\n");}
void sigtstp_handler(int sig){
printf("this is a special signal handler for SIGTSTP\n");}
void sigquit_handler(int sig){
printf("this is a special signal handler for SIGQUIT\n");}
void eof_handler(int sig){
printf("this is a special signal handler for EOF\n");}

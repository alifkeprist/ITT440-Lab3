#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define READ 0
#define WRITE 1
#define READ_BEFORE_PIPE 2
#define READ_AFTER_PIPE 3

pid_t pid;
pid_t pid2 = 0;
int numStream = 2,processCount = 0;
int prime,numPrimes,parentPID,maxNum,operationMode;
int fd[4];

void sigHandler(int);

int main()
{
signal(SIGUSR1,sigHandler);
parentPID = getpid();

printf("Which mode of operation would you like?\n");
printf("1 : Request the first k primes.\n");
printf("2 : Request all of the prime numbers between 2 and k.\n");
printf(" Please enter either a '1' or a '2' >> ");
scanf("%d",&operationMode);

if (operationMode == 1){
printf("Number of primes desired >> ");
scanf("%d",&numPrimes);
}
else if (operationMode == 2){
printf("Please enter a value for k >> ");
scanf("%d",&maxNum);
}

printf("Retrieving %d primes...\n",numPrimes);

if (pipe (fd) < 0)
perror ("Error with pipe");
if ((pid = fork()) < 0){
perror("fork failure");
exit(1);
}

if (pid == 0){
processCount++;

while (1){

if (pid2 == 0){

if (processCount >= 2)
fd[READ] = fd[READ_AFTER_PIPE];

read(fd[READ],&numStream,sizeof(numStream));

processCount++;

prime = numStream;

if (operationMode == 2){
if (prime > maxNum){
kill (parentPID,SIGUSR1);

sleep(1);
}
}
printf("%d) PID #%d : Prime Number: %d\n",processCount,getpid(),prime);
fflush(stdout);

if (processCount == numPrimes){
kill (parentPID,SIGUSR1);
sleep(1);
}


fd[READ_BEFORE_PIPE] = fd[READ];

if (pipe(fd) < 0)
perror ("Error with pipe");


fd[READ_AFTER_PIPE] = fd[READ];


fd[READ] = fd[READ_BEFORE_PIPE];

if ((pid2 = fork()) < 0){
perror("fork failure");
exit(1);
}
}
else{
read(fd[READ],&numStream,sizeof(numStream));
}


if (numStream % prime != 0){
write(fd[WRITE],&numStream,sizeof(numStream));
}
}
}
else{
close(fd[READ]);

prime = numStream;
printf("%d) PID #%d : Prime Number: %d\n",processCount + 1,getpid(),prime);
fflush(stdout);

if (processCount + 1  == numPrimes)
return 0;

while(1){
numStream++;
if (numStream % prime != 0){
write(fd[WRITE], &numStream,sizeof(numStream));
}
}
}
return 0;
}
void sigHandler(int sigUsr1){
printf("All primes retrieved... Ending all processes...\n");
killpg(getpid(),SIGKILL);
}

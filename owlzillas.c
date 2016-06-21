#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "owlzilla_hdr.h"

#define SERV_PORT 10001

void * ProcessCmd(void * msg);
void ProcessLCD(void * info);
void ProcessLPWD(void * info);
void ProcessDir(void * info);
static pthread_mutex_t mutexSum;
int hash;

typedef struct clientInfo_s
{
    char Msg[MAXLINE];
    int clientFD;
    char * path;
    int hash;
} clientInfo_t;

//Program overview
/********************
* This program acts a receiver to the client program.
*   It receives resquests and responds appropriately to them
*   Next Phase: Will return files to clients.
*
*********************/
int main(int argc, char * argv[])
{
    int sockFd, listenFd, n = 0;
    struct sockaddr_in clientAdr, serverAdr;
    char msgBuf[MAXLINE]={0};
    clientInfo_t client;
    pthread_t thread;
    socklen_t cliSckLen;
    hash=0;
    listenFd=socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAdr, 0, sizeof(serverAdr));
    serverAdr.sin_family = AF_INET;
    //serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);  //possibly remove
    serverAdr.sin_port = htons(SERV_PORT);
    if(bind(listenFd, (struct sockaddr *)&serverAdr, sizeof(serverAdr))!=0)
    {
        perror("\nUnable to bind\n");
        exit(EXIT_FAILURE);
    }

    listen(listenFd, 1);
    cliSckLen = sizeof(clientAdr);
    for(;;)
    {
        sockFd = accept(listenFd, (struct sockaddr *) &serverAdr, &cliSckLen);
        memset((void*)client.Msg, 0, sizeof(client.Msg));
        memset(msgBuf, 0, sizeof(MAXLINE));
        if ((n = read(sockFd, client.Msg, MAXLINE))==0)
        {
            printf("End of file found on client side\n");
            close(sockFd);
            break;
        }
        else
        {
            pthread_mutex_init(&mutexSum, NULL);
            client.clientFD = sockFd;
            pthread_create(&thread, NULL, ProcessCmd, (void*)&client);
            pthread_detach(thread);
            pthread_mutex_destroy(&mutexSum);
        }
    }

    close(listenFd);
    return 0;
}

void * ProcessCmd(void * clientInfo)
{
    char buffer[MAXLINE] = {0};
    char * token = NULL;

    clientInfo_t funcClient = *(clientInfo_t*) clientInfo;
    funcClient.hash=0;
    printf("\nMessage received by server\n");
    strcpy(buffer, funcClient.Msg);

    token = strtok(buffer, " ");
   // printf("\nBuffer : %s --->Token: %s\n, strcmp()result:%d", buffer,token, strcmp(token, "dir"));
    pthread_mutex_lock(&mutexSum);
    if(strcmp(token, "hash")==0)
    {
        hash=1;
    }

    else if (strcmp(token, "lpwd") == 0)
    {
        ProcessLPWD(&funcClient);
    }

    else if (strcmp(token,"dir")==0)
    {
    //printf("\nDir condition checked\n");
        ProcessDir(&funcClient);
    }

    else if (strcmp(token, "lcd")==0)
    {
        funcClient.path = strtok(NULL, " ");
        ProcessLCD(&funcClient);
    }

    pthread_mutex_unlock(&mutexSum);

    close(funcClient.clientFD);
    pthread_exit(EXIT_SUCCESS);
    return 0;
}



void ProcessLPWD(void * info)
{
    char buffer[MAXLINE]={0}, retMsg[MAXLINE]={0};
    clientInfo_t lpwdInfo = *(clientInfo_t*)info;

    sprintf(retMsg, "\nWorking Server Directory is: %s \n", (char*)getcwd(buffer, MAXLINE));//possible error causation
    write(lpwdInfo.clientFD, retMsg, sizeof(retMsg));
}

void ProcessDir(void * info)
{
    clientInfo_t wbInfo = *(clientInfo_t*)info;
    FILE * stream;
    char retMsg[MAXLINE]={0};
    stream= popen(CMD_LS_POPEN, "r");
   // printf("\nIn popen function\n");
    if (stream==NULL)
    {
        perror("\nunable to exec ls command\n");
        //possibly exit
    }
    else
    {
        while(fgets(retMsg, MAXLINE, stream)!=NULL)
        {
            write(wbInfo.clientFD, retMsg, sizeof(retMsg));
            memset((void*)retMsg, 0, MAXLINE);
        }
    }
}

void ProcessLCD(void * info)
{
    clientInfo_t lcdInfo = *(clientInfo_t*)info;
    char retMsg[MAXLINE]={0}, buffer[MAXLINE]= {0};
    if (lcdInfo.path== NULL)
    {
        sprintf(retMsg, "\n Must specify a path\n");
    }
    else
    {
        if(chdir(lcdInfo.path)==-1)
            sprintf(retMsg, "\nInvalid directory path sent from client\n");
        else
            sprintf(retMsg, "\nNew Server Directory is: %s \n", (char*)getcwd(buffer, sizeof(buffer)));
    }
    write(lcdInfo.clientFD, retMsg, sizeof(retMsg));
}



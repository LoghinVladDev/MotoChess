#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define eroareSocket -1
#define eroareSetSocketOption -1
#define eroareBind -1
#define eroareListen -1
#define eroareAccept -1
#define eroareRead -1
#define eroareWrite -1
#define disconnectRead 0
#define PORT 3500   
#define gameNotFinished 1



enum printlogs{
    socketError, socketSuccess, setSocketOptionError, setSocketOptionSuccess, bindError, bindSuccess,
    listenError, listenSuccess, acceptError, acceptSuccess, readError, readDisconnect, readSuccess,
    writeError, writeSuccess, InitSuccess, InitError, ServerAcceptLoopError, ServerAcceptLoopEnd,
    mainFunctionThreadError, mainFunctionThreadClose, GetUsernameError, GetUsernameSuccess
};

enum stariPiese
{
    gol, pionAlb, cavalerAlb, nebunAlb, turlaAlb, reginaAlb, regeAlb,
    pionNegru, cavalerNegru, nebunNegru, turlaNegru, reginaNegru,
    regeNegru
};

enum playerColors
{
    null, alb, negru
};

struct threadData
{
    int client;
    char username[256];
};

struct pair
{
    int playerTurn;
    char * challenger;
    char * challenged;
};

typedef struct threadData threadData;
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct pair pair;

pair pairs[100];

int pairNumber;
int threadIndex;
int usersCount;
pthread_t threads[100];
char connectedUsers[100][256];

int InitServer(sockaddr_in*);
int AcceptClient(int socketDescriptor, sockaddr_in* clientInformation);
void InitVariables(sockaddr_in* serverInformation);
void PrintLog(const char* context, int code);
int waitForUsername(threadData* );
int commandLoop(threadData*);
int existsUser(char* );
void refreshUsersList(threadData*, int*);
void removeUser(char *);
int challenge(threadData*);
static void* threadMain(void*);
int getChallenge(char*, int*);
char * getUser(char*);
void refreshTurns(struct threadData*, int);

int getChallenge(char* username, int* pairNo)
{
    for(int i = 0; i < pairNumber; i++)
    {
        printf("%s %s %s\n", username, pairs[i].challenged, pairs[i].challenger);
        if(strcmp(username, pairs[i].challenger) == 0)
            return 1;
        if(strcmp(username, pairs[i].challenged) == 0)
            { *pairNo = i; return 2; }
    }
    return 0;
}

void PrintLog(const char* context, int code)
{
    char error[50];
    switch(code)
    {
        case socketError :              strcpy(error, "socket() error"); break;
        case socketSuccess :            strcpy(error, "socket() success"); break;
        case setSocketOptionError :     strcpy(error, "setsockopt() error"); break;
        case setSocketOptionSuccess :   strcpy(error, "setsockopt() success"); break;
        case bindError :                strcpy(error, "bind() error"); break;
        case bindSuccess :              strcpy(error, "bind() success"); break;
        case listenError :              strcpy(error, "listen() error"); break;
        case listenSuccess :            strcpy(error, "listen() success"); break;
        case readDisconnect :           strcpy(error, "read() lost connection"); break;
        case readError :                strcpy(error, "read() error"); break;
        case readSuccess :              strcpy(error, "read() success"); break;
        case writeError :               strcpy(error, "write() error"); break;
        case writeSuccess :             strcpy(error, "write() success"); break;
        case InitError :                strcpy(error, "Init() error"); break;
        case InitSuccess :              strcpy(error, "Init() success"); break;
        case acceptError :              strcpy(error, "accept() error"); break;
        case acceptSuccess :            strcpy(error, "accept() success"); break;
        default :                       strcpy(error, "uncategorized error");
    }
    printf("%s %s code: %d\n", context, error, code);
    if(strstr(error, "error"))
        perror(error);
}


void InitVariables(sockaddr_in* serverInformation)
{
    serverInformation->sin_addr.s_addr = htonl(INADDR_ANY);
    serverInformation->sin_family = AF_INET;
    serverInformation->sin_port = htons(PORT);
}

int InitServer(sockaddr_in* serverInformation)
{
    int socketDescriptor, optionNumber = 1;
    sockaddr* serverInformationAddress = (sockaddr*)serverInformation;

    if( -1 == (socketDescriptor = socket(AF_INET, SOCK_STREAM, 0) ) )
    {
        PrintLog("Init Server: ", socketError);
        return -1;
    } 
    if( -1 == setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optionNumber, sizeof(int)) )
    {
        PrintLog("Init Server: ", setSocketOptionError);
        return -1;
    }   
    if( -1 == bind(socketDescriptor, serverInformationAddress, sizeof(sockaddr) ) )
    {
        PrintLog("Init Server: ", bindError);
        return -1;
    }
    if( -1 == listen(socketDescriptor, 10) )
    {
        PrintLog("Init Server: ", listenError);
        return -1;
    }
    return socketDescriptor;
}

int AcceptClient(int socketDescriptor, sockaddr_in* clientInformation)
{
    int clientAddressLength = sizeof(clientInformation);
    sockaddr* addressClientInformation = (sockaddr*)clientInformation;

    return accept(socketDescriptor, addressClientInformation, (socklen_t*)&clientAddressLength);
}
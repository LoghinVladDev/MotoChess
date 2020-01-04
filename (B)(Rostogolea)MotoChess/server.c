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


struct threadData
{
    int clientDesc;
    int threadNumber;
    char username[64];
};
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

int numberOfPlayerPairs;
int numberOfConnectedUsers;

struct user{
    int threadIndex;
    char username[64];
    int clientDesc;
    int isChallenged;
}connectedUsers[100];

struct matchNames
{
    int readyToStart;
    char user1[64];
    char user2[64];
}playerPair[50];



struct gameThread
{
    int playerDescriptor;
    int playerNumber;
    int playerturn;
    char playerUsername[64];
    struct gameThread* otherPlayer;
    pthread_mutex_t *tableLock;
    int **table;
};

//char connectedUsers[100][64];
pthread_mutex_t lock;
pthread_t threadVector[100];

void PrintLog(const char*, int);
void Init(struct sockaddr_in* , int* );
void ServerAcceptLoop(int* ,struct sockaddr_in*, struct sockaddr_in*);
void GetUsername(struct threadData*);
void ThreadGetCommands(struct threadData*);
void DeleteFromClients(char* username);
void DebugPrintUsers();
void ChallengeUser(char* username);
void LinkPlayers(struct threadData* player1Thread, char* username, int* descriptor);
void RemoveChallengeStatus(char* username);
void InitChessGame(char* player1name, char* player2name);
void GameLoop(int, int);


bool IsUserConnected(char* username);

static void* gameThreadStartRoutine(void* playerDataThreadUncasted);
static void* mainFunctionThread(void*);

int GetChallenger(char* username, char* challengerUsername);
int GetChallenge(char * username);
int GetPlayerDescriptor(char* username);
int GetPair(char* username);

int GetPlayerDescriptor(char* username)
{
    for (size_t i = 0; i < numberOfConnectedUsers; i++)
    {
        if(!strcmp(connectedUsers[i].username, username))
            return connectedUsers[i].clientDesc;
    }
}

void RemoveChallengeStatus(char* username)
{
for (size_t i = 0; i < numberOfConnectedUsers; i++)
{
    if(!strcmp(username, connectedUsers[i].username))
    {
        connectedUsers[i].isChallenged = 0;
        return;
    }
}

}

void ChallengeUser(char* username)
{
    for (size_t i = 0; i < numberOfConnectedUsers; i++)
    {
        if(!strcmp(connectedUsers[i].username, username))
        { 
            connectedUsers[i].isChallenged = 1;
            return;
        }
    }
}

void LinkPlayers(struct threadData *player1Thread, char* username, int* descriptor)
{
    pthread_mutex_lock(&lock);


    
    pthread_mutex_unlock(&lock);
}

void DeleteFromClients(char* username)
{

    pthread_mutex_lock(&lock);

    int i = 0;

    for (i = 0; i < numberOfConnectedUsers; i++)
    {
        if(!strcmp(username, connectedUsers[i].username))
            break;
    }
    numberOfConnectedUsers--;
    for(; i < numberOfConnectedUsers; i++)
    {
        connectedUsers[i].threadIndex = connectedUsers[i+1].threadIndex;
        connectedUsers[i].clientDesc = connectedUsers[i+1].clientDesc;
        connectedUsers[i].isChallenged = connectedUsers[i+1].isChallenged;
        strcpy(connectedUsers[i].username, connectedUsers[i + 1].username);
    }
    connectedUsers[numberOfConnectedUsers].username[0] = 0;


    pthread_mutex_unlock(&lock);
}

bool IsUserConnected(char* username)
{
    for (size_t i = 0; i < numberOfConnectedUsers; i++)
    {
        if(!strcmp(connectedUsers[i].username, username))
            return true;
    }
    return false;
}


int GetChallenger(char* username, char* challengerUsername)
{
    for (size_t i = 0; i < numberOfPlayerPairs; i++)
    {
        if(!strcmp(username, playerPair[i].user2))
        {
            strcpy(challengerUsername, playerPair[i].user1);
            //printf("%s %s\n", challengerUsername, playerPair[i].user1);
            return i;
        }
    }
    return -1;
}

int GetPair(char* username)
{
    for (size_t i = 0; i < numberOfPlayerPairs; i++)
    {
        if(!strcmp(username, playerPair[i].user1))
            return i;
    }
    
}

void DebugPrintUsers()
{
    for (size_t i = 0; i < numberOfConnectedUsers; i++)
    {
        printf("Thread %d has user %s with descriptor %d\n", connectedUsers[i].threadIndex, connectedUsers[i].username, connectedUsers[i].clientDesc);
    }
    
}

int GetChallenge(char* username)
{
    for (size_t i = 0; i < numberOfConnectedUsers; i++)
    {
        if(!strcmp(username, connectedUsers[i].username))
            return connectedUsers[i].isChallenged;
    }
    
}

/*void GameLoop(int player1Desc, int player2Desc)
{
    int playerTurn = 1;
    while(gameNotFinished)
    {
        
    }
}*/

void* gameThreadStartRoutine(void* playerDataThreadUncasted)
{
    struct gameThread playerDataThread = *((struct gameThread*)playerDataThreadUncasted);
    char messageBuffer[64];
    while(gameNotFinished)
    {
        read(playerDataThread.playerDescriptor, messageBuffer, 64);
        if(!strcmp(messageBuffer, "GETSTATE"))
        {
            write(playerDataThread.playerDescriptor, &playerDataThread.playerturn, 4);
            if(playerDataThread.playerturn == 1)
            {
                pthread_mutex_lock(playerDataThread.tableLock);

                read(playerDataThread.playerDescriptor, messageBuffer, 64);
                read(playerDataThread.playerDescriptor, playerDataThread.table, 256);
                playerDataThread.playerturn = 0;
                playerDataThread.otherPlayer->playerturn = 1;

                pthread_mutex_unlock(playerDataThread.tableLock);
            }
        }

    }
}

void InitChessGame(char* player1name, char* player2name)
{
    int player2 = GetPlayerDescriptor(player2name);
    int player1 = GetPlayerDescriptor(player1name);

    printf("Jucatori conectati la joc : alb %s ... negru %s\nDescriptori: alb : %d ... negru : %d\n", player1name, player2name, player1, player2);

    char bufferGolireSock[64];

    int confirm = 2;
    read(player1, bufferGolireSock, 64);
    write(player1, &confirm, 4);

    write(player2, "ai intrat in joc ca si negru", sizeof("ai intrat in joc ca si negru"));
    write(player1, "ai intrat in joc ca si alb", sizeof("ai intrat in joc ca si alb"));

    pthread_mutex_t tableLock;
    int table[8][8];
    pthread_t player1LoopThread, player2LoopThread;
    struct gameThread player1DataThread, player2DataThread;
    memset(&player1DataThread, 0, sizeof(struct gameThread));  
    memset(&player2DataThread, 0, sizeof(struct gameThread));  
    memset(table, 0 , sizeof(table));
    for (size_t i = 0; i < 8; i++)
    {
        table[1][i] = pionNegru;
        table[6][i] = pionAlb;
    }
    table[7][0] = table[7][7] = turlaAlb;
    table[7][1] = table[7][6] = cavalerAlb;
    table[7][2] = table[7][5] = nebunAlb;
    table[7][3] = reginaAlb;
    table[7][4] = regeAlb;

    table[0][0] = table[0][7] = turlaNegru;
    table[0][1] = table[0][6] = cavalerNegru;
    table[0][2] = table[0][5] = nebunNegru;
    table[0][3] = reginaNegru;
    table[0][4] = regeNegru;

    player1DataThread.playerDescriptor = player1;
    player1DataThread.playerNumber = alb;
    player1DataThread.playerturn = 1; 
    strcpy(player1DataThread.playerUsername, player1name);
    player1DataThread.otherPlayer = &player2DataThread;
    player1DataThread.tableLock = &tableLock;
    player1DataThread.table = table;

    player2DataThread.playerDescriptor = player2;
    player2DataThread.playerNumber = negru;
    player2DataThread.playerturn = 0;  
    strpcy(player2DataThread.playerUsername, player2name);
    player2DataThread.otherPlayer = &player1DataThread;
    player2DataThread.tableLock = &tableLock;
    player2DataThread.table = table;

    pthread_create(&player1LoopThread, NULL, gameThreadStartRoutine, &player1DataThread);
    pthread_create(&player2LoopThread, NULL, gameThreadStartRoutine, &player2DataThread);

    //GameLoop(&player1, &player2);
}

void ThreadGetCommands(struct threadData* localdata)
{

    int challenge = 0;
    char challenger[64];
    char connectedUsersString[100][64];
    int connectionStatus = 1;
    char reportString[] = "In thread function ThreadGetCommands(struct threadData*):";
    char commandBuffer[128];
    while(true)
    {
        challenge = GetChallenge(localdata->username);

        int pairNumber = -1;
        pairNumber = GetPair(localdata->username);

        if(playerPair[pairNumber].readyToStart == 0)
        {

            if( ( connectionStatus = read(localdata->clientDesc, commandBuffer, sizeof(commandBuffer) ) ) == readError )
            {
                PrintLog(reportString, readError);
            }
            else
            {
                PrintLog(reportString, readSuccess);
                if( connectionStatus == 0 )
                {
                    PrintLog(reportString, readDisconnect);
                    break;
                }
            }

            printf("Received : %s\n", commandBuffer);
            fflush(stdout);

            if(!strcmp(commandBuffer,"REFRESH"))
            {
                pthread_mutex_lock(&lock);
                for (size_t i = 0; i < numberOfConnectedUsers; i++)
                {
                    strcpy(connectedUsersString[i], connectedUsers[i].username);
                }
                connectedUsersString[numberOfConnectedUsers][0] = 0;
                
                DebugPrintUsers();
                pairNumber = GetPair(localdata->username);
                if(playerPair[pairNumber].readyToStart == 1)
                    challenge = 2;
                write(localdata->clientDesc, &challenge, 4);
                if(playerPair[pairNumber].readyToStart == 0)
                write(localdata->clientDesc, connectedUsersString, sizeof(connectedUsersString));
                if(challenge==1)
                {
                    int challengeAcceptStatus = 0;
                    memset(challenger, 0 ,64);
                    pairNumber = GetChallenger(localdata->username, challenger);
                    write(localdata->clientDesc, challenger, 64);
                    read(localdata->clientDesc, &challengeAcceptStatus, 4);
                    printf("User %s challenged by %s. Accepted status : %d\n", localdata->username, challenger, challengeAcceptStatus);
                    RemoveChallengeStatus(localdata->username);
                    //int player2Descriptor = GetPlayerDescriptor(challenger);
                    //write(player2Descriptor, &challengeAcceptStatus, 4);
                    if(challengeAcceptStatus == 1)
                    {
                        playerPair[pairNumber].readyToStart = 1;
                        InitChessGame(playerPair[pairNumber].user1, playerPair[pairNumber].user2);
                    }
                }
                pthread_mutex_unlock(&lock);
            }
            if(strstr(commandBuffer, "DISCONNECT"))
            {
                DeleteFromClients(commandBuffer + strlen("DISCONNECT "));
                close(localdata->clientDesc);
                return;
            }
            if(strstr(commandBuffer, "CHALLENGE "))
            {
                //int player1 = GetPlayer2Descriptor(localdata->username);
                pthread_mutex_lock(&lock);

                //ChallengeUser(localdata->username);
                ChallengeUser(commandBuffer + strlen("CHALLENGE "));
                strcpy(playerPair[numberOfPlayerPairs].user1, localdata->username);
                strcpy(playerPair[numberOfPlayerPairs++].user2, commandBuffer + strlen("CHALLENGE "));
                //int player2 =  GetPlayer2Descriptor(commandBuffer+ strlen("CHALLENGE "));// dup(GetPlayer2Descriptor(commandBuffer+ strlen("CHALLENGE")));

                //printf("p1 : %s, p2 : %s\n", playerPair[numberOfPlayerPairs-1].user1, playerPair[numberOfPlayerPairs-1].user2);

                //write(player1, "test1", sizeof("test1"));
                //write(player2, "test2", sizeof("test2"));


                pthread_mutex_unlock(&lock);
                //LinkPlayers(localdata, commandBuffer + strlen("CHALLENGE "), &player2);
            }
        }
        else
        {
            sleep(1);
        }
        
    }
}

int main()
{
    struct sockaddr_in server, client;
    int socketDescriptor = 0;

    memset(&server, 0, sizeof(struct sockaddr_in));
    memset(&client, 0 , sizeof(struct sockaddr_in));

    Init(&server, &socketDescriptor);

    ServerAcceptLoop(&socketDescriptor, &server, &client);
    
    return 0;
}

void ServerAcceptLoop(int* socketDescriptor, struct sockaddr_in * server, struct sockaddr_in * client)
{
    int clientDescriptor;
    int lengthOfClientAddres = sizeof( *client );
    int threadIndex = 0;
    char reportString[] = "In function ServerAcceptLoop(int*, struct sockaddr_in*, struct sockaddr_in*):";
    struct threadData *currentThreadParam;
    
    memset(threadVector, 0, sizeof(threadVector));
    memset(connectedUsers, 0, sizeof(connectedUsers));

    while( true )
    {
        
        if( ( clientDescriptor = accept( *socketDescriptor, (struct sockaddr*)client, (socklen_t*)&lengthOfClientAddres ) ) == eroareAccept )
        {
            PrintLog(reportString, acceptError);
            continue;
        }
        else
        {
            PrintLog(reportString, acceptSuccess);
        }

        currentThreadParam = ( struct threadData* ) malloc ( sizeof( struct threadData ) );

        memset(currentThreadParam, 0 ,sizeof( struct threadData ));
        memset(currentThreadParam->username, 0, sizeof(currentThreadParam->username));

        currentThreadParam->clientDesc = clientDescriptor;
        currentThreadParam->threadNumber = threadIndex;

        pthread_create( &threadVector[threadIndex ++ ], NULL, mainFunctionThread, (void*) currentThreadParam );
    }
}

void GetUsername( struct threadData* localdata)
{
    char reportString[] = "In thread function GetUsername(char*):";
    char messageBuffer[64];

    memset(messageBuffer, 0, sizeof(messageBuffer));

    if( read(localdata->clientDesc, messageBuffer, sizeof(messageBuffer)) == eroareRead )
    {
        PrintLog(reportString, readError);
    }
    else
    {
        PrintLog(reportString, readSuccess);
    }

    strcpy(localdata->username, messageBuffer);
    strcpy(messageBuffer, "recieveduser");

    if( write(localdata->clientDesc, messageBuffer, sizeof(messageBuffer)) == eroareWrite )
    {
        PrintLog(reportString, writeError);
    }
    else
    {
        PrintLog(reportString, writeSuccess);
    }

    PrintLog(reportString, GetUsernameSuccess);
}

void *mainFunctionThread( void* uncastedParameter )
{
    char reportString[] = "In thread function mainFunctionThread(void*):";
    struct threadData localdata = *( ( struct threadData* ) uncastedParameter );
    
    GetUsername(&localdata);

    pthread_mutex_lock(&lock);
    connectedUsers[numberOfConnectedUsers].clientDesc = localdata.clientDesc;
    connectedUsers[numberOfConnectedUsers].threadIndex = localdata.threadNumber;
    connectedUsers[numberOfConnectedUsers].isChallenged = 0;
    strcpy(connectedUsers[numberOfConnectedUsers++].username, localdata.username);
    DebugPrintUsers();
    pthread_mutex_unlock(&lock);

    ThreadGetCommands(&localdata);

    pthread_detach(pthread_self());
    pthread_cancel(pthread_self());

    return (NULL);
}

void Init(struct sockaddr_in * server, int* socketDescriptor)
{
    int optionToggle = 1;
    char reportString[] = "In function Init(struct sockaddr*, int*):";

    server->sin_addr.s_addr = htonl(INADDR_ANY);
    server->sin_family = AF_INET;
    server->sin_port = htons(PORT);

    if( ( *socketDescriptor =  socket ( AF_INET, SOCK_STREAM, 0 ) ) == eroareSocket  )
    {
        PrintLog(reportString, socketError);
        PrintLog(reportString, InitError);
        exit(InitError);
    }
    else
    {
        PrintLog(reportString, socketSuccess);
    }

    if( setsockopt( *socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optionToggle, sizeof(int) ) == eroareSetSocketOption )
    {
        PrintLog(reportString, setSocketOptionError);
        PrintLog(reportString, InitError);
        exit(setSocketOptionError);
    }
    else
    {
        PrintLog(reportString, setSocketOptionSuccess);
    }

    if( bind( *socketDescriptor, (struct sockaddr* )server, sizeof(struct sockaddr) ) == eroareBind )
    {
        PrintLog(reportString, bindError);
        PrintLog(reportString, InitError);
        exit(bindError);
    }
    else
    {
        PrintLog(reportString, bindSuccess);
    }

    if( listen( *socketDescriptor, 10 ) == eroareListen )
    {
        PrintLog(reportString, listenError);
        PrintLog(reportString, InitError);
        exit(listenError);
    }
    else
    {
        PrintLog(reportString, listenSuccess);
    }
    
    PrintLog(reportString, InitSuccess);
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
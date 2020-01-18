#include "serverData.h"

int main()
{
    sockaddr_in serverInformation;
    sockaddr_in clientInformation;
    threadData * newThreadInformation;
    int socketDescriptor = 0, clientDescriptor;

    InitVariables(&serverInformation);

    if( -1 == (socketDescriptor = InitServer(&serverInformation)) )
    {
        PrintLog("Main: ", InitError);
        return 0;
    }
    while( 1 )
    {
        if( -1 == ( clientDescriptor = AcceptClient(socketDescriptor, &clientInformation ) ) )
            continue;
        newThreadInformation = (threadData*)malloc(sizeof(threadData));

        newThreadInformation->client = clientDescriptor;
        
        pthread_create(&threads[threadIndex++], NULL, threadMain, (void*)newThreadInformation);
    }
}   

void* threadMain(void* parameter)
{
    threadData* threadInformation = (threadData*)parameter;
    printf("%d\n",threadInformation->client);
    while(waitForUsername(threadInformation) != 1);
    printf("entering command loop\n");
    fflush(stdout);
    while(1)
        commandLoop(threadInformation);

}

int waitForUsername(threadData* threadInformation)
{
    printf("login again\n");
    fflush(stdout);
    char username[256];
    if( read(threadInformation->client, username, 256) <= 0 )
        pthread_cancel(pthread_self());

    if(existsUser(username))
    {
        if( write(threadInformation->client, "existsUser", 256) <= 0)
        {
            pthread_cancel(pthread_self());
        }
        return 0;
    }

    strcpy(connectedUsers[usersCount++], username);
    strcpy(threadInformation->username, username);

    if( write(threadInformation->client, "loginReady", 256) <= 0) 
    {
        removeUser(username);
        pthread_cancel(pthread_self());
    }
    
    printf("user logged in\n");
    fflush(stdout);
    return 1;
}

int commandLoop(threadData* threadInformation)
{
    int pairNo = 0;
    fflush(stdout);
    char command[256];
    if( read(threadInformation->client,command, 256 ) <= 0 )
    {
        removeUser(threadInformation->username);
        pthread_cancel(pthread_self());
    }
    printf("User %s's command is : %s\n", threadInformation->username, command);
    fflush(stdout);
    if( strcmp(command, "refreshUsers") == 0 )
    {
        refreshUsersList(threadInformation, &pairNo);
    }
    if( strcmp(command, "challengeUser") == 0 )
    {
        pairNo = challenge(threadInformation);
    }
    if( strcmp(command, "refreshTurns") == 0 )
    {
        refreshTurns(threadInformation, pairNo);
    }
}

void refreshTurns(struct threadData* threadInformation, int pairNo)
{
    write(threadInformation->client, &pairNo, 4);
}

int challenge(threadData* threadInformation)
{
    char command[256];
    if ( 0>= read(threadInformation->client, command, 256) )
    {
        removeUser(threadInformation->username);
        pthread_cancel(pthread_self());
    }
    pairs[pairNumber].challenger = threadInformation->username;
    //printf("%s\n", command);
    pairs[pairNumber].playerTurn = 1;
    pairs[pairNumber++].challenged = getUser(command);
    //pairs[pairNumber].waitStatus = 1;
}

char *getUser(char* username)
{
    for(int i = 0; i< usersCount; i++)
    {
        if( strcmp(connectedUsers[i], username)  ==0 )
            return strdup(connectedUsers[i]);
    }
    return NULL;
}

void refreshUsersList(threadData* threadInformation, int* pairNo)
{
    if( 0 >= write( threadInformation->client, &usersCount, 4 ) )
    {
        removeUser(threadInformation->username);
        pthread_cancel(pthread_self());
    }
    for(int i = 0; i < usersCount; i++)
    {
        if ( 0>= write(threadInformation->client, connectedUsers[i], 256) )
        {
            removeUser(threadInformation->username);
            pthread_cancel(pthread_self());
        }
    }
    int waitsForChallenge = getChallenge(threadInformation->username, pairNo);
    printf("%s : %d\n",threadInformation->username, waitsForChallenge);
    if(0>= write(threadInformation->client, &waitsForChallenge, 4))
    {
        removeUser(threadInformation->username);
        pthread_cancel(pthread_self());
    }
}

void removeUser(char* username)
{
    int whichUser = 0;
    for(int i = 0; i < usersCount; i++)
    {
        if(!strcmp(connectedUsers[i], username))
        {
            whichUser = i;
            break;
        }
    }
    for(int i = whichUser; i < usersCount-1; i++)
    {   
        strcpy(connectedUsers[i], connectedUsers[i+1]);
    }
    usersCount--;
}

int existsUser(char *username)
{
    for(int i = 0; i < usersCount; i++)
    {
        if(!strcmp(connectedUsers[i], username))
            return 1;
    }
    return 0;
}
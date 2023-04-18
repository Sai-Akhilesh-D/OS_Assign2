#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define CONNECT_CHANNEL 100
#define MAX_CLIENT_NAME 100
#define MAX_CLIENTS 100
#define SERVER_BUSY 1
#define SERVER_READY 5
#define SUCCESSFULL 1
#define USER_EXIST 5
#define CLIENT_LIMIT_EXCEEDED 2
#define CLIENT_REQUESTED 1
#define FINISHED 0
#define ACK 0
#define NACK 1
typedef struct Response
{
    int key;
    int status;
    int server_reply;
    int ack;
}response;
typedef struct Request
{
    char name[MAX_CLIENT_NAME];
    int client_status;

}request;
typedef struct Channel
{
    response Server_response;
    request Client_request;
    
}channel;
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Wrong arguments\n");
        exit(1);
    }
    channel *connection;
    int shmid;

    key_t key =CONNECT_CHANNEL;
    // shmget returns an identifier in shmid
    if ((shmid = shmget(key,sizeof(channel), 0666)) < 0) {
        perror("Server not reachable.");
        exit(1);
    }
  
    // shmat to attach to shared memory
    connection = (channel*) shmat(shmid,(void*)0,0);
    
    while(connection->Server_response.status!= SERVER_READY )
    {
        sleep(1);
    }
    connection->Server_response.ack=NACK;
    strcpy(connection->Client_request.name, argv[1]);
    connection-> Client_request.client_status=CLIENT_REQUESTED;
    while(connection->Server_response.ack == NACK)
    {
        sleep(1);
    }
    while(connection->Server_response.status == SERVER_BUSY)
    {
        sleep(1);
    }
    if(connection->Server_response.server_reply == USER_EXIST)
    {
        printf("user already existed with name as %s\n",connection->Client_request.name);
        
    }
    else if( connection->Server_response.server_reply == SUCCESSFULL)
    {
        printf("Connection successfull with %d \n",connection->Server_response.key);
    }
    else if(connection->Server_response.server_reply== CLIENT_LIMIT_EXCEEDED)
    {
        printf("Server too busy, Try after some time\n");
    }
    //detach from shared memory 
    shmdt(connection);
}
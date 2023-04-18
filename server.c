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
#define NOT_REPLIED 10
#define ACK 0
#define NACK 1
typedef struct Client_data 
{
    /* data */
    char client_name[MAX_CLIENT_NAME];
    int key;
    int request_count;

}Client_data;
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
char check;
void* test()
{

    scanf("%c",&check);
    printf("%c hello\n",check);
    pthread_exit(0);
}
int main(int argc, char *argv[])
{
    pthread_t th1;
    check='\0';
    pthread_create(&th1, NULL, test,NULL);
    Client_data client_list[MAX_CLIENTS];
    int no_of_clients=0;
    key_t key =CONNECT_CHANNEL;
    channel *connection;

    // shmget returns an identifier in shmid
    int shmid = shmget(key,sizeof(channel),0666|IPC_CREAT);
    
    // shmat to attach to shared memory
    connection = (channel*) shmat(shmid,(void*)0,0);
    connection->Server_response.ack=NACK;
    connection->Server_response.status = SERVER_READY;
    while(check == '\0'){
    printf("Server is waiting for client\n");
    bool temp=false;
    while(connection->Client_request.client_status != CLIENT_REQUESTED && check=='\0')
    {
    
        sleep(1);
    }
    if(check!= '\0')break;
    connection->Server_response.ack=ACK;
    connection-> Server_response.status = SERVER_BUSY;
    printf("Server is processing %s request\n", connection->Client_request.name);
    for(int i=0 ;i< MAX_CLIENTS ;i++)
    {
        if(!strcmp(connection-> Client_request.name, client_list[i].client_name ))
        {
            connection->Server_response.server_reply=USER_EXIST;
            printf("user already existed with name as %s\n",connection->Client_request.name);
            break;
        }

    }
    if(connection->Server_response.server_reply != USER_EXIST)
    {
        connection->Server_response.server_reply = SUCCESSFULL;

        connection->Server_response.key= 555;
        printf("Communication channel for %s is created\n",connection->Client_request.name);
        strcpy(client_list[no_of_clients].client_name , connection->Client_request.name);
        client_list[no_of_clients++].key= connection->Server_response.key;

    }
    connection->Server_response.status =SERVER_READY;
    connection->Client_request.client_status= FINISHED;
    }
    pthread_join(th1, NULL);
    shmdt(connection);
    shmctl(shmid, IPC_RMID, NULL);
}
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
typedef struct Channel
{
    int key;
    char name[MAX_CLIENT_NAME];
    int status;
    int server_reply;
    int client_status;
    int ack;
}channel;
char check;
void* test()
{

    scanf("%c",&check);
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
    connection->ack=NACK;
    connection-> status = SERVER_READY;
    while(1){
    printf("Server is waiting for client\n");
    while(connection->client_status != CLIENT_REQUESTED)
    {
        sleep(1);
    }
    connection->ack=ACK;
    connection-> status = SERVER_BUSY;
    printf("Server is processing %s request\n", connection->name);
    for(int i=0 ;i< MAX_CLIENTS ;i++)
    {
        if(!strcmp(connection-> name, client_list[i].client_name ))
        {
            connection->server_reply=USER_EXIST;
            printf("user already existed with name as %s\n",connection->name);
            break;
        }

    }
    if(connection->server_reply != USER_EXIST)
    {
        connection->server_reply= SUCCESSFULL;

        connection->key = 555;
        printf("Communication channel for %s is created\n",connection->name);
        strcpy(client_list[no_of_clients].client_name , connection->name);
        client_list[no_of_clients++].key= connection->key;

    }
    connection->status =SERVER_READY;
    connection->client_status= FINISHED;
    }
    shmdt(connection);
}
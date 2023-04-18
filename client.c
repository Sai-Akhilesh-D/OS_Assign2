#include "myheader.h"
typedef struct Response
{
    key_t key;
    int status;
    int server_reply;
    int ack;
}response;
typedef struct Request
{
    char name[MAX_CLIENT_NAME];
    int client_status;

}request;
typedef struct Communication
{
    int x;
    char test[100];
}communication;
typedef struct Channel
{
    response Server_response;
    request Client_request;
    sem_t sem;
    
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
    int sem_value;
    key_t key =CONNECT_CHANNEL;
    // shmget returns an identifier in shmid
    if ((shmid = shmget(key,sizeof(channel), 0666)) < 0) {
        perror("Server not reachable.");
        exit(1);
    }
  
    // shmat to attach to shared memory
    connection = (channel*) shmat(shmid,(void*)0,0);
    printf("Client waiting for connection channel\n");
        sem_getvalue(&connection->sem, &sem_value);
        printf("3sem value: %d\n",sem_value);

    sem_wait(&connection->sem);
        sem_getvalue(&connection->sem, &sem_value);
        printf("2sem value: %d\n",sem_value);

    printf("client got connection channel\n");
    sleep(5);
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
        sem_post(&connection->sem);
    }
    else if( connection->Server_response.server_reply == SUCCESSFULL)
    {
        printf("Connection successfull with %d \n",connection->Server_response.key);

        int shm;
        if ((shm = shmget(connection->Server_response.key,sizeof(communication), 0666)) < 0) {
        perror("Server not reachable.");
        exit(1);}
    communication *data_comm;
    data_comm = (communication*) shmat(shm,(void*)0,0);printf("Send a message to server \n");
        sem_getvalue(&connection->sem, &sem_value);
        printf("1sem value: %d\n",sem_value);
        sem_post(&connection->sem);
        
        sem_getvalue(&connection->sem, &sem_value);
        printf("0sem value: %d\n",sem_value);
        char x = '\0';
        int i=0;
        while(x!='\n'){
            scanf("%c",&x);
            data_comm->test[i] = x;
            i++;
        }
        
    }
    else if(connection->Server_response.server_reply== CLIENT_LIMIT_EXCEEDED)
    {
        printf("Server too busy, Try after some time\n");
        sem_post(&connection->sem);
        
    }
    
    
    //detach from shared memory 
    shmdt(connection);
}
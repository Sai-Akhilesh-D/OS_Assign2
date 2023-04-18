#include "myheader.h"
typedef struct Client_data 
{
    /* data */
    char client_name[MAX_CLIENT_NAME];
    key_t key;
    int request_count;
    pthread_t thread_number;
    bool Comm_channel_isCreated;

}Client_data;
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
char check;
void* test()
{

    scanf("%c",&check);
    pthread_exit(0);
}
void *worker(void *data)
{   
    printf("worker thread created\n");
    
    
    // int shmid;
    Client_data *client_data;
    client_data = (Client_data*) data;
    
    // if ((shmid = shmget(client_data->key,sizeof(communication), 0666)) < 0) {
    //     perror("Server not reachable.");
    //     exit(1);
    // }
    // printf("HELLO MF\n");
    // int shmid = *((int*)data);
    // communication *data_comm;
    // data_comm = (communication*) shmat(shmid,(void*)0,0);
    //     printf("wsdfghj%c",data_comm-> test[0]);
    int shmid2 = shmget(client_data->key,sizeof(communication),0666|IPC_CREAT);
        communication *data_comm;
        data_comm = (communication*) shmat(shmid2,(void*)0,0);

        data_comm->test[0] ='$';
        client_data->Comm_channel_isCreated= true;
    fflush(stdout);
    while(data_comm-> test[0]== '$')
    {
        sleep(1);
    }
    printf("%s sent %s ",client_data->client_name , data_comm->test);
    fflush(stdout);
    // printf("goodbuye\n");
    shmdt(data_comm);
    shmctl(shmid2, IPC_RMID, NULL);
    pthread_exit(0);
}
int main(int argc, char *argv[])
{
    pthread_t th1;
    pthread_t worker_thread;
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
    sem_init(&connection->sem, 1, 2);
    sem_wait(&connection->sem);
    connection->Server_response.ack=NACK;
    connection->Server_response.status = SERVER_READY;
    while(check == '\0'){
    printf("Server is waiting for client\n");
    bool temp=false;
    while(connection->Client_request.client_status != CLIENT_REQUESTED && check=='\0')
    {
    
        sleep(1);
    }
    connection->Server_response.server_reply = SUCCESSFULL;
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
        
        
        printf("Communication channel for %s is created\n",connection->Client_request.name);
        strcpy(client_list[no_of_clients].client_name , connection->Client_request.name);
        connection->Server_response.key= no_of_clients + 1234;
        // ftok(connection->Client_request.name , 69);
        // int shmid2 = shmget(connection->Server_response.key,sizeof(communication),0666|IPC_CREAT);
        client_list[no_of_clients].key= connection->Server_response.key;
        // communication *data_comm;
        // data_comm = (communication*) shmat(shmid,(void*)0,0);
        // printf("before%s",data_comm->test);
        // data_comm->test[0] ='$';
        // printf("agfshgdtfnb%c",data_comm->test[0]);
        // shmdt(data_comm);
        client_list[no_of_clients].Comm_channel_isCreated=false;
        pthread_create(&worker_thread, NULL, worker,(void*) &client_list[no_of_clients]);
        // pthread_create(&worker_thread, NULL, worker, (void*) &shmid2);
        while(!client_list[no_of_clients].Comm_channel_isCreated)sleep(1);
        client_list[no_of_clients].thread_number=worker_thread;
        no_of_clients++;
    }

    connection->Server_response.status =SERVER_READY;
    connection->Client_request.client_status= FINISHED;
    }
    pthread_join(th1, NULL);
    // pthread_join(worker_thread, NULL);

    for(int i=0; i< no_of_clients;i++)
    {
        pthread_join(client_list[i].thread_number,NULL);
    }
    sem_post(&connection->sem);
    shmdt(connection);
    shmctl(shmid, IPC_RMID, NULL);
}
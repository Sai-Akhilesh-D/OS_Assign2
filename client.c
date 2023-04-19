#include "myheader.h"
typedef struct Response
{
    key_t key;
    int status;
    int server_reply;
    int ack;
}response;
typedef struct Arithmetic{
    float x;
    float y;
    char action;
}arithmetic;
typedef struct EvenOrOdd{
    int x;
}evenOrOdd;
typedef struct IsPrime{
    int x;
}isPrime;


typedef struct com_Response
{
    char msg[100];
    float ans;
    int status;
    int ack;
}com_response;

typedef struct Request
{
    char name[MAX_CLIENT_NAME];
    int client_status;

}request;

typedef struct comRequest
{
    int request_type;
    int client_status;
    arithmetic arth;
    evenOrOdd eoo;
    isPrime ip;

}com_request;

typedef struct Communication
{
    com_response Server_response;
    com_request Client_request;
}communication;

typedef struct Channel
{
    response Server_response;
    request Client_request;
    sem_t sem;
    
}channel;

typedef struct Com_channel
{
    com_response Server_response;
    com_request Client_request;
    
}Com_channel;

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
    data_comm = (communication*) shmat(shm,(void*)0,0);
        sem_getvalue(&connection->sem, &sem_value);
        printf("1sem value: %d\n",sem_value);
        sem_post(&connection->sem);
        sem_getvalue(&connection->sem, &sem_value);
        printf("0sem value: %d\n",sem_value);
    int x = 0;
    int ik = 0;
    while(1){
        ik++;
        printf("enter the number:\n");
        scanf("%d",&x);
        printf("u entered %d\n",x);
        if(data_comm == NULL){
            printf("server stopped\n");
            exit(1);
        }
        if(x==-1){
                    data_comm->Client_request.client_status = STOP;
                    break;
        }
        else if(x==1){
            printf("Please enter two operands separated by space and the type of arthimaetic operation\n");
            data_comm->Client_request.request_type=1;
            scanf("%f",&data_comm->Client_request.arth.x);       
            scanf("%f",&data_comm->Client_request.arth.y);       
            scanf(" %c",&data_comm->Client_request.arth.action);       
        }
        else if(x==2){
            printf("Please enter operands (evenorodd)\n");
            data_comm->Client_request.request_type=2;
            scanf("%d",&data_comm->Client_request.eoo.x);       
        }
        else if(x==3){
            printf("Please enter operands (isPrime)\n");
            data_comm->Client_request.request_type=3;
            scanf("%d",&data_comm->Client_request.ip.x);         
        }
        else{
            
        }
        while(data_comm!=NULL && data_comm->Server_response.status!= SERVER_READY){
            sleep(1);
        }
        if(data_comm==NULL)
        {
            printf("server stopped\n");
            exit(1);
        }
        data_comm->Server_response.ack = NACK;
        data_comm->Client_request.client_status = CLIENT_REQUESTED;
            while(data_comm->Server_response.ack == NACK){
                sleep(1);
            }
            while (data_comm->Server_response.status == PROCESSING)
            {
                sleep(1);
            }
            if(data_comm->Server_response.status == SUCCESSFULL) {
                printf("%s\n",data_comm->Server_response.msg);
                if(data_comm->Client_request.request_type==1){
                    printf("the operation result is:%f\n",data_comm->Server_response.ans);
                }
                data_comm->Client_request.client_status = MSG_REC;
            }
            else {
                printf("%s\n",data_comm->Server_response.msg);
                data_comm->Client_request.client_status = MSG_REC;
            }
        
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
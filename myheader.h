#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define CONNECT_CHANNEL 100
#define MAX_CLIENT_NAME 100
#define MAX_CLIENTS 100
#define SERVER_BUSY 11
#define SERVER_READY 9
#define SUCCESSFULL 7
#define USER_EXIST 5
#define CLIENT_LIMIT_EXCEEDED 2
#define CLIENT_REQUESTED 8
#define FINISHED 6
#define NOT_REPLIED 10
#define ACK 4
#define NACK 2
#define PROCESSING 45
#define STOP 56
#define NOT_CLIENT_REQUESTED 89
#define MSG_REC 69
// typedef struct Response
// {
//     key_t key;
//     int status;
//     int server_reply;
//     int ack;
// }response;
// typedef struct Arithmetic{
//     float x;
//     float y;
//     char action;
// }arithmetic;
// typedef struct EvenOrOdd{
//     int x;
// }evenOrOdd;
// typedef struct IsPrime{
//     int x;
// }isPrime;


// typedef struct com_Response
// {
//     char msg[100];
//     float ans;
//     int status;
//     int ack;
// }com_response;

// typedef struct Request
// {
//     char name[MAX_CLIENT_NAME];
//     int client_status;

// }request;

// typedef struct comRequest
// {
//     int request_type;
//     int client_status;
//     arithmetic arth;
//     evenOrOdd eoo;
//     isPrime ip;

// }com_request;

// typedef struct Communication
// {
//     com_response Server_response;
//     com_request Client_request;
// }communication;

// typedef struct Channel
// {
//     response Server_response;
//     request Client_request;
//     sem_t sem;
    
// }channel;

// typedef struct Com_channel
// {
//     com_response Server_response;
//     com_request Client_request;
    
// }Com_channel;
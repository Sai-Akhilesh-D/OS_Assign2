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
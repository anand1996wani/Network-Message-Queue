#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <netdb.h>
#include <ifaddrs.h>


struct Message {
	char ipAddress[20];
	int sourcePort;
	char message[100];
};

struct NetworkMessage{
	//long long mtype;
	long mtype;	
	struct Message message[20];
};

int msgget_nmb();

int msgsnd_nmb(struct NetworkMessage message);

void get_ip(long type,char *final_ans);

int get_port(long type);

long get_type(char *ip,int port);

long power(int a,int b);

long b2d(int *arr,int size);

void d2b(long n, int *bin_ip,int size); 

struct NetworkMessage msgrcv_nmb(struct NetworkMessage message);

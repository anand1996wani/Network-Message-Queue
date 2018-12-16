
#include "nmb.h"


#define portNo 1111
#define BportNo 1112
#define udpPortNo 1112

key_t key = 2000;

int udpSocket;
struct sockaddr_in udpServerAddress;
struct sockaddr_in serverAddress,clientAddress;
char myIp[20];

void UDPReceiver(){
	
	struct sockaddr_in BserverAddress,BclientAddress;
	BserverAddress.sin_family = AF_INET;
	BserverAddress.sin_port = htons(BportNo);
	//BserverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret4 = inet_pton(AF_INET,"192.168.0.9",&BserverAddress.sin_addr.s_addr);
	if(ret4==-1){
		printf("Ip Address Error\n");
	}
	int BserverSocket;
	BserverSocket = socket(AF_INET,SOCK_DGRAM,0);
		//int broadcast = 1;
		//setsockopt(serverSocket,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast));
	int ret1 = bind(BserverSocket,(struct sockaddr*)&BserverAddress,sizeof(BserverAddress));
	if(ret1==-1){
		printf("Bind Error\n");
	}
	char BclientMessage[256];
	int sLen = sizeof(BclientAddress);
	struct NetworkMessage Bmessage;
		
	int BqueueId;
		
	BqueueId = msgget(key,IPC_CREAT | 0666);
		
	if(BqueueId==-1){
		printf("Message Queue Error\n");
	}
	int ret;
	while(1){
	
		recvfrom(BserverSocket,&Bmessage,sizeof(Bmessage),0,(struct sockaddr *)&BclientAddress,&sLen);
		//printf("Message Received Is : %s\n",Bmessage.message->message);
		
		//Bmessage.mtype = ntohl(Bmessage.mtype);
		
		ret = msgsnd(BqueueId,(const void *)&Bmessage,sizeof(Bmessage.message),0);
		if(ret==-1){
			printf("Message Queue Send Error\n");
		}
		//printf("Message Received Is : %s\n",Bmessage.message->message);

		/*recvfrom(serverSocket,&message,sizeof(message),0,(struct sockaddr *)&clientAddress,&sLen);
		printf("Message Received Is : %s\n",message.message->message);
		if(strcmp(myIp,message.message->ipAddress)==0){
			ret = msgsnd(queueId,(const void *)&message,sizeof(message.message),0);
			if(ret==-1){
				printf("Message Queue Send Error\n");
			}
			printf("Message Received Is : %s\n",message.message->message);
		}*/
	}		
}


void zombieHandler(int signalNo){
	wait(NULL);
}

void handleClient(int clientSocket){

	
	
	struct NetworkMessage message;
	int i = 0;
	int status = recv(clientSocket,&message,sizeof(message),0);
	if(status ==-1){
		printf("Error Receiving\n");
	}else{
		if(message.mtype > 0){
			int clientPort = ntohs(clientAddress.sin_port);
			//printf("Client Port No%d\n",clientPort);	
			//printf("Data Received From Process Id  %d Is %s\n",getpid(),message.message->message);
			udpServerAddress.sin_addr.s_addr = inet_addr(message.message->ipAddress);
			
			//message.mtype = htonl(message.mtype);
			
			sendto(udpSocket,&message,sizeof(message),0,(struct sockaddr *)&udpServerAddress,sizeof(udpServerAddress));
			close(udpSocket);
			close(clientSocket);
		}else if(message.mtype < 0){
			int queueId;
			queueId = msgget(key,IPC_CREAT | 0666);
			if(queueId==-1){
				printf("Message Queue Error\n");
			}
			int ret;
			struct NetworkMessage message1;
			message1.mtype = -1;
			int clientPort = ntohs(clientAddress.sin_port);
			//printf("Client Port No %d\n",clientPort);
			
			long type = get_type(myIp,message.message->sourcePort);
			
			//printf("type is : %ld\n",type);
			sleep(1);
			
			//ret = msgrcv(queueId,(void *)&message1,sizeof(message1.message),message.message->sourcePort,IPC_NOWAIT);
			ret = msgrcv(queueId,(void *)&message1,sizeof(message1.message),type,IPC_NOWAIT);
			
			if(ret==-1){
				strcpy(message1.message->message,"No Message Received");
			}else{
				//printf("Message From Queue is : %s\n",message1.message->message);
				//printf("Message From Queue is : %s\n",message1.message->ipAddress);
				//printf("Message From Queue is : %d\n",message1.message->sourcePort);
				
			}
			send(clientSocket,&message1,sizeof(message1),0);
			//printf("Hello\n");
			close(udpSocket);
			close(clientSocket);
		}
	}	
}


int main(int argc,char *argv[]){
	
		
	
	if(argc < 2){
		printf("./local_server <local IPADDRESS>\n");
		exit(0);
	}
		strcpy(myIp,argv[1]);
		
		pid_t pid = fork();
		if(pid==0){
			//UDP Receiver
			UDPReceiver();
			exit(0);
		}
		
		
		int serverSocket,clientSocket;
		
		signal(SIGCHLD,zombieHandler);
		
		serverSocket = socket(AF_INET,SOCK_STREAM,0);
		
		socklen_t clientLength;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(portNo);
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		bind(serverSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
		listen(serverSocket,10);
		int childId = 0;
		
		
		
		udpSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		
		
		udpServerAddress.sin_family = AF_INET;
		udpServerAddress.sin_port = htons(udpPortNo);
		//udpServerAddress.sin_addr.s_addr = INADDR_BROADCAST;
		//udpServerAddress.sin_addr.s_addr = inet_addr("192.168.43.255");
		//udpServerAddress.sin_addr.s_addr = inet_addr("127.255.255.1");
		
	    	//int broadCastPerm = 1;
		//setsockopt(udpSocket,SOL_SOCKET,SO_BROADCAST,&broadCastPerm,sizeof(broadCastPerm));
		
		
		for( ; ; ){
			clientLength = sizeof(clientAddress);
			clientSocket = accept(serverSocket,(struct sockaddr *)&clientAddress,&clientLength);
			childId = fork();
			if(childId==0){
				close(serverSocket);
				handleClient(clientSocket);
				//printf("Child Process Terminated\n");
				exit(0);
			}
			close(clientSocket);
			//exit(0);
		}
	return 0;
}

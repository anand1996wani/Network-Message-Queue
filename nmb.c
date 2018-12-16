

#include "nmb.h"


#define portNo 1111
key_t key1 = 2000;

int clientSocket = -1;
struct sockaddr_in serverAddress,clientAddress;


int msgget_nmb(){
	int queueId;
	queueId = msgget(key1,IPC_CREAT | 0666);
	if(queueId==-1){
		printf("Message Queue Error\n");
		exit(0);
	}
	return queueId;
}


int msgsnd_nmb(struct NetworkMessage message){	
	//printf("Data is %s\n",message.message);
	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_addr.s_addr = INADDR_ANY;
	clientAddress.sin_port = htons(message.message->sourcePort);
	
	if(bind(clientSocket,(struct sockaddr*)&clientAddress,sizeof(struct sockaddr_in))==0){
		//printf("Bind Successful On Port Number %d\n",message.message->sourcePort);
	}else{
		printf("Bind UnSuccessful Please Select Another PortNo\n");
		return -1;
	}
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNo);
	//serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	if(inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0){
	 	printf("\nInvalid address/ Address not supported \n"); 
    	}
	
	int connectionStatus = connect(clientSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
	if(connectionStatus == -1)
		printf("Connection Error\n");
	
	
	int ret = send(clientSocket,&message,sizeof(message),0);
	if(ret==-1){
		printf("Message Sending Error\n");
	}
	return ret;
}

struct NetworkMessage msgrcv_nmb(struct NetworkMessage message){
	
	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_addr.s_addr = INADDR_ANY;
	clientAddress.sin_port = htons(message.message->sourcePort);
	
	struct NetworkMessage message1;
	message1.mtype = -1;
	if(bind(clientSocket,(struct sockaddr*)&clientAddress,sizeof(struct sockaddr_in))==0){
		//printf("Bind Successful On Port No %d\n",message.message->sourcePort);
	}else{
		printf("Bind UnSuccessful Please Select Another PortNo\n");
		return message1;
	}
	
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNo);
	//serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	if(inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0){
	 	printf("\nInvalid address/ Address not supported \n"); 
    	} 
	
	int connectionStatus = connect(clientSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
	if(connectionStatus == -1)
		printf("Connection Error\n");
		
	
	send(clientSocket,&message,sizeof(message),0);
	
	//int ret = recv(clientSocket,&message1,sizeof(message1),MSG_DONTWAIT);
	int ret = recv(clientSocket,&message1,sizeof(message1),0);
	
	return message1;
}

void d2b(long n, int *bin_ip,int size) 
{ 
   
    int ans[48]={0};
    int i = 0; 
    while (n > 0) { 
  
        ans[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 
  
    int k=0;
    for (int j = size-1; j >=0 ; j--) 
     {   bin_ip[k] = ans[j];
   	k++;
     }	
} 

long b2d(int *arr,int size) 
{ 
	long ans=0,base=1;

	for(int i=size-1;i>=0;i--)
	{
	  	ans+=arr[i]*base;
		base*=2;
	}

	return ans;
} 

long power(int a,int b)
{
	long result=a;
	
	if(b==0)
	return 1;

	for(int i=0;i<b-1;i++)
	{

		result*=a;
	}

	return result;
}

long get_type(char *ip,int port)
{
	int n1, n2, n3, n4;
	sscanf(ip, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);

	long converted_ip = n1*power(256,3) + n2*power(256,2) + n3*power(256,1) + n4*power(256,0);	
	int bin_ip[32]={0};
	int bin_sock[16]={0};

	int final_val[48]={0};

	d2b(converted_ip,bin_ip,32);
	d2b(port,bin_sock,16);

    for (int j = 0; j <48 ; j++)
     {       if(j<=31)	
	     {
		final_val[j] = bin_ip[j];
	     }              
	     else
	     final_val[j] = bin_sock[j-32];	     
     }
	
	return b2d(final_val,48);

}

int get_port(long type)
{
	int final_val[48]={0};
	d2b(type,final_val,48);



	int port[16]={0};
	int k=0;
	for (int j = 32; j <=47 ; j++)
	{
	   port[k++] = final_val[j];
	}

	return b2d(port,16);

}

void get_ip(long type,char *final_ans)
{
	int final_val[48]={0};
	d2b(type,final_val,48);

	int ip[32]={'\0'};
	int k=0;
	for (int j = 0; j <=31 ; j++)
	{
	   ip[k++] = final_val[j];
	}

	long converted_ip = b2d(ip,32);

	int n1,n2,n3,n4;

	n4=converted_ip%256;
	converted_ip/=256;
	n3=converted_ip%256;
	converted_ip/=256;
	n2=converted_ip%256;
	converted_ip/=256;
	n1=converted_ip%256;

	sprintf(final_ans,"%d.%d.%d.%d",n1,n2,n3,n4);
}

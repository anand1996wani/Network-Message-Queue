

#include "nmb.h"


int main(int argc,char *argv[]){
	
	int option = 0;
	
	int queueId = msgget_nmb();
	
	while(1){
		printf("Enter Your Choice   \n");
		printf("1 : Send Message    \n");
		printf("2 : Receive Message \n");
		printf("3 : EXIT            \n");
		scanf("%d",&option);
		if(option==1){
			
			struct NetworkMessage message;
			printf("Enter Source Port\n");
			scanf("%d",&message.message->sourcePort);
			printf("Enter Remote IP\n");
			scanf("%s",message.message->ipAddress);
			//printf("Enter Remote Port\n");
			//scanf("%d",&message.message->desPort);
			
			
			
			message.mtype = get_type(message.message->ipAddress,message.message->sourcePort);
			//printf("type is : %ld\n",message.mtype);
			
			//message.mtype = 12345;
			printf("Enter Your Message\n");
			while(getchar()!='\n');
			fgets(message.message->message, sizeof(message.message->message), stdin);
			
			//scanf("%s",message.message->message);
			msgsnd_nmb(message);
			
		}else if(option==2){
			struct NetworkMessage message;
			printf("Enter Source Port\n");
			scanf("%d",&message.message->sourcePort);
			message.mtype = -1;
			strcpy(message.message->message,"No Message Received");
			message = msgrcv_nmb(message);
			printf("Message : %s\n",message.message->message);
		}else{
			break;
		}
	}
	
	return 0;
}

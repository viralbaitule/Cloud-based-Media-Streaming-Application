#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <mysql/mysql.h>

static char *host="localhost";
static char *user="root";
static char * pass="htc";
static char * dbname="warehouse";
static unsigned int port=NULL;
static char * unix_socket=NULL;
static unsigned int flag=0;
static int initial_input[5];
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_ROW row1;
MYSQL * conn;

void sendfile(char resultstr[], int sockdes);
void* video(void* sock);
void* audio(void* sock);
void* text(void* sock);

int main(void)
{
	int listenfd = 0;
	int connfd = 0;
	int optval=1;
	struct sockaddr_in serv_addr;
	char sendBuff[1025];
	int numrv,checkth;
	pthread_t th;
	
	// database creation and connection 

	conn=mysql_init(NULL);
	if(!(mysql_real_connect(conn,host,user,pass,dbname,port,unix_socket,flag)))
	{
	printf("Error in connecting to database\n");
	}
	
	// Socket crreation
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	printf("Socket retrieve success\n");

	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

	if(listen(listenfd, 10) == -1)
	{
		printf("Failed to listen\n");
		return -1;
	}
	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
		recv(connfd,initial_input,sizeof(initial_input),0);       
		if(initial_input[0]==1)
		{
			checkth=pthread_create(&th,NULL,video,(void*)&connfd);
			if(checkth<0)
			printf("Error in thread creation");
		}
		if(initial_input[0]==2)
		{
			checkth=pthread_create(&th,NULL,audio,(void*)&connfd);
			if(checkth<0)
			printf("Error in thread creation");
		}
		if(initial_input[0]==3)
		{
			checkth=pthread_create(&th,NULL,text,(void*)&connfd);
			if(checkth<0)
			printf("Error in thread creation");
		}
		 
	}


	return 0;
}


// File transfer code for video audio and text

void* video(void* sock) 
{
	/* Open the file that we wish to transfer */
	int sockdes = *(int*)sock;
	int num_fields,num_rows,j;
	int decision[2]={0};
	char resultStr[250];
	mysql_query(conn, "SELECT * from video");
	res=mysql_store_result(conn);
	int i=0;
	num_fields = mysql_num_fields(res);
		
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		for(j = 0; j < num_fields; j++) 
	      	{					  
			if(j==1)
			{	
				char temp[50];
				char temp2[50];
				memset(temp, 0, sizeof(temp));
				memset(temp2, 0, sizeof(temp2));
				strcpy(temp2,row[1]);
				strncpy(temp,temp2+13,(strlen(temp2)-13));//calculation according to file path size
				strcat(resultStr , temp);
			}
			else{	  
	      		strcat(resultStr , row[j]);
			strcat(resultStr , " ");
			}
		} 
					  	
		strcat(resultStr , "\n"); 
	}
	send(sockdes,&resultStr,sizeof(resultStr),0);  // client should receive list of options
        
	recv(sockdes,decision,sizeof(decision),0);
	
	if(decision[0]>0)
	{	
		//row1 = mysql_fetch_row(res);
		mysql_query(conn, "SELECT * from video");		
		res=mysql_store_result(conn);
		while ((row = mysql_fetch_row(res))!=NULL)
		{
	
			if (decision[0]==atoi(row[0]))
			{
				strcpy(resultStr , row[1]);
				sendfile(resultStr,sockdes);
			} 
		}
	}
	return NULL;
}

void* audio(void* sock)
{
	/* Open the file that we wish to transfer */
	int sockdes = *(int*)sock;
	int num_fields,num_rows,j;
	int decision[2]={0};
	char resultStr[250];
	mysql_query(conn, "SELECT * from audio");
	res=mysql_store_result(conn);
	int i=0;
	num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		for(j = 0; j < num_fields; j++) 
	      	{					  
			if(j==1)
			{
				char temp[50];
				char temp2[50];
				memset(temp, 0, sizeof(temp));
				memset(temp2, 0, sizeof(temp2));
				strcpy(temp2,row[1]);
				strncpy(temp,temp2+13,(strlen(temp2)-13));//calculation according to file path size
				strcat(resultStr , temp);
			}
			else{	  
	      		strcat(resultStr , row[j]);
			strcat(resultStr , " ");
			}
		} 
					  	
		strcat(resultStr , "\n"); 
	}
	send(sockdes,&resultStr,sizeof(resultStr),0);  // client should receive list of options
	recv(sockdes,decision,sizeof(decision),0);
	if(decision[0]>0)
	{
		mysql_query(conn, "SELECT * from audio");		
		res=mysql_store_result(conn);
		while ((row = mysql_fetch_row(res)) != NULL)
		{

			if (decision[0]==atoi(row[0]))
			{
				strcpy(resultStr , row[1]);
				sendfile(resultStr,sockdes);
			} 
		}
	}
	return NULL;
}

void* text(void* sock)
{
	/* Open the file that we wish to transfer */
	int sockdes = *(int*)sock;
	int num_fields,num_rows,j;
	int decision[2]={0};
	char resultStr[250];
	mysql_query(conn, "SELECT * from text");
	res=mysql_store_result(conn);
	int i=0;
	num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		for(j = 0; j < num_fields; j++) 
	      	{
			if(j==1)
			{
				char temp[50];
				char temp2[50];
				memset(temp, 0, sizeof(temp));
				memset(temp2, 0, sizeof(temp2));
				strcpy(temp2,row[1]);
				strncpy(temp,temp2+13,(strlen(temp2)-13));
				strcat(resultStr , temp);
			}
			else{	  
	      		strcat(resultStr , row[j]);
			strcat(resultStr , " ");
			}
		} 
					  	
		strcat(resultStr , "\n"); 
	}
	send(sockdes,&resultStr,sizeof(resultStr),0);  // client should receive list of options
	recv(sockdes,decision,sizeof(decision),0);
	if(decision[0]>0)
	{
		mysql_query(conn, "SELECT * from text");		
		res=mysql_store_result(conn);
		while ((row = mysql_fetch_row(res)) != NULL)
		{

			if (decision[0]==atoi(row[0]))
			{
				strcpy(resultStr , row[1]);
				sendfile(resultStr,sockdes);
			} 
		}
	}
	return NULL;
}
    	
void sendfile(char resultstr[], int sockdes)
{
  	FILE *fp = fopen(resultstr,"rb");
  	if(fp==NULL)
	{
    		printf("File open error");
    		  
	}   

	/* Read data from file and send it */
	while(1)
	{
    		/* First read file in chunks of 256 bytes */
		 unsigned char buff[256]={0};
		 int nread = fread(buff,1,256,fp);
		 printf("Bytes read %d \n", nread);        

    		/* If read was success, send data. */
	    	if(nread > 0)
	    	{
	      		printf("Sending \n");
			write(sockdes, buff, nread);
	    	}

	    	if (nread < 256)
	    	{
			if (feof(fp))
		    	printf("End of file\n");
			if (ferror(fp))
		    	printf("Error reading\n");
			break;
	    	}
	}

	close(sockdes);
        sleep(1);
        pthread_exit(0);

}



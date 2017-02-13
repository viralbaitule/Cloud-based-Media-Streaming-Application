#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <glib.h>

void *Thread_connection_handler(void * filename);

// Sending and retrieving info
static void video(GtkWidget *widget, gpointer data)
{ 
    int sockfd = 0;
    int optval=1;
    int rcount=1;
    int input;
    int bytesReceived = 0;
    char recvBuff[256];
    struct sockaddr_in serv_addr;
    int user_input[2]={0};
    user_input[0]=1;

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
    }

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    serv_addr.sin_addr.s_addr = inet_addr("52.11.48.192");
   
    /*  connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
    }

    /* sending Initial choice for video selection*/
    if (send(sockfd,user_input,sizeof(user_input),0)<0)
    {
	printf("send error\n");
    }
    /* receving List from server */
    if((bytesReceived = read(sockfd, recvBuff, 256))<0)
	{	
		printf("error in receiving");
	}	
	puts(recvBuff);
	if (user_input[0]==1)
	{
		printf("Select no. from the list \n");
		scanf("%d",&input);
		user_input[0]=input;
		if (send(sockfd,user_input,sizeof(user_input),0)<0)
		{
			printf("send error");
		}
		char filename[32]; 
		snprintf(filename, sizeof(char) * 32, "video%i.avi", input);
		FILE *fp;
		fp = fopen(filename, "ab"); 

		if(NULL == fp)
		{
			printf("Error opening file");
		}

		/* Receive data in chunks of 256 bytes */
		while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
		{
			printf("Bytes received %d\n",bytesReceived);    
			fwrite(recvBuff, 1,bytesReceived,fp);
			pthread_t thread_id;
			if (rcount==10)
			{
				if(pthread_create(&thread_id,NULL,Thread_connection_handler,(void*)filename)<0)
				{
					printf("error in creating thread \n");
				}
				rcount++;
			}
			rcount++;	
		}

		if(bytesReceived < 0)
		{
			printf("\n Read Error \n");
		}
	}
}

/* code for Audio streaming */
static void audio(GtkWidget *widget, gpointer data)
{ 
	int sockfd = 0;
	int optval=1;
	int rcount=1;
	int input;
	int bytesReceived = 0;
	char recvBuff[256];
	struct sockaddr_in serv_addr;
	int user_input[2]={0};
	user_input[0]=2;
	/* Create a socket first */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		printf("\n Error : Could not create socket \n");

	}
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	/* Initialize sockaddr_in data structure */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000); // port
	serv_addr.sin_addr.s_addr = inet_addr("52.11.48.192");

	/* Attempt a connection */
	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		printf("\n Error : Connect Failed \n");

	}


	if (send(sockfd,user_input,sizeof(user_input),0)<0)
	{
		printf("send error\n");
	}


	if((bytesReceived = read(sockfd, recvBuff, 256))<0)
	{	
		printf("error in receiving");
	}	
	puts(recvBuff);
	if (user_input[0]==2)
	{
		printf("Select no. from the list \n");
		scanf("%d",&input);
		user_input[0]=input;
		if (send(sockfd,user_input,sizeof(user_input),0)<0)
		{
			printf("send error");
		}
		char filename[32]; 
		snprintf(filename, sizeof(char) * 32, "audio%i.mp3", input);
		FILE *fp;
		fp = fopen(filename, "ab"); 
		 
		if(NULL == fp)
		{
			printf("Error opening file");
		
		}

		/* Receive data in chunks of 256 bytes */
		while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
		{
			printf("Bytes received %d\n",bytesReceived);    
		
			fwrite(recvBuff, 1,bytesReceived,fp);
		
			pthread_t thread_id;
			if (rcount==10)
			{
				if(pthread_create(&thread_id,NULL,Thread_connection_handler,(void*)filename)<0)
				{
					printf("error in creating thread \n");
				}
				rcount++;
			}
			rcount++;	
		}

		if(bytesReceived < 0)
		{
			printf("\n Read Error \n");
		}
	}

}

static void text(GtkWidget *widget, gpointer data)
{
  
	int sockfd = 0;
	int optval=1;
	int rcount=1;
	int input;
	int bytesReceived = 0;
	char recvBuff[256];
	struct sockaddr_in serv_addr;
	int user_input[2]={0};
	user_input[0]=3;
	/* Create a socket first */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		printf("\n Error : Could not create socket \n");

	}
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
	/* Initialize sockaddr_in data structure */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000); // port
	serv_addr.sin_addr.s_addr = inet_addr("52.11.48.192");

	/* Attempt a connection */
	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		printf("\n Error : Connect Failed \n");

	}


	if (send(sockfd,user_input,sizeof(user_input),0)<0)
	{
		printf("send error\n");
	}


	if((bytesReceived = read(sockfd, recvBuff, 256))<0)
		{	
			printf("error in receiving");
		}	
		puts(recvBuff);
		if (user_input[0]==3)
		{
			printf("Select no. from the list \n");
			scanf("%d",&input);
			user_input[0]=input;
			if (send(sockfd,user_input,sizeof(user_input),0)<0)
			{
				printf("send error");
			}
			char filename[32]; 
			snprintf(filename, sizeof(char) * 32, "text%i.txt", input);
			FILE *fp;
			fp = fopen(filename, "ab");
			 
			if(NULL == fp)
			{
				printf("Error opening file");
			
			}

			/* Receive data in chunks of 256 bytes */
			while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
			{
				printf("Bytes received %d\n",bytesReceived);    
				fwrite(recvBuff, 1,bytesReceived,fp);

			}
			if(bytesReceived < 0)
			{
				printf("\n Read Error \n");
			}
			fclose(fp);
			/* To open text file in GUI */
			char cmdbuf[256];
			snprintf(cmdbuf, sizeof(cmdbuf), "xdg-open %s ", (char*)filename);
			system(cmdbuf);
		}

}

int main(int argc, char *argv[])
{
	int sockfd = 0;
	int optval=1;
	int rcount=1;
	int input;
	int bytesReceived = 0;
	char recvBuff[256];
	int user_input[2]={0};

	memset(recvBuff, '0', sizeof(recvBuff));

// GTK GUI implementation and code integration 
	GtkWidget *window;
	GtkWidget *fixed;

	GtkWidget *btn1;
	GtkWidget *btn2;
	GtkWidget *btn3;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window), "GRAPHICAL USER INTERFACE");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 30);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);

	// Video Button
	btn1 = gtk_button_new_with_label("VIDEO FILE");
	gtk_fixed_put(GTK_FIXED(fixed), btn1, 0, 0);
	gtk_widget_set_size_request(btn1, 300, 100); 
 
	g_signal_connect(G_OBJECT(btn1), "clicked", 
	G_CALLBACK(video), NULL);

	//Audio Button
	btn2 = gtk_button_new_with_label("AUDIO FILE");
	gtk_fixed_put(GTK_FIXED(fixed), btn2, 0, 100);
	gtk_widget_set_size_request(btn2, 300, 100);
	g_signal_connect(G_OBJECT(btn2), "clicked", 
	G_CALLBACK(audio), NULL);

	//Text Button
	btn3 = gtk_button_new_with_label("TEXT FILE");
	gtk_fixed_put(GTK_FIXED(fixed), btn3, 0, 200);
	gtk_widget_set_size_request(btn3, 300, 100);
	g_signal_connect(G_OBJECT(btn3), "clicked", 
	G_CALLBACK(text), NULL);



	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy",G_CALLBACK(gtk_main_quit), NULL);  

	gtk_main();

	return 0;
}

void *Thread_connection_handler(void * filename)
{
	char cmdbuf[256];
 	snprintf(cmdbuf, sizeof(cmdbuf), "vlc --rate=1.00 --playlist-autostart -vvv %s --tcp-caching=10", (char*)filename);
	system(cmdbuf);
	pthread_exit(NULL);
}


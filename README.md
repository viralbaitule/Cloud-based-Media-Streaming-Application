# Cloud-based-Media-Streaming-Application

The “Multimedia streaming” project offers the clients an independence of accessing files from any where over the internet. The server responds to each client concurrently and fetch each with their choice of file. This project is implemented with the concepts gained during the course on Network programing and applications. The Client Server communication is implemented using TCP. The server is featured with concurrency using multithreading. The server listens to each of the clients on the master thread and creates new slave threads to serve new requests from each client. The project is implemented using a fat server and a thin client architecture where majority of processing is handled by the server. The system architecture consists of one server and multiple clients. The server provides redundancy, reliability and backup services. Multiple clients can access server and make their desired request at the same time. Multiple clients can even access a single file at the same time. The clients of this server can be located anywhere and access the server irrespective of their geographical location. This service is provided by deploying the server on the Amazon web services.


Server Dependencies
My SQL
Install MYSQL DataBase-- Command -- sudo apt-get install mysql-server


Client Dependencies
GTK 
Install gtk library -- command -- sudo apt-get install libgtk2.0-dev
build gtk -- command -- sudo apt-get build-dep gedit
VLC Player


Server Compile and run
gcc -o 13_server 13_server.c $(mysql -config --cflags) $(mysql -config --libs) -lpthread

Client Compile and run
gcc -o 13_client 13_client.c `pkg-config --libs --cflags gtk+-2.0` -lpthread

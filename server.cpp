#include "server.h"
#define ALL_BUT(i) i
#define ALL -1
#define USERS_ONLINE ("Users online: " + std::to_string(counter_user_online))

server::server(int Port){
	//reset user counter
	counter_user_online = 0;
	//give a name to the server
	name_serv = "server";

	//configure the socket for the server
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(Port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int enable = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

    //bind the socket to the server
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
}

server::~server(){
	//close all client sockets
	for(int i=0; i<client_sockets.size(); ++i)
		close(client_sockets[i]);
	//close server socket
	close(listener);
	client_sockets.clear();
}

void server::sender(const int &sock,const string &buf, const string &name_client)
{   
	//concatenate name and buffer
    string buf_send = name_client + ": " + buf + '\0';
    //display a message on the server
    std::cout << buf_send << std::endl;            
	//send a message to everyone except the sender	    
    for(int curr_sock=0; curr_sock < client_sockets.size(); ++curr_sock)            	            	
		if(sock != client_sockets[curr_sock])
	    	send(client_sockets[curr_sock], buf_send.c_str(), buf_send.length(), 0);   
       

}

void server::disconnect_client(int index)
{
	//close the socket at the specified index
	close(client_sockets[index]);
	//remove the socket from the socket vector on the server
	cout << "sock size: " << client_sockets.size() << endl;
	cout << "index: " <<  index << endl;
	if(client_sockets.size() > 0)client_sockets.erase(client_sockets.begin()+index);
	//decrease the user counter by 1
	if(counter_user_online > 0) counter_user_online--;	

}

void server::client_handler(int sock)
{	
    int bytes_read; //number of bytes received
	char buf_recv[MESSAGE_SIZE]; // buffer for received messages
    string buf_send; // message buffer
    string name_client; // client name

	bytes_read = recv(sock, buf_recv, MESSAGE_SIZE, 0); // expect a message with a name from the client
    if(bytes_read > 0) // if the message arrived
    	name_client = buf_recv; // save name client
    else {
    //else break the connection
    for(int i=0; i<client_sockets.size(); ++i)
    				//find socket index in vector
           			if(sock == client_sockets[i])
    					disconnect_client(i);
    return; 
	}
    cout << "flag 2\n";

	//send to all customers, the number of online users
    buf_send.clear();
    buf_send = USERS_ONLINE;
    sender(ALL,buf_send,name_serv);

    //send a message to all clients about connecting a new user
    buf_send.clear();
    buf_send = name_client +": is connected.";
    sender(sock,buf_send,name_serv);
    //and print it on the server
    cout << buf_send << endl;
	while(1)  //customer communication cycle
       {        	
           bytes_read = recv(sock, buf_recv, MESSAGE_SIZE, 0);//waiting for a message
           /*if the connection is lost, interrupt the message; 
           decrease the counter of current users; 
           We inform other clients that we are leaving the chat;*/
           if(bytes_read <= 0){           		
           		for(int i=0; i<client_sockets.size(); ++i)
           			if(sock == client_sockets[i]){

           				buf_send.clear();
           				buf_send = " connection lost";
           				cout << name_client << buf_send << endl;
           				sender(ALL_BUT(sock), buf_send, name_client);
           				disconnect_client(i);
           				
           				buf_send.clear();
    					buf_send = USERS_ONLINE;
    					sender(ALL,buf_send,name_serv);

           				return;
           			}
           		
           }
           else { // otherwise we send a message to everyone except the sender
           	buf_send.clear();
           	buf_send = buf_recv;
           	sender(ALL_BUT(sock),buf_send,name_client);			   
	        }	
           
        } 

}


void server::stop()
{
	//if the user entered "stop", close all sockets and exit the program
	cout << "Enter stop to stop the server\n";
	string buf;
	while(buf != "stop")
	{
		cin >> buf;
	} 
	this->~server();	
	exit(4);
}

void server::start(bool daemon)
{

	std::cout << "server started\n";
	listen(listener, 1); // put the socket in listening mode

	//if the server is started in interactive mode, 
	//we start the thread waiting for the "stop" command to complete the work server
	if(daemon == false){ 
	thread threat_stop_serv(&server::stop,this);
	threat_stop_serv.detach();
	}

	int sock; // socket for client
	while(sock = accept(listener, NULL, NULL)) // accept the next connection
    {
        
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        //push the socket into a vector so as not to lose
        client_sockets.push_back(sock);
        //increment online user counter
        counter_user_online++;
        //allocate a separate thread for the client handler
        thread thread_for_client(&server::client_handler, this, sock);
        thread_for_client.detach();    
        cout << "sock size: " << client_sockets.size() << endl;    
    }
}



#pragma once
#include "Includes.h"

class server
{
private:		
	int listener; // server socket to listen clients
	int counter_user_online; // counter users
    struct sockaddr_in addr; // server address   
	vector<int> client_sockets;	// client sockets
	string name_serv; // server name 

	// method for sending messages, all but specified
    void sender(const int &, const string &, const string &); 
    // disconnect specified client
    void disconnect_client(int);
    // client handler, each client is given a stream
    void client_handler(int sock);
public:
	void start(bool); // server start: option (true: with daemon option; false: interactive option)
	void start() { start(false); }; // default: server launch with interactive option

	//method for stopping the server interactively
	void stop();

	//server initialization on the specified port
	server(int Port); // 
	//server initialization on a standard port
	server(): server(PORT) {}; // 


	~server();
	
};
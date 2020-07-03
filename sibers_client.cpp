#include "Includes.h"
int sock; // client socket
struct sockaddr_in addr; //client address

void sender()
{   
    string buf; //buffer for sending messages
    string name;//name buffer

    //we ask the user to enter a name
    cout << "Enter your name: ";
    cin >> name;      
    name += '\0';
    //send our name to the server
    send(sock, name.c_str(), name.length(), 0);
    
    while(1){
        buf.clear();
        cout << name << ": ";
        getline(cin,buf); //enter message
 
        buf += '\0';    
        send(sock, buf.c_str(), buf.length(), 0);  //  send message    
    }

}

void recipient()
{
    int bytes_read; ////number of bytes received
    char buf[MESSAGE_SIZE];// buffer for received messages
    while(1){    //endless message loop    
        bytes_read = recv(sock, buf, sizeof(buf), 0);//waiting for a message
        if(bytes_read <= 0){  
            //if the connection is broken, close the socket, exit the program              
            cout << "Connection to the server is lostn\n";
            close(sock);            
            exit(3);
        }
        //if you got a message, print it
        cout << buf << endl;
    }
}

int main()
{
    //create a client socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    //configure the address, port (standard 3485), connection type
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }    
    //create separate threads for receiving and sending messages
    thread T1(sender);
    thread T2(recipient);
    T1.join();
    T2.join();
    
    // printf(buf);
    close(sock);

    return 0;
}
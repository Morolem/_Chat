#include "server.h"


#define DAEMON true
#define INTERACTIVE false
#define PID_FILE ".pid_daemon.txt"

//function to write the current pid to a file
void save_pid()
{

    std::ofstream file(PID_FILE);
    if(file.is_open())
    {
        file << getpid();
    }
}

//function to read pid from file
int load_pid()
{
    pid_t pid;
    std::ifstream file(PID_FILE);
    if(file.is_open())
    {
        file >> pid;
        return pid;
    }
    else cout << "Server in daemon mode is not running\n";
    return -1;
}

int main(int argc, char *argv[])
{
    
    
    pid_t parpid; // PID process

    if (argc < 2) //if there are less than 2 arguments, then we get an error message
    {
        cout << "Usage ./daemon -d for daemon or ./daemon -i for interactive\n";
        exit(1);
    }
    //if the interactive launch flag
    if (strcmp(argv[1],"-i")==0) 
    {
        //run interactively
        server s; // create server
        s.start(INTERACTIVE);
        cout << "PID interactive: " << getpid() << endl;
        
    }
    else if(strcmp(argv[1],"-k")==0) // daemon process completion key
    {            
        parpid = load_pid(); //load process id
        if(parpid > 0){ //if successful

        auto err = kill(parpid,SIGINT); //destroy
        perror("kill ");        
        cout << "PID: " << parpid << endl; 
        cout << "Daemon destroyed\n";
        std::remove(PID_FILE); // delete the file from which you downloaded, it is no longer needed
        }
    }    
    else if (strcmp(argv[1],"-d")==0)
    {
        //create a child process
        if((parpid=fork())<0) //
        {                  
            cout << "\ncan't fork";
            exit(1);               
        }
        else if (parpid!=0) //we destroy the main process
        exit(0);           

        setsid(); //detach the child process
        if(load_pid() < 0){ //if you didn’t download the id of the process from the file, then the daemon was not running

        save_pid();//save current id
        server s; // create server
        cout << "PID daemon: " << getpid() << endl;
        s.start(DAEMON); //start the server as a child process
        }
        else cout << "Server is already running\n";

        

    }
    else
    {
    cout << "Usage ./daemon -d for daemon or ./daemon -i for interactive\n";
    exit(1);
    }              
    return 0;
}

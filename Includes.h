#include <vector>
#include <thread>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <cstdio>
#include <cstring> 
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/wait.h>
#include <resolv.h>
#include <time.h>
#include <errno.h>

using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::thread;

#define PORT 3485
#define MESSAGE_SIZE 4096 

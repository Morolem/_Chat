all:
	rm -rf chat
	mkdir chat
	g++ sibers_serv.cpp server.cpp -std=c++11 -pthread -o chat/chat_serv
	g++ sibers_client.cpp -pthread -std=c++11 -o chat/chat_client

/*UDP Echo Client
UDPEchoClient.cpp
*/
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "cipher.h"
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

string crypt(string msg){
	string eMsg = "";
	
	int i = 0;
	while (i < msg.length()){
		eMsg += encr[msg[i]];
		i++;
	}
	return eMsg;
}

string deCrypt(char* msg){
	if (msg[0] == '\0'){
		return "empty reply";
	}
	//Get Rid of first 2 semi colons
	int i = 0;
	int numSemi = 0;
	while (msg[i] != '\0' && numSemi < 2){
		if (msg[i] == ';')
			numSemi++;

		i++;
	}

	string rMsg;
	while (msg[i] != '\0'){
		
		rMsg += decr[msg[i]];
		i++;
	}
	return rMsg;
}


int main(int argc, char *argv[]){
	srand(time(NULL));
	int min = 10000;
	int randMsgNum = (rand() % (RAND_MAX - min) + min);



	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message, server_reply[500];
	int recv_size;
	cout << "Initialising Winsock...." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	{
		printf("Failed. Error Code : %d", WSAGetLastError());		return 1;
	}
	cout << "Initialised" << endl;
	//Create a socket	
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	cout << "socket created" << endl;
	server.sin_addr.s_addr = inet_addr("204.76.188.23");
	server.sin_family = AF_INET;
	server.sin_port = htons(23456);

	//set SO_RESESADDR on a socket s to true	
	int optval = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof optval);
	//Send some data	
	string sMsg = to_string(randMsgNum) + ";1;" + crypt("chris");
	message = const_cast<char*>( sMsg.c_str() );
			
	if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&server, sizeof(server)) < 0)	{
		cout << "Send Failed" << endl;
		return 1;
	}
	cout << "Data Sent" << endl;
	//Receive a reply from the server	
	if ((recv_size = recvfrom(s, server_reply, 500, 0, NULL, NULL)) == SOCKET_ERROR)	{
		cout << "recv failed" << endl;
		return 1;
	}
	cout << "reply Received" << endl;

	//Add \0 at the end of received string string before printing	
	server_reply[recv_size] = '\0';	//puts(server_reply);	
	string res = deCrypt(server_reply);
	cout << res << endl;
	cout << server_reply << endl;
	closesocket(s);
	WSACleanup();
	system("pause"); 
	return 0;
}






/*

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include <ws2tcpip.h>
#include <iostream>


using namespace std;


int main(int argc, char *argv[]){
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message, server_reply[500];
	int recv_size;
	cout << "Initialising Winsock...." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	{
		printf("Failed. Error Code : %d", WSAGetLastError());		return 1;
	}
	cout << "Initialised" << endl;

	//Create a socket	
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)	{
		printf("Could not create socket : %d", WSAGetLastError());
		return 1;
	}
	cout << "socket created" << endl;

	server.sin_addr.s_addr = inet_addr("204.76.188.23");
	server.sin_family = AF_INET;
	server.sin_port = htons(23456);

		int optval = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof optval);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)	{
		cout << "connection error" << endl;
		return 1;
	}
	cout << "connected" << endl;
	//Send some data	
	message = "22222;1;Chris";
	if (send(s, message, strlen(message), 0) < 0)	{
		cout << "Send Failed" << endl;
		return 1;
	}
	cout << "Data Sent" << endl;
	//Receive a reply from the server	
	if ((recv_size = recv(s, server_reply, 500, 0)) == SOCKET_ERROR)	{
		cout << "recv failed" << endl;
		return 1;
	}
	cout << "reply Received" << endl;
	//Add \0 at the end of received string 
	server_reply[recv_size] = '\0';
	cout << server_reply << endl;
	closesocket(s);
	WSACleanup();
	return 0;
}

*/
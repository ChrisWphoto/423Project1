
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
#include <process.h>
#include <vector>
using namespace std;

//global which will be updated by secondary thread
vector<string> recMsgs;

/*
	encryption function
*/

string crypt(string msg){
	string eMsg = "";
	
	int i = 0;
	while (i < msg.length()){
		eMsg += encr[msg[i]];
		i++;
	}
	return eMsg;
}

/*
	Assumption: There will always be 2 semicolons in msg param.
*/
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

//Global Server Vars
WSADATA wsa;
SOCKET s;
struct sockaddr_in server;
char *message, server_reply[500];
int recv_size;

int main(int argc, char *argv[]){
	srand(time(NULL));
	int min = 10000;
	int randMsgNum = (rand() % (RAND_MAX - min) + min);


	
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
	cout << "IP: 204.76.188.23 Port: 23456" << endl;
	cout << "Starting MSG number: " << randMsgNum << endl;

	//set SO_RESESADDR on a socket s to true	
	int optval = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof optval);

	/*
		Start secondary thread for listening
	*/
	double temp = 10.0;
	_beginthread(listen, 0, (void *)&temp);


	
	//Main Program Loop
	string username;
	int msgPlus = 0;
	while (true) {
		
		//set username if not set
		if (username == ""){
			cout << "Welcome to Chat: Enter your username:";
			cin >> username;


			//Send user info to server
			string sMsg = to_string(randMsgNum) + ";1;" + crypt(username);
			message = const_cast<char*>(sMsg.c_str());

			if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&server, sizeof(server)) < 0)	{
				cout << "Send Failed" << endl;
				return 1;
			}

			//Receive a reply from the server	
			if ((recv_size = recvfrom(s, server_reply, 500, 0, NULL, NULL)) == SOCKET_ERROR)	{
				cout << "recv failed" << endl;
				return 1;
			}
			server_reply[recv_size] = '\0';	//puts(server_reply);	
			string res = deCrypt(server_reply);
			cout << res << endl;
		}
		
		//Display user menu
		cout << "Enter q (for quit), s (send msg), or c (check for msgs): ";
		char req;
		cin >> req;
		
		//quit
		if (req == 'q') break; 

		//Send Dialog 
		if (req == 's'){
			cout << "Enter Buddy Name: ";
			string buddy;
			cin >> buddy;
			cout << "Enter message here: ";
			string userMsg;
			cin >> userMsg;

			//adding new line
			buddy += '\n';

			//increment msg
			int randMsgPlus = ++randMsgNum;
			//Construct Buddy Message
			string sMsg = to_string(randMsgPlus) + ";2;" + crypt((username + '\n')) + crypt(buddy) + crypt(userMsg);
			message = const_cast<char*>(sMsg.c_str());

			//Send Message
			if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&server, sizeof(server)) < 0)	{
				cout << "Send Failed" << endl;
				return 1;
			}
			if ((recv_size = recvfrom(s, server_reply, 500, 0, NULL, NULL)) == SOCKET_ERROR)	{
				cout << "recv failed" << endl;
				return 1;
			}
			cout << "Msg Sent Successfully" << endl;
		}


		if (req == 'c'){
			//Send some data	
			string sMsg = to_string(randMsgNum++) + ";1;" + crypt(username);
			message = const_cast<char*>(sMsg.c_str());

			if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&server, sizeof(server)) < 0)	{
				cout << "Send Failed" << endl;
				return 1;
			}
			cout << "Msg Sent" << endl;
		}

		//Receive a reply from the server	
		if ((recv_size = recvfrom(s, server_reply, 500, 0, NULL, NULL)) == SOCKET_ERROR)	{
			cout << "recv failed" << endl;
			return 1;
		}
		

		//Add \0 at the end of received string string before printing	
		server_reply[recv_size] = '\0';	//puts(server_reply);	
		string res = deCrypt(server_reply);
		cout << res << endl;


	}

	closesocket(s);
	WSACleanup();
	system("pause"); 
	return 0;
}



void listen(void * temp){
	double conversion = *(double *)temp;
	//Receive a reply from the server	
	if ((recv_size = recvfrom(s, server_reply, 500, 0, NULL, NULL)) == SOCKET_ERROR)	{
		cout << "recv failed" << endl;
	}
	cout << server_reply << endl;

}


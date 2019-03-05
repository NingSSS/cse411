#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "node.h"
#include "convert.h"
#define MAX_LINE 40960
#define DEFAULT_PORT 8000
using namespace std;
void printHelp();
int main(int argc, char* argv[])
{
	if (argc < 2) {
		cout << "usage: ./client ip_address" << endl;
		return 0;
	}

	int socket_fd;
	struct sockaddr_in server_addr;
	//creat socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "Creat a  socket error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
		exit(0);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(DEFAULT_PORT);
	if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
		cout << "get address error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
		exit(0);
	}

	int on = 1;
	if ((setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
		perror("setsockopt error\n");
		exit(0);
	}
	
	if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		cout << "connect error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
		exit(0);
	}
	
	char sendline[MAX_LINE], recvline[MAX_LINE];
	char buff[MAX_LINE];
	printHelp();
	while (true) {
		cout << "send a command to server: " << endl;
		fgets(sendline, MAX_LINE, stdin);
		sendline[strlen(sendline)-1] = '\0';
		if (send(socket_fd, sendline, strlen(sendline), 0) < 0) {
			cout << "send messege to server error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
			exit(0);
		}
		string send_str = sendline;
		if (send_str == "exit") {
			cout << "1 client has exited" << endl;
			break;
		}
		if (send_str == "help") {
			printHelp();
			continue;
		}
		
		int len;
		if ((len = recv(socket_fd, buff, MAX_LINE, 0)) < 0) {
			perror("receive from server error\n");
			exit(0);
		}
		buff[len] = '\0';
		Node* recv_node = StrToNode(buff);
		if (recv_node == NULL) {
			cout << buff<< endl;
		} else if (recv_node->key == "get") {
			if (recv_node->value == "failed")
				cout << "The key is not existed in server" << endl;
			else
				cout << "The key's vlaue: " << recv_node->value << endl;
        }else if(recv_node->key == "del"){
            if (recv_node->value == "failed")
                cout << "The key is not existed in server" << endl;
            else
                cout << "The key is deleted." << endl;
        }else if (recv_node->key == "put") {
			 if (recv_node->value == "success")
				cout << "put success." << endl;
			else
				cout << "put failed" << endl;
        } else if (recv_node->key == "re") {
            cout<<recv_node->value<<endl;
            if (recv_node->value == "failed")
                cout << "The key is not existed in server" << endl;
            else
                cout <<"The key and The key's vlaue: " << recv_node->value << endl;
        }else {
			cout << buff << endl;
		}
	}
	
	close(socket_fd);
	return 0;
}

void printHelp() {
	cout << "get [key]            search a key and get the key's value." << endl \
	     << "put [key]-[value]    put or set a data node(key and value)." << endl \
         << "del [key]    delete a data node(key and value) into server." << endl \
         << "where [regex]    use regex to get value." << endl \
	     << "save                 save the server's cache data" << endl \
	     << "help                 print this help" << endl \
	     << "exit                 kill the client" << endl \
	     << "shutdown    shutdown the server" << endl;
}

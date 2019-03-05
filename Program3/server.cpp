#include <unordered_map>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <iostream>
#include "node.h"
#include "convert.h"
#include "hash_cache.h"
#include <regex>


using namespace std;
#define DEFAULT_PORT  8000
#define MAX_LINE 40960
//#define CACHE_SIZE 1000
#define LEN (KEY_SIZE+VALUE_SIZE+2)
static mutex put_mutex, get_mutex;
static class HashCache cache;
static void handle_client_requests(int connected_fd); // thread function
int main(int argc, char* argv[])
{
	int socket_fd, connected_fd;
	char buff[MAX_LINE];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_addr_size = sizeof(server_addr);
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cout << "Creat a socket error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
		exit(0);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	server_addr.sin_port = htons(DEFAULT_PORT);
	int on = 1;
	if ((setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
		perror("setsockopt error\n");
		exit(0);
	}
	if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		cout << "bind  socket error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
		exit(0);
	}
	if (listen(socket_fd, 10) == -1) {
		cout << "listen  socket error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
		exit(0);
	}
	cout << "=====Waiting for clients's request=====\n" << endl;
	while(true) {
		if ((connected_fd = 
			accept(socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&sin_addr_size)) == -1) {
			cout << "accept  socket error: " << strerror(errno) << " (errno:" << errno << ")" << endl;
			continue;
		}
		cout << "client " << connected_fd - 3 << " is connecting." << endl;
		cout << "IP: " << inet_ntoa(client_addr.sin_addr) << "  " << "Port: " << htons(client_addr.sin_port) << endl;
		thread client(handle_client_requests, connected_fd);
		client.detach();
	}
	close(socket_fd);
	cout << "server finish" << endl;
	return 0;
}


static void handle_client_requests(int connected_fd) 
{
	cout << "client " << connected_fd - 3 << " connected success" << endl; // 0~3
	char buff[MAX_LINE];
	memset(buff, 0, MAX_LINE);
	buff[0] = '\0';
	while (true) {
		int len = recv(connected_fd, buff, MAX_LINE, 0);  // 0 is a flag
		if (len == 0)
			continue;
		vector<string> recv_vector = split_str(buff, " ;:");
		if (recv_vector[0] == "exit") {
			break;
		}
		if (recv_vector[0] == "save") {
			cache.save_cache();
			static const char* save_info = "All cache has saved to files. You can shutdown the server safely.";
			if(send(connected_fd, save_info, MAX_LINE, 0) == -1) {
                perror("send error");
			}
			cout << save_info << endl;
			continue;
		}
        if(recv_vector[0] == "shutdown"){
            cout << "server finish" << endl;
            exit(0);
            break;
        }
		memset(buff, 0, MAX_LINE);
		if (recv_vector.size() == 2 && recv_vector[0] == "get"){
			put_mutex.lock();
			string value = cache.get(recv_vector[1]);
			put_mutex.unlock();
			if(send(connected_fd, ("get-"+value).c_str(), MAX_LINE, 0) == -1) {
                perror("send error");
			}
			if (!value.empty()) {
				cout << "search success, key = " << recv_vector[1] << " and value = " << value << endl;
			} else {
				cout << "search failed, there is no key = " <<  recv_vector[1] << endl;
			}
			
		} else if (recv_vector.size() == 3 && recv_vector[0] == "put") {
			put_mutex.lock();
			get_mutex.lock();
			cache.put(recv_vector[1], recv_vector[2]);
			get_mutex.unlock();
			put_mutex.unlock();
			if(send(connected_fd, "put-success", MAX_LINE, 0) == -1) {
                perror("send error");
			}
			cout << "put success" << endl;
        } else if(recv_vector.size() == 2 && recv_vector[0] == "del"){
            put_mutex.lock();
            get_mutex.lock();
            string value = cache.get(recv_vector[1]);
            cache.del(recv_vector[1]);
            get_mutex.unlock();
            put_mutex.unlock();
            if(send(connected_fd, ("del-"+value).c_str(), MAX_LINE, 0) == -1) {
                perror("send error");
            }
            if (!value.empty()) {
                cout << "delete key =" << recv_vector[1] << endl;
            }else {
                cout << "search failed, there is no key = " <<  recv_vector[1] << endl;
            }
            
        }else if(recv_vector.size() == 2 && recv_vector[0] == "where"){
            put_mutex.lock();
            get_mutex.lock();
            string value=cache.where(recv_vector[1]);
//            string abc;
//            put_mutex.unlock();
//            for(int i=0;i<100;i++){
//                if(value[i]==""){
//                    break;
//                }else{
//                    abc= abc+value[i];
//                }
//            }
            get_mutex.unlock();
            put_mutex.unlock();
            if(send(connected_fd, (value).c_str(), MAX_LINE, 0) == -1) {
                perror("send error");
            }
            if (!value.empty()) {
                cout << "search success"<<endl;
            } else {
                cout << "search failed" << endl;
            }
        }else {
			static const char* input_error_info = "Input command error, try again.";
			if(send(connected_fd, input_error_info, MAX_LINE, 0) == -1)
                perror("send error");
			cout << input_error_info << endl;
		}
		cout << "==================================================" << endl;
	}
	cout << "client " << connected_fd - 3 << " has exited" << endl;
	close(connected_fd);
}

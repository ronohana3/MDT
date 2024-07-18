#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class SocketClient 
{
public:
    SocketClient(std::string hostAddress, int port);
    ~SocketClient() {};
    int Open();
    int Close();
    int SendPayload(char * payload);
    int Receive(char * buffer, size_t bufferSize);

private:
    int m_port;
    std::string m_hostAddress;
    struct sockaddr_in m_serverAddress;
    WSADATA m_wsaData;
    SOCKET m_sockfd;
};
#endif
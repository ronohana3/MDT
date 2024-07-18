#include "SocketClient.hpp"
#include <iostream>


SocketClient::SocketClient(std::string hostAddress, int port)
{
    m_port = port;
    m_hostAddress = hostAddress;

}
int SocketClient::Open()
{
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }

    if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return -1;
    }

    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_port = htons(m_port);
    inet_pton(AF_INET, m_hostAddress.data(), &m_serverAddress.sin_addr);

    if (connect(m_sockfd, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        closesocket(m_sockfd);
        WSACleanup();
        return -1;
    }

    return 0;
}

int SocketClient::Close()
{
    closesocket(m_sockfd);
    WSACleanup();
    return 0;
}

int SocketClient::SendPayload(char *payload)
{
    return send(m_sockfd, payload, strlen(payload), 0);
}

int SocketClient::Receive(char *buffer, size_t bufferSize)
{
    return recv(m_sockfd, buffer, bufferSize, 0);
}


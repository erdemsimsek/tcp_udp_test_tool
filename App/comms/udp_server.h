#pragma once

#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace comms{
class UdpServer{
public:

    using rxCbFxn = void(*)(const char * msg, size_t msgLen);

    UdpServer(rxCbFxn rxCallbackFunc, size_t portNumber) : 
        rxCballbackFunc{rxCallbackFunc}, 
        socket{ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), portNumber)}
    {

    }

    UdpServer(rxCbFxn rxCallbackFunc, unsigned long ipAddr, size_t portNumber) : 
        rxCballbackFunc{rxCallbackFunc}, 
        socket{ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4(ipAddr), portNumber)}
    {

    }

    void sendMessage(std::string message)
    {
        auto msgTpSend = std::make_shared<std::string>(message);
        socket.async_send_to(boost::asio::buffer(*msgTpSend), remoteEndpoint,
                boost::bind(&UdpServer::handleSend, this, msgTpSend,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }

    void runComms()
    {
        startListenning();
        ioService.run();
    }

private:
    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint remoteEndpoint;
    rxCbFxn rxCballbackFunc;
    std::array<char, 1024> buffer;

    void handleReceivedData(const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if (!error || error == boost::asio::error::message_size) {
            std::string receivedMessage{buffer.begin()};
            std::cout << receivedMessage;

            if(nullptr != rxCballbackFunc)
            {
                rxCballbackFunc(buffer.begin(), bytesTransferred);
            }

            startListenning();
        }
    }

    void handleSend(std::shared_ptr<std::string> message,
                    const boost::system::error_code& ec,
                    std::size_t bytes_transferred) {
    }

    void startListenning()
    {
        socket.async_receive_from(
            boost::asio::buffer(buffer), remoteEndpoint,
            boost::bind(&UdpServer::handleReceivedData, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }


};

}




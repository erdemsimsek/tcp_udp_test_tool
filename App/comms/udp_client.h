#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace comms{

class UdpClient{
public:

    using rxCbFxn = void(*)(const char * msg, size_t msgLen);

    UdpClient(rxCbFxn rxCallbackFunc, std::string ipAddr, size_t portNumber) : 
        rxCballbackFunc{rxCallbackFunc},
        endpoint{boost::asio::ip::make_address(ipAddr, errCode), static_cast<unsigned short>(portNumber)},
        socket{ioService}
    {

    }

    void sendMessage(std::string message)
    {
        auto msgTpSend = std::make_shared<std::string>(message);
        socket.async_send_to(boost::asio::buffer(*msgTpSend), endpoint,
                boost::bind(&UdpClient::handleSend, this, msgTpSend,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }

    void runComms()
    {
        socket.connect(endpoint);
        startListenning();
        ioService.run();
    }

private:
    boost::system::error_code errCode;
    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint endpoint;
    
    rxCbFxn rxCballbackFunc;
    std::array<char, 1024> buffer;

    void handleReceivedData(const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if (!error || error == boost::asio::error::message_size) {

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
            boost::asio::buffer(buffer), endpoint,
            boost::bind(&UdpClient::handleReceivedData, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }


};

}
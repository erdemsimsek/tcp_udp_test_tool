#pragma once

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>



class UdpServer{
public:

using rxCbFxn = void(*)(char * msg, size_t msgLen);

    UdpServer(rxCbFxn rxCallbackFunc, size_t portNumber) : 
        rxCballbackFunc{rxCallbackFunc}, 
        socket{ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), portNumber)}
    {

    }

    void startListenning()
    {
        socket.async_receive_from(
            boost::asio::buffer(buffer), remoteEndpoint,
            boost::bind(&UdpServer::handleReceivedData, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void execute()
    {
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
            std::cout << receivedMessage << "\n";

            auto message = std::make_shared<std::string>("Hello, World\n");

            socket.async_send_to(boost::asio::buffer(*message), remoteEndpoint,
                boost::bind(&UdpServer::handleSend, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
    }

    void handleSend(std::shared_ptr<std::string> message,
                    const boost::system::error_code& ec,
                    std::size_t bytes_transferred) {
        startListenning();
    }


};



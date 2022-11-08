#pragma once

#include <string>
#include <thread>
#include <tuple>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace comms{
class UdpServer{
public:

    using rxCbFxn = void(*)(const char * msg, size_t msgLen);
    using clientDetails = std::tuple<boost::asio::ip::address, unsigned short>;

    UdpServer(rxCbFxn rxCallbackFunc, size_t portNumber) : 
        rxCballbackFunc{rxCallbackFunc}
    {
        try{
            socket.open(boost::asio::ip::udp::v4());
            socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), portNumber));
        }
        catch(boost::system::system_error& e)
        {
            throw;
        }
    }

    void sendMessage(std::string message, clientDetails& client)
    {
        auto msgTpSend = std::make_shared<std::string>(message);
        socket.async_send_to(
            boost::asio::buffer(*msgTpSend), 
            boost::asio::ip::udp::endpoint(std::get<0>(client), std::get<1>(client)),
            boost::bind(&UdpServer::handleSend, this, msgTpSend,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }

    void runComms()
    {
        startListenning();
        ioService.run();
    }

private:
    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket{ioService};
    boost::asio::ip::udp::endpoint remoteEndpoint;
    rxCbFxn rxCballbackFunc;
    std::vector<clientDetails> connectionHistory;
    std::array<char, 1024> buffer;

    void handleReceivedData(const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if(std::find_if(connectionHistory.begin(), 
                        connectionHistory.end(), 
                        [this](const clientDetails& e) {return std::get<0>(e) == remoteEndpoint.address();}) == connectionHistory.end())
        {
            connectionHistory.emplace_back(remoteEndpoint.address(), remoteEndpoint.port());
        }

        if (!error || error == boost::asio::error::message_size) {
            std::string receivedMessage{buffer.begin()};
            if(nullptr != rxCballbackFunc)
            {
                rxCballbackFunc(buffer.begin(), bytesTransferred);
            }

            startListenning();
        }
    }

    void handleSend(std::shared_ptr<std::string> message,
                    const boost::system::error_code& error,
                    std::size_t bytes_transferred) {
        if(error.value())
        {
            throw std::runtime_error(error.message());
        }
    }

    void startListenning()
    {
        socket.async_receive_from(
            boost::asio::buffer(buffer),
            remoteEndpoint,
            boost::bind(&UdpServer::handleReceivedData, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }
};

}




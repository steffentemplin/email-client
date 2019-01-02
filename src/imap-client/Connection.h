#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "Protocol.h"

class Connection;

class Authenticator {
public:
    virtual void authenticate(Connection& con) = 0;
    virtual ~Authenticator() {}
};

class UsernamePasswordAuthenticator : public Authenticator {
public:
    UsernamePasswordAuthenticator() = delete;
    UsernamePasswordAuthenticator(const std::string username, const std::string password);
    virtual void authenticate(Connection& con) override;

    const std::string& getUsername() const;
    const std::string& getPassword() const;
private:
    const std::string username;
    const std::string password;
};

class Connection {
public:
    Connection();
    Connection(const std::string host, const uint16_t port, Authenticator& authenticator);
    Connection(Connection&&) = default;
    Connection& operator= (Connection&&) = default;
    Connection(const Connection&) = delete;
    Connection& operator= (const Connection&) = delete;
    ~Connection();

    void open(const std::string host, const uint16_t port, Authenticator& authenticator);
    Response exec(const Command& cmd);
    const Capabilities& getPreAuthCapabilities() const;
    const Capabilities& getCapabilities() const;
    void close();


private:
    uint16_t tagIndex;
    Capabilities preauthCaps;
    Capabilities caps;

    struct SocketConnection;
    std::unique_ptr<SocketConnection> socketCon;

    struct SocketConnection {
        bool ssl;
        std::unique_ptr<boost::asio::io_context> ioContext;
        boost::asio::ip::tcp::socket socket;

        virtual size_t sendSync(boost::asio::streambuf& request);
        virtual size_t readUntil(boost::asio::streambuf& response, std::string delim);
        virtual void close();
        
        SocketConnection();
        SocketConnection(SocketConnection&& other);
        virtual ~SocketConnection();
    };

    struct SSLSocketConnection : SocketConnection {
        boost::asio::ssl::context sslContext;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> sslSocket;

        virtual size_t sendSync(boost::asio::streambuf& request) override;
        virtual size_t readUntil(boost::asio::streambuf& response, std::string delim) override;
        virtual void close() override;

        SSLSocketConnection(SocketConnection&& other);
        ~SSLSocketConnection();
    };

    class ResponseParser {
    public:
        ResponseParser();
        ~ResponseParser() = default;
        void setSocketConnection(SocketConnection* socketCon);

        Response expectTagged(const std::string& tag);
        Response expectTagged(const std::string& tag, ResponseStatus status);
        Response expectUntagged(const size_t numLines);
        Response expectUntagged(const size_t numLines, ResponseStatus status);
    
    private:
        SocketConnection* socketCon;
        boost::asio::streambuf responseBuffer;

    };

    ResponseParser parser;

};

#endif // _CONNECTION_H_
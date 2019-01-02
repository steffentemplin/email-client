#include "Connection.h"
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;

const bool TRACE = false;

void checkGreeting(const Response& response);

Connection::Connection() : tagIndex{0}, preauthCaps{""}, caps{""}, socketCon{nullptr}, parser{}
{

}

Connection::Connection(const std::string host, const uint16_t port, Authenticator& authenticator) : Connection()
{
    open(host, port, authenticator);
}

void Connection::close()
{
    if (socketCon) {
        try {
            Logout logoutCmd;
            exec(logoutCmd);
        } catch (exception& e) {
            cout << "Error during logout: " << e.what() << endl;
        }

        socketCon.reset();
    }
}

Connection::~Connection()
{
    close();    
}

void logCommand(boost::asio::streambuf& request)
{
    if (TRACE) {
        auto data = request.data();
        string request_line(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + request.size() - 2);
        cout << "Request:" << endl;
        cout << "  " << request_line << endl;
    }
}

void logResponse(Response& response)
{
    if (TRACE) {
        cout << "Response:" << endl;
        auto lines = response.getRawResponse();
        for (auto const& line : lines) {
            cout << "  " << line << endl;
        }
    }
}

void Connection::open(const string host, const uint16_t port, Authenticator& authenticator)
{
    if (socketCon) {
        throw runtime_error("Connection is already open");
    }

    socketCon = unique_ptr<SocketConnection>(new SocketConnection);
    // Get a list of endpoints corresponding to the server name.
    boost::asio::ip::tcp::resolver resolver(*(socketCon->ioContext));
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, to_string(port));

    // Try each endpoint until we successfully establish a connection.
    boost::asio::connect(socketCon->socket, endpoints);
    parser.setSocketConnection(socketCon.get());

    checkGreeting(parser.expectUntagged(1));

    Capability capCmd;
    Response capResp = exec(capCmd);
    preauthCaps = capCmd.parseResponse(capResp);
    
    //TODO: pass from outside
    const bool startTls = true;
    if (startTls && preauthCaps.has("STARTTLS")) {
        StartTLS stlsCmd;
        auto stlsResp = exec(stlsCmd);
        stlsResp.expectTaggedOK();

        // do TLS
        SocketConnection* oldCon = socketCon.release();
        socketCon = unique_ptr<SocketConnection>(new SSLSocketConnection(move(*oldCon)));
        parser.setSocketConnection(socketCon.get());

        capResp = exec(capCmd);
        preauthCaps = capCmd.parseResponse(capResp);
    }

    authenticator.authenticate(*this);

    capResp = exec(capCmd);
    caps = capCmd.parseResponse(capResp);
}

Response Connection::exec(const Command& cmd)
{
    if (tagIndex >= 9999) {
        tagIndex = 0;
    }

    boost::asio::streambuf request;
    ostream request_stream(&request);
    auto tag = (boost::format("A%04d") % ++tagIndex).str();
    request_stream << tag << ' ';
    cmd.writeTo(request_stream);
    request_stream << "\r\n";

    logCommand(request);
    socketCon->sendSync(request);
    auto response = parser.expectTagged(tag);
    logResponse(response);

    return response;
}

const Capabilities& Connection::getPreAuthCapabilities() const
{
    return preauthCaps;
}

const Capabilities& Connection::getCapabilities() const
{
    return caps;
}

UsernamePasswordAuthenticator::UsernamePasswordAuthenticator(const string username_, const string password_)
    :username{move(username_)}, password{move(password_)}
{

}

const string& UsernamePasswordAuthenticator::getUsername() const
{
    return username;
}

const string& UsernamePasswordAuthenticator::getPassword() const
{
    return password;
}

class Login : public Command {
public:
    Login() = delete;
    Login(const UsernamePasswordAuthenticator& auth) : auth{auth} {}

    void writeTo(ostream& out) const override
    {
        out << "LOGIN " << auth.getUsername() << " " << auth.getPassword();
    }
private:
    const UsernamePasswordAuthenticator& auth;
};

void UsernamePasswordAuthenticator::authenticate(Connection& con)
{
    if (!con.getPreAuthCapabilities().has("AUTH=PLAIN")) {
        throw ProtocolException();
    }

    Login loginCmd{*this};
    con.exec(loginCmd);
    // TODO: check response
}

void checkGreeting(const Response& response)
{
    auto greeting = response.getRawResponse().front();
    if (!boost::starts_with(greeting, "* ")) {
        throw runtime_error("Received invalid server greeting: " + greeting);
    }

    auto rest = greeting.substr(2); // length check?
    if (boost::starts_with(rest, "OK")) {
        return;
    } else if (boost::starts_with(rest, "BYE")) {
        throw runtime_error("Connection denied by server: " + rest);
    } else {
        // TODO: support PREAUTH
        throw runtime_error("Unsupported server greeting: " + rest);
    }
}

Connection::SocketConnection::SocketConnection() : ssl{false}, ioContext{new boost::asio::io_context}, socket{*ioContext}
{

}

Connection::SocketConnection::SocketConnection(SocketConnection&& other)
    : ssl{false}, ioContext{move(other.ioContext)}, socket{move(other.socket)}
{

}

Connection::SocketConnection::~SocketConnection()
{
    close();
}

size_t Connection::SocketConnection::sendSync(boost::asio::streambuf& request)
{
    return boost::asio::write(socket, request);
}

size_t Connection::SocketConnection::readUntil(boost::asio::streambuf& response, std::string delim)
{
    return boost::asio::read_until(socket, response, delim);
}

void Connection::SocketConnection::close()
{
    if (socket.is_open()) {
        socket.close();
    }
}

Connection::SSLSocketConnection::SSLSocketConnection(SocketConnection&& other)
    : SocketConnection(move(other)), sslContext{boost::asio::ssl::context::sslv23}, sslSocket{socket, sslContext}
{
    try {
        sslContext.set_verify_mode(boost::asio::ssl::verify_none); // TODO: check hostname and cert...
        sslSocket.handshake(boost::asio::ssl::stream_base::handshake_type::client);
        ssl = true;
    } catch (system_error& e) {
        cout << "SSL handshake failed: " << e.what() << endl;
        throw runtime_error("SSL handshake failed");
    }
}

Connection::SSLSocketConnection::~SSLSocketConnection()
{
    close();
}

size_t Connection::SSLSocketConnection::sendSync(boost::asio::streambuf& request)
{
    return boost::asio::write(sslSocket, request);
}

size_t Connection::SSLSocketConnection::readUntil(boost::asio::streambuf& response, std::string delim)
{
    return boost::asio::read_until(sslSocket, response, delim);
}

void Connection::SSLSocketConnection::close()
{
    if (socket.is_open()) {
        socket.close();
    }
}

Connection::ResponseParser::ResponseParser() : socketCon{nullptr}, responseBuffer{}
{

}

void Connection::ResponseParser::setSocketConnection(SocketConnection* socketCon)
{
    this->socketCon = socketCon;
}

Response Connection::ResponseParser::expectTagged(const std::string& tag)
{
    if (!socketCon) {
        throw runtime_error("No socket connection has been set");
    }

    vector<string> lines;
    bool done = false;
    while (!done) {
        auto n = socketCon->readUntil(responseBuffer, "\r\n");
        auto data = responseBuffer.data();
        string line(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + n - 2);
        responseBuffer.consume(n);

        if (boost::starts_with(line, tag + " ")) {
            done = true;
        }

        lines.push_back(line);
    }

    return Response{lines, tag};
}

// TODO: implement response status check
Response Connection::ResponseParser::expectTagged(const std::string& tag, ResponseStatus status)
{
    return expectTagged(tag);
}

Response Connection::ResponseParser::expectUntagged(const size_t numLines)
{
    if (!socketCon) {
        throw runtime_error("No socket connection has been set");
    }

    vector<string> lines;
    for (uint16_t i = 0; i < numLines; i++) {
        auto n = socketCon->readUntil(responseBuffer, "\r\n");
        auto data = responseBuffer.data();
        string line(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + n - 2);
        responseBuffer.consume(n);

        lines.push_back(line);
    }

    return Response{lines};
}

// TODO: implement response status check
Response Connection::ResponseParser::expectUntagged(const size_t numLines, ResponseStatus status)
{
    return expectUntagged(numLines);
}

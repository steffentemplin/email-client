//
//  Protocol.h
//  email-client
//
//  Created by Steffen Templin on 09.12.18.
//  Copyright © 2018 Steffen Templin. All rights reserved.
//

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <exception>
#include <boost/asio.hpp>

class Command {
public:
    virtual void writeTo(std::ostream& out) const = 0;
    virtual ~Command() {}
};

enum class ResponseStatus {
    OK,
    NO,
    BAD,
    BYE,
    PREAUTH,
};

class Response {
public:
    Response(std::vector<std::string> rawResponse_) : rawResponse{std::move(rawResponse_)}, tag{NOTAG} {};
    Response(std::vector<std::string> rawResponse_, std::string tag_) : rawResponse{std::move(rawResponse_)}, tag{std::move(tag_)} {};
    const std::vector<std::string>& getRawResponse() const { return rawResponse; };
    void expectTaggedOK();
    bool isTagged() const;
    const std::string& getTag() const;

    static const std::string NOTAG;

private:
    std::vector<std::string> rawResponse;
    std::string tag;
    
};

class ProtocolException : public std::exception {
public:
    virtual const char* what() const throw();
};

class Capabilities {
public:
    explicit Capabilities(std::string raw);
    bool has(const std::string& cap) const;
    const std::unordered_set<std::string>& getAll() const;
    const std::string& getRaw() const;
private:
    std::string raw;
    std::unordered_set<std::string> capSet;
};

class Capability : public Command {
public:
    void writeTo(std::ostream& out) const override;
    Capabilities parseResponse(const Response& response) const;
};

class Logout : public Command {
public:
    void writeTo(std::ostream& out) const override;
};

class StartTLS : public Command {
public:
    void writeTo(std::ostream& out) const override;
};

class NamespaceResult : public Command {
public:
    void writeTo(std::ostream& out) const override;
    std::vector<std::pair<std::string, std::string>>& getPrivate();
    std::vector<std::pair<std::string, std::string>>& getShared();
    std::vector<std::pair<std::string, std::string>>& getPublic();
};

class Primitive {
public:
    virtual void writeTo(std::ostream out);
};

class Atom : Primitive {
public:
    Atom(std::string value): value{std::move(value)} {};
private:
    std::string value;
};

#endif /* _PROTOCOL_H_ */

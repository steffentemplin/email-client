//
//  Protocol.cc
//  email-client
//
//  Created by Steffen Templin on 09.12.18.
//  Copyright © 2018 Steffen Templin. All rights reserved.
//

#include "Protocol.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;

const char* ProtocolException::what() const throw()
{
    return "Error while parsing response";
}

Capabilities::Capabilities(const string raw_) : raw{move(raw_)}, capSet{32}
{
    boost::char_separator<char> sep{" "};
    boost::tokenizer<boost::char_separator<char>> tokenizer{raw, sep};
    for (const auto &t : tokenizer) {
        capSet.insert(t);
    }
}

const string& Capabilities::getRaw() const
{
    return raw;
}

bool Capabilities::has(const std::string& cap) const
{
    return capSet.find(cap) != capSet.end();
}

const std::unordered_set<std::string>& Capabilities::getAll() const
{
    return capSet;
}

void Capability::writeTo(ostream& out) const
{
    out << "CAPABILITY";
}

Capabilities Capability::parseResponse(const Response& response) const
{
    auto lines = response.getRawResponse();
    if (lines.size() < 2) {
        throw ProtocolException();
    }

    string prefix = "* CAPABILITY ";

    for (const auto& line : lines) {
        if (boost::starts_with(line, prefix)) {
            auto capStr = line.substr(prefix.length());
            return Capabilities{capStr};
        }
    }

    throw ProtocolException();
}

void Logout::writeTo(ostream& out) const
{
    out << "LOGOUT";
}

void StartTLS::writeTo(ostream& out) const
{
    out << "STARTTLS";
}

const string Response::NOTAG = "";

void Response::expectTaggedOK()
{
    auto taggedLine = rawResponse.back();
    if (!boost::starts_with(taggedLine, tag + " OK")) {
        throw ProtocolException();
    }
}

bool Response::isTagged() const
{
    return tag != NOTAG;
}

const std::string& Response::getTag() const
{
    return tag;
}

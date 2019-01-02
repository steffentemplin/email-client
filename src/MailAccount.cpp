//
//  mail_account.cc
//  email-client
//
//  Created by Steffen Templin on 08.12.18.
//  Copyright © 2018 Steffen Templin. All rights reserved.
//

#include "MailAccount.h"

using namespace std;

SecuritySettings::SecuritySettings(TransportEncryption transportEncryption, bool disallowPlain, bool allowUntrustedCert)
    : transportEncryption{transportEncryption}, disallowPlain{disallowPlain}, allowUntrustedCert{allowUntrustedCert} {};

MailAccount::MailAccount(uint32_t id, std::string name, ProtocolType type, std::string host, uint16_t port,
        SecuritySettings& securitySettings, std::string login, std::string password)
    : id{id}, name{move(name)}, type{type}, host{move(host)}, port{port}, securitySettings{securitySettings},
        login{move(login)}, password{move(password)} {};

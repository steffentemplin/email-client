//
//  mail_account.h
//  email-client
//
//  Created by Steffen Templin on 08.12.18.
//  Copyright © 2018 Steffen Templin. All rights reserved.
//

#ifndef _MAIL_ACCOUNT_H_
#define _MAIL_ACCOUNT_H_

#include <string>

enum class ProtocolType { imap };

enum class TransportEncryption { tls, starttls, none };

struct SecuritySettings
{
    SecuritySettings(TransportEncryption transportEncryption, bool disallowPlain, bool allowUntrustedCert);

    TransportEncryption transportEncryption;
    bool disallowPlain;
    bool allowUntrustedCert;
};

struct MailAccount
{
    MailAccount(uint32_t id, std::string name, ProtocolType type, std::string host, uint16_t port,
            SecuritySettings& securitySettings, std::string login, std::string password);

    uint32_t id;
    
    std::string name;
    ProtocolType type;
    std::string host;
    uint16_t port;
    SecuritySettings& securitySettings;
    
    std::string login;
    std::string password;
};


#endif /* _MAIL_ACCOUNT_H_ */

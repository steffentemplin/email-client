//
//  main.cc
//  email-client
//
//  Created by Steffen Templin on 08.12.18.
//  Copyright © 2018 Steffen Templin. All rights reserved.
//

#include <iostream>
#include "MailAccount.h"
#include "imap-client/Connection.h"

using namespace std;

Capabilities getCapabilities()
{
    SecuritySettings ss{TransportEncryption::starttls, true, true};
    MailAccount acc{0, "test-account", ProtocolType::imap, "localhost", 143, ss, "user@example.com", "secret"};    
    
    UsernamePasswordAuthenticator auth{acc.login, acc.password};
    Connection con{acc.host, acc.port, auth};
    //con.open("localhost", 143, auth);
    return con.getCapabilities();
}

int main(int argc, const char * argv[]) {
    Capabilities caps = getCapabilities();
    cout << "Got capabilities: " << caps.getRaw() << endl;
    cout << "  Parsed:";
    for (const auto& c : caps.getAll()) {
        cout << " " << c;
    }
    cout << endl;
    cout << "  Has IMAP4rev1: " << to_string(caps.has("IMAP4rev1")) << endl;

    return 0;
}

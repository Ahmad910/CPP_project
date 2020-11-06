/* myserver.cc: sample server program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocolViolationException.h"
#include "server.h"
#include "db.h"
#include "dbDisk.h"
#include "inmemory_db.h"
#include "sessionHandler.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <map>

using namespace std;


DB* database;
map <Connection*,SessionHandler*> handler;


Server init(int argc, char* argv[])
{
        if (argc < 2 || argc > 3) {
                cerr << "Usage: myserver port-number mode\n mode = 'inMemory'|'disk'" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[1]);
        } catch (exception& e) {
                cerr << "Wrong format for port number. " << e.what() << endl;
                exit(2);
        }

        Server server(port);
        if (!server.isReady()) {
                cerr << "Server initialization error." << endl;
                exit(3);
        }

	if(argc == 2 || !string("disk").compare(argv[2])) {
		database = new dbDisk();
	} else if(!string("inMemory").compare(argv[2])){
		database = new InMemory_db();
	} else {
		cerr << "wrong format for mode\nHas to be either 'disk' or 'inMemory'" << endl;
		exit(1);
	}

        return server;
}

int main(int argc, char* argv[])
{
        Server server = init(argc, argv);

        while (true) {
                auto conn = server.waitForActivity();
                if (conn != nullptr) {
                        try {
                                SessionHandler *h = handler[conn];
                                h->handleCommand();
                        } catch (ConnectionClosedException&) {
                                handler.erase(conn);
                                server.deregisterConnection(conn);
                                cout << "connection closed" << endl;
                        } catch (ProtocolViolationException&) {
                                handler.erase(conn);
                                server.deregisterConnection(conn);
                                cout << "protocol violated" << endl;
                        }
                } else {
                        conn = new Connection();
                        server.registerConnection(conn);
			                  SessionHandler sh (database,conn);
			                  handler.insert({conn,&sh});
                        cout << "client connected" << endl;
                }
        }
        free(database);
        return 0;
}

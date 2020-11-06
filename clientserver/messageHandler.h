#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "protocol.h"

#include <string>
#include <memory>


using namespace std;

/* A MessageHandler handles the low level communication protocol */
class MessageHandler {
  public:
        /* Creates a MessageHandler that uses conn  */
       MessageHandler(Connection* conn);

       ~MessageHandler();

	     void sendCode(Protocol code) const;

       void sendInt(unsigned int value) const ;

	     void sendIntParameter(unsigned int param) const;

	     void sendStringParameter(const string &param) const;

	     Protocol rcvCode() const;

	     unsigned int rcvInt() const;

	     unsigned int rcvIntParameter() const;

	     string* rcvStringParameter() const;

  private:

	     Connection* conn;

	     void sendByte(unsigned int code) const;

	     unsigned int rcvByte() const;

};

#endif

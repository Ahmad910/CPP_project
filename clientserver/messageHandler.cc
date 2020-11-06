#ifndef MESSAGEHANDLER_CC
#define MESSAGEHANDLER_CC

#include "connection.h"
#include "protocol.h"
#include "messageHandler.h"
#include "protocolViolationException.h"

#include <string>
#include <iostream>

using namespace std;

/* Creates a MessageHandler that uses conn  */
MessageHandler::MessageHandler(Connection* conn)
{
	this->conn = conn;
}

MessageHandler::~MessageHandler() {
	this->conn = nullptr;
}

void MessageHandler::sendCode(Protocol code) const
{
	sendByte(static_cast<unsigned int>(code));
}

void MessageHandler::sendInt(unsigned int value) const
{
	sendByte((value >> 24) & 0xFF);
	sendByte((value >> 16) & 0xFF);
	sendByte((value >>  8) & 0xFF);
	sendByte((value >>  0) & 0xFF);
}

void MessageHandler::sendIntParameter(unsigned int param) const
{
	sendCode(Protocol::PAR_NUM);
	sendInt(param);
}

void MessageHandler::sendStringParameter(const string &param) const
{
	sendCode(Protocol::PAR_STRING);
	sendInt(param.length());
	for(unsigned int i = 0; i < param.length(); i++) {
		sendByte(param[i]);
	}
}

Protocol MessageHandler::rcvCode() const
{
	return static_cast<Protocol>(rcvByte());
}

unsigned int MessageHandler::rcvInt() const
{
	unsigned int res = 0;
	for(int i = 0; i < 4; i++) {
		res = res << 8;
		res += rcvByte();
	}
	return res;
}

unsigned int MessageHandler::rcvIntParameter() const
{
	Protocol code = rcvCode();
	if(code != Protocol::PAR_NUM) {
		throw ProtocolViolationException();
	}
	return rcvInt();
}

string* MessageHandler::rcvStringParameter() const
{
	Protocol code = rcvCode();
	if(code != Protocol::PAR_STRING) {
		throw ProtocolViolationException();
	}
	int length = rcvInt();
	string* r = new string();
	for(int i = 0; i < length; i++) {
		r->push_back(rcvByte());
	}
	return r;
}

void MessageHandler::sendByte(unsigned int code) const
{
	(*this->conn).write(code);
}

unsigned int MessageHandler::rcvByte() const
{
	return this->conn->read();
}

#endif

#ifndef SESSIONHANDLER_CC
#define SESSIONHANDLER_CC

#include "sessionHandler.h"
#include "messageHandler.h"
#include "protocol.h"
#include "newsGroupAlreadyExistsException.h"
#include "newsGroupDoesnotExistException.h"
#include "articleDoesnotExistException.h"
#include "protocolViolationException.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

/* Creates a SessionHandler that uses conn  */
SessionHandler::SessionHandler(DB* database, Connection* conn)
{
	this->database = database;
	this->handler = new MessageHandler(conn);
}

void SessionHandler::handleCommand() const {
	Protocol code = this->handler->rcvCode();
	switch(code) {
		case Protocol::COM_LIST_NG : listGroups();
			break;
		case Protocol::COM_CREATE_NG : createGroup();
			break;
		case Protocol::COM_DELETE_NG : deleteGroup();
			break;
		case Protocol::COM_LIST_ART : listArticles();
			break;
		case Protocol::COM_CREATE_ART : createArticle();
			break;
		case Protocol::COM_DELETE_ART : deleteArticle();
			break;
		case Protocol::COM_GET_ART : getArticle();
			break;
		default: throw ProtocolViolationException();
	}
}

void SessionHandler::assertEnd() const
{
	Protocol end = this->handler->rcvCode();
	if(end != Protocol::COM_END) {
		throw ProtocolViolationException();
	}
}

void SessionHandler::listGroups() const
{
	this->assertEnd();

	map<unsigned int,string> groups = this->database->getGroups();

	this->handler->sendCode(Protocol::ANS_LIST_NG);
	this->handler->sendIntParameter(groups.size());
	for(map<unsigned int,string>::iterator it = groups.begin(); it != groups.end(); it++) {
		this->handler->sendIntParameter(it->first);
		this->handler->sendStringParameter(it->second);
	}

	this->handler->sendCode(Protocol::ANS_END);
}

void SessionHandler::createGroup() const
{
	string *name = this->handler->rcvStringParameter();
	this->assertEnd();
	try {
		this->database->createGroup(*name);
		this->handler->sendCode(Protocol::ANS_CREATE_NG);
		this->handler->sendCode(Protocol::ANS_ACK);
	} catch (NewsGroupAlreadyExistsException&) {
		this->handler->sendCode(Protocol::ANS_CREATE_NG);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
	}

	delete name;

	this->handler->sendCode(Protocol::ANS_END);
}

void SessionHandler::deleteGroup() const
{
	int groupId = handler->rcvIntParameter();
	this->assertEnd();

	try {
		this->database->deleteGroup(groupId);
		this->handler->sendCode(Protocol::ANS_DELETE_NG);
		this->handler->sendCode(Protocol::ANS_ACK);
	} catch (NewsGroupDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_DELETE_NG);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	this->handler->sendCode(Protocol::ANS_END);

}

void SessionHandler::listArticles() const
{
	int groupId = handler->rcvIntParameter();
	this->assertEnd();

	try {
		vector<pair<unsigned int, string>> articles = this->database->getArticles(groupId);
		this->handler->sendCode(Protocol::ANS_LIST_ART);
		this->handler->sendCode(Protocol::ANS_ACK);
		this->handler->sendIntParameter(articles.size());
		for(vector<pair<unsigned int, string>>::iterator it = articles.begin(); it != articles.end(); ++it) {
			this->handler->sendIntParameter(it->first);
			this->handler->sendStringParameter(it->second);
		}
	} catch (NewsGroupDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_LIST_ART);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}

	this->handler->sendCode(Protocol::ANS_END);
}

void SessionHandler::createArticle() const
{
	cout << "create article" << endl;
	int groupId = this->handler->rcvIntParameter();
	cout << groupId << endl;
	string *title = this->handler->rcvStringParameter();
	cout << *title << endl;
	string *author = this->handler->rcvStringParameter();
	cout << *author << endl;
	string *text = this->handler->rcvStringParameter();
	cout << *text << endl;
	this->assertEnd();

	try {
		this->database->createArticle(groupId,*title,*author,*text);
		this->handler->sendCode(Protocol::ANS_CREATE_ART);
		this->handler->sendCode(Protocol::ANS_ACK);
	} catch (NewsGroupDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_CREATE_ART);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}

	delete title;
	delete author;
	delete text;

	this->handler->sendCode(Protocol::ANS_END);
}

void SessionHandler::deleteArticle() const
{
	int groupId = this->handler->rcvIntParameter();
	int articleId = this->handler->rcvIntParameter();
	this->assertEnd();

	try {
		this->database->deleteArticle(groupId,articleId);
		this->handler->sendCode(Protocol::ANS_DELETE_ART);
		this->handler->sendCode(Protocol::ANS_ACK);
	} catch (NewsGroupDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_DELETE_ART);
    this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	} catch (ArticleDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_DELETE_ART);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
	}

	this->handler->sendCode(Protocol::ANS_END);
}

void SessionHandler::getArticle() const
{
	int groupId = this->handler->rcvIntParameter();
	int articleId = this->handler->rcvIntParameter();
	this->assertEnd();

	try {
		tuple<string,string,string> article = this->database->getSpecificArticle(groupId,articleId);
		this->handler->sendCode(Protocol::ANS_GET_ART);
		this->handler->sendCode(Protocol::ANS_ACK);
		this->handler->sendStringParameter(std::get<0>(article));
		this->handler->sendStringParameter(std::get<1>(article));
		this->handler->sendStringParameter(std::get<2>(article));
	} catch (NewsGroupDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_GET_ART);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	} catch (ArticleDoesNotExistException&) {
		this->handler->sendCode(Protocol::ANS_GET_ART);
		this->handler->sendCode(Protocol::ANS_NAK);
		this->handler->sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
	}

	this->handler->sendCode(Protocol::ANS_END);
}

#endif

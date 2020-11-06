#ifndef SESSIONHANDLER_H
#define SESSIONHANDLER_H

#include "connection.h"
#include "messageHandler.h"
#include "db.h"

/* A MessageHandler handles the low level communication protocol */
class SessionHandler {
      public:
        /* Creates a MessageHandler that uses conn  */
        SessionHandler(DB* database, Connection* conn);

	      void handleCommand() const;

      private:

	DB* database;
	MessageHandler* handler;

	void listGroups() const;
	void createGroup() const;
	void deleteGroup() const;
	void listArticles() const;
	void createArticle() const;
	void deleteArticle() const;
	void getArticle() const;

  void assertEnd() const;

};

#endif

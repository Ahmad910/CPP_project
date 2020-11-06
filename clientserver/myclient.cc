/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "messageHandler.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

MessageHandler* messageHandler;
unsigned int groupId;
unsigned int articleId;

void bar(int size) {
  if(size == 1) {
    cout << "------------------------------------------------------------------" << endl;
  } else if(size == 2) {
    cout << "==================================================================" << endl;
  }
}

void enterGroupId(){
  cout << "Enter the group identification number: ";
  cin >> groupId;
}
void enterArticleId(){
  cout << "Enter the article identification number: ";
  cin >> articleId;
}
void createArticle(){
  enterGroupId();

  string title;
  cout << "Enter the title of the article: ";
  cin >> title;
  string author;
  cout << "Enter the author name: ";
  cin >> author;
  string text;
  string s;
  cout << "Enter the text of the article and when you finish, enter * :" << endl;
  while (cin >> s && s != "*"){

    text = text + s + " ";
  }

  messageHandler->sendCode(Protocol::COM_CREATE_ART);

  messageHandler->sendIntParameter(groupId);
  messageHandler->sendStringParameter(title);

  messageHandler->sendStringParameter(author);

  messageHandler->sendStringParameter(text);

  messageHandler->sendCode(Protocol::COM_END);

  bar(1);

  messageHandler->rcvCode();
  if(messageHandler->rcvCode() == Protocol::ANS_ACK){
    cout << "Article with title " << title << " was created successfully" << endl;
  }else{
    cout << "Failed to create article with title " << title << endl;
    if(messageHandler->rcvCode() == Protocol::ERR_NG_DOES_NOT_EXIST){
      cout << "The group with id " << groupId << " does not exist" << endl;
    }
  }
  messageHandler->rcvCode();
}
void deleteArticle(){
  enterGroupId();
  enterArticleId();
  messageHandler->sendCode(Protocol::COM_DELETE_ART);
  messageHandler->sendIntParameter(groupId);
  messageHandler->sendIntParameter(articleId);
  messageHandler->sendCode(Protocol::COM_END);
  bar(1);
  messageHandler->rcvCode();
  if(messageHandler->rcvCode() == Protocol::ANS_ACK){
    cout << "The article with id " << articleId << " has been deleted from group " << groupId << endl;
  }else{
    cout << "Failed to delete article with id " << articleId << endl;
    if(messageHandler->rcvCode() == Protocol::ERR_NG_DOES_NOT_EXIST){
      cout << "The group with id " << groupId << " does not exist" << endl;
    }
    if(messageHandler->rcvCode() == Protocol::ERR_ART_DOES_NOT_EXIST){
      cout << "Article with id" << articleId << " does not exist" << endl;
    }
  }
  messageHandler->rcvCode();
}
void getArticles(){
  enterGroupId();
  messageHandler->sendCode(Protocol::COM_LIST_ART);
  messageHandler->sendIntParameter(groupId);
  messageHandler->sendCode(Protocol::COM_END);
  bar(1);
  messageHandler->rcvCode();
  int articleId;
  if(messageHandler->rcvCode() == Protocol::ANS_ACK){
    int size = messageHandler->rcvIntParameter();
    cout << "Listing " << size << " articles with article id and the title of the article:" << endl;
    for(int i = 0; i != size; ++i){
      articleId = messageHandler->rcvIntParameter();
      cout << articleId << " ";
      string *title = messageHandler->rcvStringParameter();
      cout << *title << endl;
      delete(title);
    }
  }else{
    cout << "Failed to get articles for group with id: " << groupId << endl;
    if(messageHandler->rcvCode() == Protocol::ERR_NG_DOES_NOT_EXIST){
      cout << "The group with id " << groupId << " does not exist" << endl;
    }
  }
  messageHandler->rcvCode();
}
void getSpecificArticle(){
  enterGroupId();
  enterArticleId();
  messageHandler->sendCode(Protocol::COM_GET_ART);
  messageHandler->sendIntParameter(groupId);
  messageHandler->sendIntParameter(articleId);
  messageHandler->sendCode(Protocol::COM_END);
  bar(1);
  messageHandler->rcvCode();
  if(messageHandler->rcvCode() == Protocol::ANS_ACK){
    string *title = messageHandler->rcvStringParameter();
    string *author = messageHandler->rcvStringParameter();
    string *text = messageHandler->rcvStringParameter();
    cout << "The title is: " << *title << endl;
    cout << "The author is: " << *author << endl;
    cout << "The text is: " << endl;
    cout << *text << endl;
    delete(title);
    delete(author);
    delete(text);
  }else{
    cout << "Failed to get articles for group with id: " << groupId << endl;
    if(messageHandler->rcvCode() == Protocol::ERR_NG_DOES_NOT_EXIST){
      cout << "The group with id " << groupId << " does not exist" << endl;
    }
  }
  messageHandler->rcvCode();
}


string commandNames[8];

void listGroups() {
  messageHandler->sendCode(Protocol::COM_LIST_NG);
  messageHandler->sendCode(Protocol::COM_END);
  bar(1);
  messageHandler->rcvCode();
  int groupId;


    int size = messageHandler->rcvIntParameter();
    cout << "Listing " << size << " news groups: "<<endl;
    for(int i = 0; i != size; ++i){
      groupId = messageHandler->rcvIntParameter();
      cout << groupId << " ";
      string *name = messageHandler->rcvStringParameter();
      cout << *name << endl;
      delete(name);
    }

  messageHandler->rcvCode();
}

void createGroup() {


  string title;
  cout << "Enter the title of the group: ";
  cin >> title;

  messageHandler->sendCode(Protocol::COM_CREATE_NG);
  messageHandler->sendStringParameter(title);
  messageHandler->sendCode(Protocol::COM_END);
  bar(1);
  messageHandler->rcvCode();
  if(messageHandler->rcvCode() == Protocol::ANS_ACK){
    cout << "News group with title " << title << " was created successfully" << endl;
  }else{
    cout << "Failed to create news group with title " << title << endl;
    if(messageHandler->rcvCode() == Protocol:: ERR_NG_ALREADY_EXISTS){
      cout << "The  news group with title " << title << " already exists" << endl;
    }
  }
  messageHandler->rcvCode();
}

void deleteGroup() {
 enterGroupId();
  messageHandler->sendCode(Protocol::COM_DELETE_NG);
  messageHandler->sendIntParameter(groupId);
  messageHandler->sendCode(Protocol::COM_END);
  bar(1);
  messageHandler->rcvCode();
  if(messageHandler->rcvCode() == Protocol::ANS_ACK){
    cout << "The news group with id " << groupId << " has been deleted. " << endl;
  }else{
    cout << "Failed to delete news group with id " << groupId << endl;
    if(messageHandler->rcvCode() == Protocol::ERR_NG_DOES_NOT_EXIST){
      cout << "The news group with id " << groupId << " does not exist" << endl;
    }
  }
  messageHandler->rcvCode();
}

void printCommands() {
  for(int i = 0; i < 8; ++i) {
    cout << (i+1) << " " << commandNames[i] << endl;
  }
  cout << endl;
}

void commandLoop() {
  unsigned int command = 0;
  while(command != 8) {
    bar(2);
    printCommands();
    cout << "pick a command" << endl;
    cout << ">";
    cin >> command;
    switch (command) {
      case 1 : listGroups();
      break;
      case 2 : createGroup();
      break;
      case 3 : deleteGroup();
      break;
      case 4 : getArticles();
      break;
      case 5 : createArticle();
      break;
      case 6 : deleteArticle();
      break;
      case 7 : getSpecificArticle();
      break;
      case 8 : break;
      default: cout << endl << "invalid input" << endl;
    }
  }
}

void initCommands() {
  commandNames[0] = "list groups";
  commandNames[1] = "create group";
  commandNames[2] = "delete group";
  commandNames[3] = "list articles";
  commandNames[4] = "create article";
  commandNames[5] = "delete article";
  commandNames[6] = "get article";
  commandNames[7] = "quit";

}

Connection init(int argc, char* argv[])
{
        if (argc != 3) {
                cerr << "Usage: myclient host-name port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[2]);
        } catch (exception& e) {
                cerr << "Wrong port number. " << e.what() << endl;
                exit(2);
        }

        Connection conn(argv[1], port);
        if (!conn.isConnected()) {
                cerr << "Connection attempt failed" << endl;
                exit(3);
        }

        return conn;
}




int main(int argc, char* argv[])
{
  Connection conn = init(argc, argv);
  messageHandler = new MessageHandler(&conn);

  initCommands();

  commandLoop();

  return 0;
}

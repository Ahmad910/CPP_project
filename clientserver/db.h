#ifndef DB_H
#define DB_H

#include <vector>
#include <string>
#include <utility>
#include <map>
#include "newsGroupAlreadyExistsException.h"
#include "newsGroupDoesnotExistException.h"
#include "articleDoesnotExistException.h"

using namespace std;

class DB{
public:
	virtual ~DB() = default;

	virtual map<unsigned int, string> getGroups() const = 0;
	virtual bool createGroup(string) = 0;
	virtual bool deleteGroup(unsigned int) = 0;
	virtual vector<pair<unsigned int, string>> getArticles(unsigned int) = 0;
	virtual bool createArticle(unsigned int, string, string, string) = 0;
	virtual bool deleteArticle(unsigned int, unsigned int) = 0;
	virtual tuple<string, string, string> getSpecificArticle(unsigned int, unsigned int) = 0;

};
#endif

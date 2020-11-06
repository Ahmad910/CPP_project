#ifndef DBDISK_H
#define DBDISK_H

#include <vector>
#include <string>
#include <utility>
#include <map>
#include "db.h"

using namespace std;

class dbDisk : public DB{
public:
	dbDisk();
	virtual map<unsigned int, string> getGroups() const; 
	virtual bool createGroup(string);
	virtual bool deleteGroup(unsigned int);
	virtual vector<pair<unsigned int, string>> getArticles(unsigned int);
	virtual bool createArticle(unsigned int, string, string, string);
	virtual bool deleteArticle(unsigned int, unsigned int);
	virtual tuple<string, string, string> getSpecificArticle(unsigned int, unsigned int);
private:
	unsigned int groupId;
	unsigned int articleId;
};
#endif

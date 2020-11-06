#ifndef INMEMORYdb_H
#define INMEMORYdb_H
#include <vector>
#include <map>
#include <string>
#include "db.h"
#include "newsgroup.h"
#include "article.h"
#include "newsGroupAlreadyExistsException.h"
#include "newsGroupDoesnotExistException.h"
#include "articleDoesnotExistException.h"
using std::string;

class InMemory_db: public DB {

public:
  //InMemory_db() {}

  virtual std::map<unsigned int, string> getGroups() const ;
	virtual bool createGroup(string) ;
	virtual bool deleteGroup(unsigned int) ;
	virtual std::vector<pair<unsigned int, string>> getArticles(unsigned int) ;
	virtual bool createArticle(unsigned int, string, string, string) ;
	virtual bool deleteArticle(unsigned int, unsigned int) ;
	virtual std::tuple<string, string, string> getSpecificArticle(unsigned int, unsigned int) ;
private:
                    std::vector<NewsGroup> newsgr_vect;
};
#endif

#include <iostream>
#include "inmemory_db.h"
#include "article.h"
#include "db.h"
#include "newsgroup.h"
#include "newsGroupAlreadyExistsException.h"
#include "newsGroupDoesnotExistException.h"
#include "articleDoesnotExistException.h"

using namespace std;

int main(){


           InMemory_db inmdb;
           
           inmdb.createGroup("gruppo 1");
           inmdb.createGroup("gruppo 2");
           inmdb.createGroup("gruppo 3");
           inmdb.createArticle(1, "titolo 1", "Sara", "testo articolo 1");
           
           map<unsigned int, string> mappa= inmdb.getGroups();
           for(auto el: mappa)
              cout<< el.first<< el.second<< endl;
           
           inmdb.deleteGroup(3);
           
           map<unsigned int, string> mapp= inmdb.getGroups();
           for(auto el: mapp)
              cout<< el.first<< el.second<< endl;
              
           try{
           inmdb.deleteGroup(5); //throw nogroup
           }catch (NewsGroupDoesNotExistException()){cout<< "group does not exist" <<endl;}
           
            try{
           inmdb.createArticle(10, "titolo 10", "Sara", "testo articolo 10"); //throw no grouo
           }
           catch(ArticleDoesNotExistException()){cout<< "article does not exist" <<endl;}

           try{
           inmdb.createGroup("gruppo 2");//throw articleexists
           }
           catch(NewsGroupAlreadyExistsException()){cout<< "group already exist" <<endl;}
           inmdb.deleteArticle(1,1);
           


}

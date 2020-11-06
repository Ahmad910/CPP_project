#ifndef INMEMORY_DB_CC
#define INMEMORY_DB_CC

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "inmemory_db.h"
#include "newsgroup.h"
using std::string;
using std::find_if;

                    std::map<unsigned int, string> InMemory_db::getGroups() const{
                          std::map<unsigned int, string> groups_map;
                          for(auto el: newsgr_vect){
                          groups_map[el.get_id()]=el.get_name();
                          }
                          return groups_map;
                     }

	bool InMemory_db::createGroup(string namn){
	      auto itr=find_if(newsgr_vect.begin(), newsgr_vect.end(), [=](const NewsGroup& ngr){return namn==ngr.get_name(); });
	      if(itr != newsgr_vect.end())
	          throw NewsGroupAlreadyExistsException();

                           newsgr_vect.emplace_back(namn);
                           return true;
	}


	bool InMemory_db::deleteGroup(unsigned int id_gr){
	    auto itr= find_if(newsgr_vect.begin(), newsgr_vect.end(), [=](const NewsGroup& ngr){return id_gr==ngr.get_id(); });
	    if(itr == newsgr_vect.end())
	        throw NewsGroupDoesNotExistException();

                        newsgr_vect.erase(itr);
                        return true;
	}




	std::vector<std::pair<unsigned int, string>> InMemory_db::getArticles(unsigned int id_gr) {
	     std::vector<std::pair<unsigned int, string>> articles_vect;
                         auto itr = find_if(newsgr_vect.begin(), newsgr_vect.end(), [=](const NewsGroup& ngr){return id_gr==ngr.get_id(); });
                         if(itr==newsgr_vect.end())
	        throw NewsGroupDoesNotExistException();
	     if(itr != newsgr_vect.end()){
                         vector<Article> articles= itr ->get_articles();
                         for(auto el: articles)
                            articles_vect.push_back(std::make_pair(el.get_id(), el.get_title()));
                          }
                         return  articles_vect;
	}




	bool InMemory_db::createArticle(unsigned int id_gr, string title, string author, string text){
                          auto itr = find_if(newsgr_vect.begin(), newsgr_vect.end(), [=](const NewsGroup& ngr){return id_gr==ngr.get_id(); });
                          if(itr==newsgr_vect.end())
	        throw NewsGroupDoesNotExistException();

                          (*itr).get_articles().emplace_back(title, author, text);
                          return true;
	}




	bool InMemory_db::deleteArticle(unsigned int id_gr, unsigned int id_art){
	     //find groupnews with id == id_gr, throw exception if not found
                         auto itr_gr = find_if(newsgr_vect.begin(), newsgr_vect.end(), [=](const NewsGroup& ngr){return id_gr==ngr.get_id(); });
                         if(itr_gr==newsgr_vect.end())
	        throw NewsGroupDoesNotExistException();
                         //inside groupnews wih id= id_gr find article with id==id_art and delete it. Throw exception if not found.
                        vector<Article>& articles= (*itr_gr).get_articles();
                         auto itr_art=find_if(articles.begin(), articles.end(), [=](const Article& art)
                         {return id_art==art.get_id(); });
                         if(itr_art == articles.end())
                             throw ArticleDoesNotExistException ();

                         (*itr_gr).get_articles().erase(itr_art);
                         return true;

	}


	std::tuple<string, string, string> InMemory_db::getSpecificArticle(unsigned int id_gr, unsigned int id_art){
                         //find groupnews with id== id_gr. Throw exception if not found
                         auto itr_gr = find_if(newsgr_vect.begin(), newsgr_vect.end(), [=](const NewsGroup& ngr){return id_gr==ngr.get_id(); });
                         if(itr_gr==newsgr_vect.end())
	             throw NewsGroupDoesNotExistException();
	      //inside groupnews wih id==id_gr find article with id= id_art. Throw exception if not found
                         vector<Article>& articles= (*itr_gr).get_articles();
                         auto itr_art= find_if(articles.begin(), articles.end(), [=](const Article& art)
                         {return id_art==art.get_id(); });

                         if(itr_art == articles.end())
                             throw ArticleDoesNotExistException ();

                         return std::make_tuple((*itr_art).get_title(), (*itr_art).get_author(), (*itr_art).get_text());


	}

#endif

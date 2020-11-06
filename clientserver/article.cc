#include<string>
#include "article.h"
using std::string;

              static unsigned int id_articles=0;

               Article::Article(const string ttl, const string athr, const string txt): title{ttl}, author{athr}, text{txt},id{id_articles++}{
                //id= id_articles++;
                }
                unsigned int Article::get_id()const { return id;}
                
                string Article::get_title()const { return title;} 
                
                string Article::get_author()const { return author;}
                
                string Article::get_text()const { return text;}

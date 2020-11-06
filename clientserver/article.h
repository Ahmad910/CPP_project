#ifndef ARTICLE_H
#define ARTICLE_H
#include<string>
using std::string;

class Article{
public:     
                

                Article()=delete;
                Article(const string ttl, const string athr, const string txt);
                unsigned int get_id()const ;
                string get_title()const ;
                string get_author()const ;
                string get_text()const ;
                
                
                


private:
                
                string title;
                string author;
                string text;
                 unsigned int id;
};
#endif

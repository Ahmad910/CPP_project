#ifndef NEWSGROUP_H
#define NEWSGROUP_H
#include<string>
#include "article.h"
#include <vector>

using std::string;
class NewsGroup{
public:        
             static unsigned int id_groups;

              NewsGroup()=delete;
              NewsGroup(string nm);
              unsigned int get_id()const;
              string get_name()const;
              std::vector<Article>& get_articles();
              
              

private: 
              std::vector<Article> article_list;

              unsigned int id_group;
              
              string name;



};
#endif

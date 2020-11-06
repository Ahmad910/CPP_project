#include<string>
#include "newsgroup.h"
#include "article.h"


              unsigned int NewsGroup::id_groups=1;
              
              NewsGroup::NewsGroup(string nm): name{nm}{id_group = id_groups++;}

              std::vector<Article>& NewsGroup::get_articles() {return article_list;}
              
              unsigned int NewsGroup::get_id()const{return id_group;}
              
              string NewsGroup::get_name()const {return name;}

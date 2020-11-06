#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <map>
#include <string>
class Database {

public:
                virtual std::pair <int, std::map<int, std::string>> getGroups() = 0;
                //for server impl: read bool and write ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS from server
                virtual bool createGroup(const std::string& group_name) = 0; 
                //for server impl:read bool and write ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS from server
                virtual bool deleteGroup(const int id_group) = 0; 
               //for server impl:read bool and write [ANS_ACK num_p [num_p string_p]* |ANS_NAK ERR_NG_DOES_NOT_EXIST rom server
                virtual std::pair <int, std::map<int, std::string>> getArticle(int id_group) = 0; 
                //for server impl:read bool and write [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] from server
                virtual bool createArticle(const int id_group, const std::string title, const std::string author, const std::string text ) = 0; 
                //for server impl: read bool (first bool: existing group, second bool : existing article)
                //T [ANS_ACK | ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]]
                virtual std::pair <bool,bool> deleteArticle(const int id_group, const int id_article) = 0; 
                virtual std::tuple <std::string ,std::string, std::string > getArticle(const int id_group, const int id_article) = 0; 

};
#endif
                
                
                
                
                
              

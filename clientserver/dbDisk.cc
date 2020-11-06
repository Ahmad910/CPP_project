#include <iostream>
#include "dbDisk.h"
#include <algorithm>
#include <fstream>
#include <ios>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include "articleDoesnotExistException.h"
#include "newsGroupAlreadyExistsException.h"
#include "newsGroupDoesnotExistException.h"
#include <tuple>
#include <string>
#include <iterator>

using namespace std;

dbDisk::dbDisk(){
	ifstream groupId_in("groupId.txt");
	//The groupId file is closed, so we create a new such file
	if(!(groupId_in.is_open())){
		ofstream groupId_out("groupId.txt");
		groupId_out << 1 << endl;
		groupId = 1;
	}else{ // the file is open
		groupId_in >> groupId;
		groupId_in.close();
	}

	ifstream articleId_in("articleId.txt");
	if(!(articleId_in.is_open())){
		ofstream articleId_out("articleId.txt");
		articleId_out << 1 << endl;
		articleId = 1;
	}else{
		articleId_in >> articleId;
		articleId_in.close();
	}

	ifstream newsGroups_in("newsGroups.txt");
	if(!(newsGroups_in.is_open())){
		ofstream newsGroups_out("newsGroups.txt");
	}else{
		newsGroups_in.close();
	}
}
map<unsigned int, string> dbDisk::getGroups() const {
	map<unsigned int, string> groups;
	unsigned int id;
	ifstream groupId_in("newsGroups.txt");
	string line;
	while(getline(groupId_in, line)) {

		istringstream iss(line);

		iss >> id;
		string group;
		string s;

		while(iss >> s) {

			group = group + s + " ";
		}
		groups[id] = group.substr(0,group.length()-1);
	}
	return groups;
}
bool dbDisk::createGroup(string groupName){
	ifstream newsGroups_in("newsGroups.txt");
	bool checkIfExists = false;
	string line;
	while(getline(newsGroups_in, line)){
		istringstream iss(line);

		string s1;
		iss >> s1; // the group number id

		string s2;
		while(iss >> s1){
			s2 = s2 + s1 + " ";
		}
		if(groupName == s2.substr(0, s2.size() - 1)){
			checkIfExists = true;
		}
		newsGroups_in.close();
		if(checkIfExists){
			throw NewsGroupAlreadyExistsException();
		}
	}
		ofstream newsGroups_out("newsGroups.txt", ios::app); // we use ios::app in order to not overwrite
		newsGroups_out << groupId << " " << groupName << endl;


		mkdir(to_string(groupId).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		//increasing the groupId
		ofstream groupId_out("groupId.txt");
		++groupId;
		groupId_out << groupId << endl;
		return true;
}

bool dbDisk::deleteGroup(unsigned int groupId){
	string sGroupId = to_string(groupId);
	DIR* dirp = opendir(sGroupId.c_str());
	if(!dirp){
		throw NewsGroupDoesNotExistException();
	}
	struct dirent* dp;
	while((dp = readdir(dirp)) != NULL){
		remove((sGroupId + "/" + string(dp->d_name)).c_str());
	}
	remove(sGroupId.c_str());

	ifstream groupId_in("newsGroups.txt");
	ofstream tmpFile("tmpFile.txt");
	string line;
	unsigned int id;
	while(getline(groupId_in, line)){
		istringstream iss(line);
		iss >> id;
		if(groupId != id){
			tmpFile << line << endl;
		}
	}
	tmpFile.close();
	groupId_in.close();
	closedir(dirp);
	remove(sGroupId.c_str());
	rename("tmpFile.txt", "newsGroups.txt");
	return true;
}

vector<pair<unsigned int, string>> dbDisk::getArticles(unsigned int groupId){
	string sGroupId = to_string(groupId);
	DIR* dirp = opendir(sGroupId.c_str());

	if(!dirp){
		throw NewsGroupDoesNotExistException();
	}

	struct dirent* dp;
	vector<pair<unsigned int, string>> groupArticles;
	while((dp = readdir(dirp)) != NULL){
		string article_id;
		string article_name = (dp->d_name);
		if(article_name[0] != '.'){
			istringstream iss(article_name);
			unsigned char c;
			while(iss >> c && c != '.'){
				article_id += c;
			}
			unsigned int artId;
			istringstream iss2(article_id);
			iss2 >> artId;
			ifstream artIn(sGroupId + "/" + article_name);
			string title;
			getline(artIn, title);
			groupArticles.push_back(make_pair(artId, title));
		}
	}
	sort(groupArticles.begin(), groupArticles.end());
	closedir(dirp);
	return groupArticles;
}

bool dbDisk::createArticle(unsigned int groupId, string title, string author, string text){
	string sGroupId = to_string(groupId);
	DIR* dirp = opendir(sGroupId.c_str());
	if(!dirp){

		throw NewsGroupDoesNotExistException();
	}
	ofstream articleId_out(sGroupId + "/" + to_string(articleId) + ".txt");
	articleId_out << title << endl;
	articleId_out << author << endl;
	articleId_out << text << endl;

	ofstream out("articleId.txt");
	++articleId;
	out << articleId << endl;
	closedir(dirp);
	return true;
}

bool dbDisk::deleteArticle(unsigned int groupId, unsigned int articleId){
	string sGroupId = to_string(groupId);
	DIR* dirp = opendir(sGroupId.c_str());
	if(!dirp){
		throw NewsGroupDoesNotExistException();
	}
	closedir(dirp);
	string artId = to_string(articleId);
	ifstream articleId_in(sGroupId + "/" + artId + ".txt");
	if(!(articleId_in.is_open())){
		throw ArticleDoesNotExistException();
	}
	remove((sGroupId + "/" + artId + ".txt").c_str());
	articleId_in.close();
	return true;
}

tuple<string, string, string> dbDisk::getSpecificArticle(unsigned int groupId, unsigned int articleId){
	string sGroupId = to_string(groupId);
	DIR* dirp = opendir(sGroupId.c_str());
	if(!dirp){
		throw NewsGroupDoesNotExistException();
	}
	closedir(dirp);
	string artId = to_string(articleId);
	ifstream articleId_in(sGroupId + "/" + artId + ".txt");
	string title;
	string author;
	string text;
	if(!articleId_in.is_open()){
		throw ArticleDoesNotExistException();
	}
	getline(articleId_in, title);
	getline(articleId_in, author);

	string tmp;
	while(getline(articleId_in, tmp)){
		text = text + tmp + "\n";
	}


	return make_tuple(title, author, text.substr(0, text.size() - 1));
}

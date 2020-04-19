#include "Path.h"
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include "StringUtils.h"

#include <iostream> //debugging: "std::cout<<"@ line: "<<__LINE__<<std::endl;"

struct CPath::SImplementation{
    // Your code goes here
	std::vector< std::string > places = {};	//a vector of all thje different parts of the path
};

CPath::CPath() : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here
	DImplementation->places.push_back("");	//an empty path is empty
} 

CPath::CPath(const CPath &path) : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here
	DImplementation->places = path.DImplementation->places;	//a path that is a copy
} 

CPath::CPath(const std::string &path): DImplementation(std::make_unique< SImplementation >()) {
    // Your code goes here
	this->DImplementation->places = StringUtils::Split(path, "/"); //a path is divided by slashes
}

CPath::~CPath(){
    // Your code goes here	
	//idk what goes here, not much tho
}

CPath &CPath::operator=(const CPath &path){
    // Your code goes here  
	this->DImplementation->places.clear();	//clear the original path
	
	//replace the original path
	for(unsigned int i = 0; i < path.DImplementation->places.size(); i++){
		this->DImplementation->places.push_back(path.DImplementation->places[i]);
	}
	
	return *this;
}

CPath CPath::operator+(const CPath &path) const{
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();	//needed to remove the empty slot
	//since ret was spawned with an empty path, places[0] == "" without this
	
	//combine both paths
	for(unsigned int i = 0; i < this->DImplementation->places.size(); i++){
		ret.DImplementation->places.push_back(this->DImplementation->places[i]);	
	}
	for(unsigned int i = 0; i < path.DImplementation->places.size(); i++){
		ret.DImplementation->places.push_back(path.DImplementation->places[i]);	
	}
	
	return ret;
}

bool CPath::operator==(const CPath &path) const{
    // Your code goes here
	for(unsigned int i = 0; i < path.DImplementation->places.size(); i++){
		if(this->DImplementation->places[i] != path.DImplementation->places[i]){
			return false;	//if they are different 
		}
	} 
	
	return true;	//no differences found
}

bool CPath::operator!=(const CPath &path) const{
    // Your code goes here
	//if != is the opposite of ==
	if(*this == path){
		return false;
	}else{
		return true;
	}
}

CPath CPath::Directory() const{
    // Your code goes here
	//spawns ret on the root of this
	CPath ret(this->DImplementation->places[0]); 
	//also this takes into account an empty path as it will set ret and not start the for loop
	
	for(unsigned int i = 1; i < this->DImplementation->places.size() - 1; i++){
		ret.DImplementation->places.push_back(this->DImplementation->places[i]);
	}
	
	return ret;
}

std::string CPath::BaseName() const{
	if (this->DImplementation->places[this->DImplementation->places.size() - 1] == ""){
		DImplementation->places.pop_back();
	}
	return DImplementation->places[this->DImplementation->places.size() - 1];
}

std::string CPath::Extension() const{
    // Your code goes here
	std::vector< std::string > tempstr; //the BaseName split up
	std::string retstr = "";	//where the extensioon goes
	
	tempstr = StringUtils::Split(this->DImplementation->places[this->DImplementation->places.size() - 1],".");
	
	if(tempstr.size() > 1){
		retstr += ".";
		for(size_t i = 0; i < (tempstr[tempstr.size() - 1]).length(); i++){
				retstr += (tempstr[tempstr.size() - 1])[i];
		}
	}
	
	return retstr;
}

bool CPath::IsAbsolute() const{
    // Your code goes here
	
	//empty paths arnt absolute so > 1,
	//absstrings start with \ meaning the first item is ""
	if((this->DImplementation->places.size() > 1)
	&& (this->DImplementation->places[0] == "")){
		return true;
	}else{
		return false;
	}
}

std::string CPath::ToString() const{
    // Your code goes here
	std::string tempstr;	
	for(unsigned int i = 0; i < this->DImplementation->places.size() - 1; i++){
		tempstr += DImplementation->places[i];
		tempstr += "/";	//paths are divided by /
	}
	tempstr += DImplementation->places[DImplementation->places.size() - 1]; //done to avoid trailing /
	
	return tempstr;
}

CPath::operator std::string() const{
    // Your code goes here
	std::string tempstr;
	if(DImplementation->places.size() == 1 && DImplementation->places[0].empty()){
		return "/";
	}

	for(unsigned int i = 0; i < this->DImplementation->places.size() - 1; i++){
		tempstr += DImplementation->places[i];
		tempstr += "/";
	}
	tempstr += DImplementation->places[DImplementation->places.size() - 1];
	
	return tempstr;
}

CPath CPath::AbsolutePath() const{
    // Your code goes here
	//if empty or absolute no work needed
	if(this->IsAbsolute() || this->DImplementation->places[0] == ""){
		return *this;
	}

	CPath ret(GetCWD());
	
	for(unsigned int i = 1; i < this->DImplementation->places.size(); i++){	//skips leading dot
		ret.DImplementation->places.push_back(this->DImplementation->places[i]);
	}
	
	return ret;
}

CPath CPath::CommonPath(const CPath &path) const{
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();

	//just keep going until the paths arent common anymore
	unsigned int i = 0;
	while(i < this->DImplementation->places.size() && i < path.DImplementation->places.size()){
		if(this->DImplementation->places[i] != path.DImplementation->places[i]){
			break;
		}
		ret.DImplementation->places.push_back(this->DImplementation->places[i]);
		i++;
	}
	
	return ret;
}

CPath CPath::NormalizePath() const{
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();
	for(unsigned int i = 0; i < this->DImplementation->places.size(); i++){
		//if .. then prev item is removed
		if(this->DImplementation->places[i].compare("..") == 0){
			if(ret.DImplementation->places.size() > 0){	
				ret.DImplementation->places.pop_back();
			}
		//skip .s 
		}else if(this->DImplementation->places[i].compare(".") == 0){
			//nothing here
		//otherwise carry on
		}else{
			ret.DImplementation->places.push_back(this->DImplementation->places[i]);
		}
	}

	unsigned int j = 1;	
	while(j < ret.DImplementation->places.size()){
		if(ret.DImplementation->places[j].compare("") == 0){
			ret.DImplementation->places.erase(ret.DImplementation->places.begin() + j);
		}else{
			j++;
		}
	}
	
	if(ret.DImplementation->places.size() == 0){
		ret.DImplementation->places.push_back("");
	}
		
	return ret;
}

CPath CPath::RelativePathTo(const CPath &path) const{
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();
	
	//make sure both are the full path
	CPath here(this->AbsolutePath());
	CPath there(path.AbsolutePath());
	
	//skips all the common path
	unsigned int i = 0;
	while(here.DImplementation->places[i] == there.DImplementation->places[i]){
		i++;
	}
	
	//remaining parts of startingpath need to be climbed up
	for(unsigned int j = i; j < here.DImplementation->places.size(); j++){
		ret.DImplementation->places.push_back("..");
	}
	//climb down the goal path
	for(unsigned int k = i; k < there.DImplementation->places.size(); k++){
		ret.DImplementation->places.push_back(there.DImplementation->places[k]);
	}
	
	return ret;
}

// Helper function provided to getbbtgh the current working directory
std::string CPath::GetCWD(){
    std::vector< char > CurrentPathName;

    CurrentPathName.resize(1024);
    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CurrentPathName.data();
}

// Helper funciton provided to get the home directory
std::string CPath::GetHome(){
    return std::getenv("HOME");
}

CPath CPath::AbsolutePath(const CPath &path){
    // Your code goes here
	if(path.IsAbsolute() || path.DImplementation->places[0] == ""){
		return path;
	}

	CPath ret(GetCWD());
	//ret.DImplementation->places.pop_back();
	for(unsigned int i = 1; i < path.DImplementation->places.size(); i++){
		ret.DImplementation->places.push_back(path.DImplementation->places[i]);
	}
	
	return ret;
}

CPath CPath::CurrentPath(){
    // Your code goes here
	CPath ret(GetCWD());
	return ret;
}

CPath CPath::CommonPath(const CPath &path1, const CPath &path2){
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();

	unsigned int i = 0;
	while(i < path2.DImplementation->places.size() && i < path1.DImplementation->places.size()){
		if(path2.DImplementation->places[i] != path1.DImplementation->places[i]){
			break;
		}
		ret.DImplementation->places.push_back(path2.DImplementation->places[i]);
		i++;
	}
	
	return ret;
}

CPath CPath::ExpandUserPath(const CPath &path){
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();
	
	if(path.DImplementation->places[0] == "~"){
		ret = HomePath();
		for(unsigned int i = 1; i < path.DImplementation->places.size(); i++){
			ret.DImplementation->places.push_back(path.DImplementation->places[i]);
		}
	}else{
		ret = path;
	}
	
	return ret;
}

CPath CPath::HomePath(){
    // Your code goes here
	CPath ret(GetHome());
	return ret;
}

CPath CPath::NormalizePath(const CPath &path){
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();

	for(unsigned int i = 0; i < path.DImplementation->places.size(); i++){
		if (path.DImplementation->places.size() == 1) {
			ret.DImplementation->places.pop_back();
			break;
		}
		else if(path.DImplementation->places[i].compare("..") == 0){
			ret.DImplementation->places.pop_back();
		}else if(path.DImplementation->places[i].compare(".") == 0){
			//nothing
		}else{
			ret.DImplementation->places.push_back(path.DImplementation->places[i]);
		}
	}
	
	return ret;
}

CPath CPath::RelativePath(const CPath &path, const CPath &startpath){
    // Your code goes here
	CPath ret;
	ret.DImplementation->places.pop_back();
	
	CPath here(startpath.AbsolutePath());
	CPath there(path.AbsolutePath());
	
	unsigned int i = 0;
	while(here.DImplementation->places[i] == there.DImplementation->places[i]){
		i++;
	}
	
	for(unsigned int j = i; j < here.DImplementation->places.size(); j++){
		ret.DImplementation->places.push_back("..");
	}   
	for(unsigned int k = i; k < there.DImplementation->places.size(); k++){
		ret.DImplementation->places.push_back(there.DImplementation->places[k]);
	}
	
	return ret;
}

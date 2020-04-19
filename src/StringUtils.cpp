#include "StringUtils.h"
#include <algorithm> 
#include <cctype>
#include <cstdio>

#include <iostream> //debugging: "std::cout<<"@ line: "<<__LINE__<<std::endl;"

namespace StringUtils{
    
std::string Slice(const std::string &str, ssize_t start, ssize_t end){
    // Your code goes here	
	ssize_t strlen = str.length(); 

	//checks and adjusts for negative numbers used as arguments	
	if(start < 0){
		start += strlen; 
	}
	if(end <= 0){	//end defaults to 0 if nothing is input
		end += strlen;
	}

	std::string tempstr;

	if(start < strlen){	//checks if the start is after the end
		tempstr = str.substr(start, end - start);	
	}

	return tempstr;
}
std::string Capitalize(const std::string &str){
    // Your code goes here
	size_t strlen = str.length(); 

	std::string tempstr;

	for(size_t i = 0; i < strlen; i++){
		tempstr += tolower(str[i]);	
	}

	tempstr[0] = toupper(tempstr[0]);

	return tempstr;
}

std::string Title(const std::string &str){
    // Your code goes here
	size_t strlen = str.length(); 

	std::string tempstr;

	bool cap = true;

	for(size_t i = 0; i < strlen; i++){
		if(cap && isalpha(str[i])){ tempstr += toupper(str[i]);
			cap = false;
		}else if(!isalpha(str[i])){
			tempstr += str[i];	
			cap = true;
		}else{
			tempstr += tolower(str[i]);	
		}
	}

	return tempstr;
}

std::string LStrip(const std::string &str){
    // Your code goes here
	size_t strlen = str.length(); 

	std::string tempstr;

	size_t i = 0;
	while(isspace(str[i])){
		i++;
	}
	while(i < strlen){
		tempstr += str[i];
		i++;	
	}

	return tempstr;
}

std::string RStrip(const std::string &str){
    // Your code goes here
	size_t strlen = str.length(); 

	std::string tempstr;

	size_t j = strlen;
	while(isspace(str[j-1])){
		strlen--;
		j--;
	}

	for(size_t i = 0; i < strlen; i++){
		tempstr += str[i];	
	}
	
	return tempstr;
}

std::string Strip(const std::string &str){
    // Your code goes here
	std::string tempstr;

	tempstr = RStrip(str);
	tempstr = LStrip(tempstr);

	return tempstr;
}

std::string Center(const std::string &str, int width, char fill){
    // Your code goes here
	std::string tempstr;

	int right = ((width - str.length()) / 2) + str.length(); //sets it so 
//about when RJust is run, it adds about half the added space needed for the
//length to equal to width

	tempstr = RJust(str, right, fill);
	tempstr = LJust(tempstr, width, fill);
	
	return tempstr;
}

std::string LJust(const std::string &str, int width, char fill){
    // Your code goes here
	size_t strlen = str.length(); 
	
	std::string tempstr = str;

	for(ssize_t i = strlen; i < width; i++){
		tempstr += fill;
	}	
 
	return tempstr;
}		

std::string RJust(const std::string &str, int width, char fill){
    // Your code goes here
	size_t strlen = str.length(); 

	std::string tempstr;

	for(size_t i = 0; i < width - strlen; i++){
		tempstr += fill;
	}

	for(size_t i = 0; i < strlen; i++){
		tempstr += str[i];
	}
    
	return tempstr;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
    // Your code goes here
	size_t strlen = str.length();
	size_t oldlen = old.length();
	size_t replen = rep.length();

	std::string tempstr;

	size_t i = 0;
	while(i < strlen - (oldlen - 1)){	//no point in checking any sub-
//strings that are shorter than string we are looking for (-1 since we still
// need to check the last oldlen length substring)
		if(str.compare(i, oldlen, old) == 0){
			for(size_t j = 0; j < replen; j++){
				tempstr += rep[j];
			}
			i += oldlen;
		}else{
			tempstr += str[i];
			i++;
		}
	}

	return tempstr;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt){
    // Your code goes here
	std::vector< std::string > spltlst;
	spltlst.push_back("");

	//the parameter case
	if(splt.compare("") != 0){
		for(size_t j = 0; j < str.length(); j++){
			if(str.compare(j, splt.length(), splt) == 0){
				spltlst.push_back("");
			}else if(spltlst.size() > 0){
				spltlst[spltlst.size() - 1] += str[j];
			}
		}
	//the default case
	}else{
		for(size_t j = 0; j < str.length(); j++){
			if(isspace(str[j])){
				if(spltlst[spltlst.size() - 1].compare("") != 0){
					spltlst.push_back("");
				}
			}else if(spltlst.size() > 0){
				spltlst[spltlst.size() - 1] += str[j];
			}
		}
	}

	return spltlst;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect){
    // Your code goes here
	std::string tempstr;

	for(size_t m = 0; m < vect[0].length(); m++){
		tempstr += vect[0][m];
	}

	for(unsigned int i = 1; i < vect.size(); i++){
		for(size_t k = 0; k < str.length(); k++){
			tempstr += str[k];
		}
		for(size_t j = 0; j < vect[i].length(); j++){
			tempstr += vect[i][j];
		}
	}

	return tempstr;
}

std::string ExpandTabs(const std::string &str, int tabsize){
    // Your code goes here
	std::string tempstr;
	
	for(size_t i = 0; i < str.length(); i++){
		//when it finds a tab it adds spaces until it aligns with tabsize
		if(str.compare(i, 1, "\t") == 0){
			if(tabsize > 0){
				do{
					tempstr += ' ';
				}while(tempstr.length() % tabsize != 0);
			}
		}else{
			tempstr += str[i];
		}
	}

	return tempstr;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase){
    // Your code goes here
	
/*
*following is heavely inspired by the "Calculating Distance" code
*found on https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm
*posted originally there by User9669
*/

	//string lengths
	size_t rightlen = right.length();
	size_t leftlen = left.length();
	
	//the DP table, column/row for each letter in each
	//string and a 0 row and column for empty string
	size_t dist[leftlen+1][rightlen+1] = {0};
	
	//actual strings we will be using
	std::string tleft;
	std::string tright;
	
	//sets the temp strings and accounts for ignorecase 
	if(ignorecase){
		for(size_t i = 0; i < leftlen; i++){
			tleft += tolower(left[i]);	
		}
		for(size_t i = 0; i < rightlen; i++){
			tright += tolower(right[i]);	
		}
	}else{
		tleft = left;
		tright = right;
	}
	
	//the changes against an empty string would be delteing every letter
	for(size_t i = 0; i <= leftlen; i++){
		dist[i][0] = i;
	}

	//the changes from an empty would be adding every letter
	for(size_t i = 0; i <= rightlen; i++){
		dist[0][i] = i;
	}

	//construcvts the DP table for the program, 
	//each cell representing what the solution if the strings 
	//were left(chars 0 to j) compared to right(chars 0 to i)
	for(size_t i = 1; i <= rightlen; i++){
			for(size_t j = 1; j <= leftlen; j++){
					if(tleft[j-1] == tright[i-1]){
						dist[j][i] = dist[j-1][i-1];			//no changes needed
					}else{
						dist[j][i] = std::min(dist[j-1][i] + 1,	//need to delete a character
									std::min(dist[j][i-1] + 1,	//need to add a character
										dist[j-1][i-1] + 1));	//need to change chatacter
					}
			}
	}
		
/*Loop to print the DP table, for debugging purposes, but it was 
  too handy for me to want to delete it :(	
  By: LihO on https://stackoverflow.com/questions/12311149/how-to-print-2d-arrays-in-c

	for (int i = 0; i < leftlen+1; ++i)
    {
        for (int j = 0; j < rightlen+1; ++j)
        {
            std::cout << dist[i][j] << '\t';
        }
        std::cout << std::endl;
	}
 */   
 
	//solution is in the bottom right of table
	//cast it to int because that is what function wants
	return int(dist[leftlen][rightlen]);
}

}

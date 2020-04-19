#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#define PATH_GIVEN 1




//Loads in the directories and files
void DFS(const std::string& path, CFileSystemTree::CEntry& entry);

// Function to help with the cp function
bool copy_helper (std::vector<std::string> vect, CFileSystemTree::CEntry entry);

// Allow keyboard manipulation of input in real time
void reset_input_mode(void);
void set_input_mode (void);
struct termios saved_attributes;

//Struct to check if valid directory is given
struct stat dir;
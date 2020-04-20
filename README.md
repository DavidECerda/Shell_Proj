    
# ShellProject 
> #### A C++ Project
> > ###### By: David Cerda & Alexander Dunn 

### Goal 
Implement a file system that is navigable via basic linux commands. The system is a tree with a root directory

### Implementation
The main class for the file system is called CFileSystemTree and each entry is a CEntry class. The CEntry objects point to a node struct that holds the information of the entry. Each node will have a vector of shared pointers to its children and weak pointers to the parents. This ensures that if a parent is deleted all its descendents will also be deleted as the pointer count reaches 0. Main will emulated a simplified shell similiar to unix the has the commands: pwd, cd, tree, ls, mkdir, mv, cp, cat. 

### Functionality
* Main commands: 
    * ls - list directory contents
    * pwd - show present working directory
    * tree - show all files descended from current entry (as tree)
    * cd - change directory
    * mkdir - make directory
    * mv - move and/or rename a file or directory
    * cat - concatenate or print contents of a file
    * cp - copy file or directory

> Sources:
 >> CPath.cpp and StringUtils.cpp by Alex Dunn (project 1)(with some special modifications)
 
 >> ToString's HelperFunction from Toothman and Nitta in OH
 
 >> Main.cpp: DFS, exit, and while-loop skeleton from Nitta in Discussion 4/25/2019
 
 >> Compare and Sort from Nitta on Piazza
 
 >> Layout of Node struct from Nitta in OH
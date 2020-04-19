    
# Project2
ECS 34 Project 2
By: David Cerda & Alexander Dunn
Date: April 29, 2019

FileSystemTree Tests: All Pass <3
main commands: cd works well most of the time but has some odd behavior and segfaulting in some cases, mv is perfect (except if you have root as dest the src just dissapears... woops), mv works but it does't work with renaming, cp works but it doesn't copy data but the added caveat is the the dest must be the full path including what the files name will be (if the dest is just a directory it will mess up), otherwise we are all good B^) 

Sources:
  CPath.cpp and StringUtils.cpp by Alex Dunn (project 1)(with some special modifications)
  ToString's HelperFunction from Toothman and Nitta in OH
  main.cpp: DFS, exit, and while-loop skeleton from Nitta in Discussion 4/25/2019
  Compare and Sort from Nitta on Piazza
  Layout of Node struct from Nitta in OH
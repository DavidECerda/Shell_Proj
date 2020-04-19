#include "DirectoryListing.h"
#include <dirent.h>

namespace DirectoryListing{

bool GetListing(const std::string &path, std::vector< std::tuple< std::string, bool > > &entries){
    DIR *Directory = opendir(path.c_str());

    if(!Directory){
        return false;
    }
    entries.clear();
    while(auto Entry = readdir(Directory)){
        entries.push_back( std::make_tuple< std::string, bool >( Entry->d_name, Entry->d_type & DT_DIR));
    }
    closedir(Directory);
    return true;
}

};

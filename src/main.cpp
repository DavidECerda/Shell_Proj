#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include "StringUtils.h"
#include "Path.h"
#include "Main.h"
#include "UserInput.h"

///--------------------------------------------------------------
/// @brief	The main binary is called shell_proj and it acts as a file system 
/// 		that is navigable and manipulatable by the user via unix commands: 
///                 ls, cd, mv, cp, tree, cat, pwd, exit.
///
/// Assumptions:	The user calls the program from a unix shell with 
/// 				the name of the program. If there is a second parameter given 
/// 				the program assumes that is the directory they want to load in.
/// 				Otherwise it loads the present working directory. 
/// 
/// Limitations: 
///--------------------------------------------------------------

///--------------------------------------------------------------
/// @brief 
///
/// 
/// Assumptions: 
/// 
/// Limitations: 
///
/// @param
///--------------------------------------------------------------

void reset_input_mode(void){
	tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
};

void set_input_mode (void){

	struct termios tattr;
	if(!isatty (STDIN_FILENO)){
		fprintf (stderr, "Not a terminal.\n");
    	exit (EXIT_FAILURE);
	}

	tcgetattr (STDIN_FILENO, &saved_attributes);
	atexit (reset_input_mode);


	tcgetattr (STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
};


int main(int argc, char* argv[]){

	bool Done = false;
	std::string CurrentDirectory = "/";
	CFileSystemTree TheTree;
	std::string GivenDirectory(".");
	
	/*
	if (argc > PATH_GIVEN) {
		/*
		int sizeOfArray = sizeof(argv[2]);
		std::cout<< sizeOfArray;
		std::string DirectoryAsString = ConvToString(argv[2],sizeOfArray);
		std::cout<< DirectoryAsString;
		CPath GivenDir(DirectoryAsString);
		if(!GivenDir.IsAbsolute()){
			GivenDir.AbsolutePath();
		}

		std::string CurDir = std::string(GivenDir);
		std::cout<< CurDir;make
		char DirNameArray[CurDir.length()];
		
		for (unsigned int i = 0; i < sizeof(DirNameArray); i ++){
			DirNameArray[i] = CurDir[i];
		}

		if(stat(DirNameArray, &dir ) != 0){
			std::cout<< "Not a valid Directory";
			Done = true;
		}
		else{
			GivenDirectory = std::string(GivenDir);
		}
		
	}
	*/

	// From thermios library and sets the user input to be manipulated without a newline (i.e., in real time)
	set_input_mode();
	UserInput myInput;

	// Depth first search of the given directory. 
	DFS(GivenDirectory, TheTree.Root());
	CFileSystemTree::CEntry CurrentEntry= TheTree.Root();
	//std::vector<std::string> pastInputs;
	
	while (!Done) {
		std::cout<< "> ";


		std::string InputLine = myInput.ReadingInput();

		InputLine = StringUtils::Strip(InputLine);
		std::vector<std::string> InputElements = StringUtils::Split(InputLine);

		CPath PathElems;
		std::string NormPath;
		bool oneCommand = false;

		if (InputElements.size() == 1) {
			oneCommand = true;
		}
		else if (InputElements[1].compare(0, 1, "/") == 0) {
			CPath PathElems(InputElements[1]);
			NormPath = std::string(PathElems.NormalizePath());
		}
		else {
			std::string temp = CurrentEntry.FullPath() + "/" + InputElements[1];
			CPath PathElems(temp);
			NormPath = std::string(PathElems.NormalizePath());
		}

		// For mv and cp
		CPath DestElems;
		std::string NormDest;
		if (InputElements.size()>=3){
			if (InputElements[2].compare(0, 1, "/") == 0) {
			CPath DestElems(InputElements[2]);
			NormDest = std::string(DestElems.NormalizePath());
			}

			else {
			std::string temp = CurrentEntry.FullPath() + "/" + InputElements[2];
			CPath DestElems(temp);
			NormDest = std::string(DestElems.NormalizePath());
			}
		}

		std::cout <<"\n" << NormPath << " to " << NormDest << "\n";
		if (InputElements[0] == "exit") {
			Done = true;
		}

		else if (InputElements[0] == "pwd"){
			std::cout << CurrentDirectory << std::endl;
		}

		else if (InputElements[0] == "cd") {
			if (oneCommand) {
				CurrentDirectory = TheTree.Root().Name();
				CurrentEntry = TheTree.Root();
			}
			else {
				if (NormPath.compare("/") == 0) {
					CurrentDirectory = TheTree.Root().Name();
					CurrentEntry = TheTree.Root();
				}
				else if (TheTree.Find(NormPath) != TheTree.NotFound()) {
					
					std::vector<char> data;
					auto Found = TheTree.Find(NormPath);
					if (Found->GetData(data)) {
						std::cout << "Unknown directory: " + InputElements[1] << std::endl;
					}
					else {
						CurrentDirectory = NormPath;
						CurrentEntry = *TheTree.Find(NormPath); //This was using InputElements[1] before.
					}
				}
				else {
					std::cout << "Unknown directory: " + InputElements[1] << std::endl;
				}
			}
		}

		else if (InputElements[0] == "tree") {
			if (oneCommand) {
				std::cout << std::string(CurrentEntry) << std::endl;
			}
			else {
				if (NormPath.compare("/") == 0) {
					std::cout << std::string(TheTree) << std::endl;
				}
				else if (TheTree.Find(NormPath) != TheTree.NotFound()) {
					std::cout << TheTree.Find(NormPath)->ToString() << std::endl;
				}
				else {
					std::cout << "Unknown directory: " + InputElements[1] << std::endl;
				}
			}
		}
		else if (InputElements[0] == "ls") {
			if (oneCommand) {
				for (CFileSystemTree::CEntryIterator iter = CurrentEntry.begin(); iter != CurrentEntry.end(); iter++) {
					auto name = iter->Name();
					std::cout << iter->Name() << std::endl;
				}
			}
			else {
				if (NormPath == "/"){
					for (CFileSystemTree::CEntryIterator iter = TheTree.Root().begin(); iter != TheTree.Root().end(); iter++) {
						auto name = iter->Name();
						std::cout << iter->Name() << std::endl;
					}
				}
				else if (TheTree.Find(NormPath) != TheTree.NotFound()) {
					auto Found = TheTree.Find(NormPath);
					for (CFileSystemTree::CEntryIterator iter = Found->begin(); iter != Found->end(); iter++) {
						std::cout << iter->Name() << std::endl;
					}
				}
				else {
					std::cout << "Unknown directory: " + InputElements[1] << std::endl;
				}
			}
		}
		else if (InputElements[0] == "cat") {
			if (oneCommand) {
				std::cout << "Error cat: missing parameter" << std::endl;
			}
			else {
				if (TheTree.Find(NormPath) != TheTree.NotFound()) {

					auto Found = TheTree.Find(NormPath);
					std::cout<<Found->Name()<<std::endl;
					if (Found->ChildCount() != 0) {
						std::cout << "Err Cat: " + NormPath + " is not a file" << std::endl;
					}
					else {
						std::vector<char> data;
						Found->GetData(data);
						for (auto ch : data) {
							std::cout << ch;
						}
						std::cout<<std::endl;
					}
				}
				else {
					std::cout << "Unknown directory: " + InputElements[1] << std::endl;
				}
			}
		}
		else if (InputElements[0] == "mkdir") {
			if (oneCommand) {
				std::cout << "Error mkdir: missing parameter" << std::endl;
			}
			else{
				if (TheTree.Find(NormPath) == TheTree.NotFound()) {
						if(CurrentEntry.AddChild(NormPath,true)){
							continue;
						}
						else{
							std::cout << "Error mkdir: failed to make directory " + NormPath << std::endl;
						}
					}
				
				else {
					std::cout << "Error mkdir: failed to make directory " + InputElements[1] << std::endl;
				}
			}
		}
		else if (InputElements[0] == "rm") {
			if (oneCommand) {
				std::cout << "Error rm: missing parameter" << std::endl;
			}
			else {

				if (TheTree.Find(NormPath) != TheTree.NotFound()) {
					if (CurrentEntry.RemoveChild(NormPath)){
					}
					else {
						std::cout << "Error mkdir: failed to remove " + InputElements[1] << std::endl;
					}
				}
				else {
					std::cout << "Error mkdir: failed to remove " + InputElements[1] << std::endl;
				}
			}
		}
		else if (InputElements[0] == "mv"){
			if (InputElements.size()<3) {
				std::cout << "Error mv: missing parameter" << std::endl;
				continue;
			}
			auto FoundSrc = TheTree.Find(NormPath);
			auto FoundDest = TheTree.Find(NormDest);

			if (FoundSrc != TheTree.NotFound()){
				if (NormDest=="/" || FoundDest != TheTree.NotFound()) {
					std::vector<char> data;
					if (NormDest != "/" && FoundDest->GetData(data)) {
						std::cout << "Err mv " + NormDest + " is not a Directory" << std::endl;
					}
					else if (NormDest=="/"){
						TheTree.Root().SetChild(NormPath,FoundSrc);
					}
					else {
						FoundDest->SetChild(NormPath,FoundSrc);
						}
				}	
				else {
					CurrentEntry.AddChild(NormDest);
					FoundDest = TheTree.Find(NormDest);
					std::vector<char> data;
					if (FoundSrc->GetData(data)) {
						FoundDest->SetData(data);
						FoundSrc->RemoveChild(NormPath);
					}
					else {
						FoundDest->SetChild(NormPath, FoundSrc);
					}
				}
			}
			else {
				std::cout << "Unknown file or directory: " + InputElements[1] << std::endl;
			}
		}
		else if (InputElements[0] == "cp"){

			if (InputElements.size()<3) {
				std::cout << "Error cp: missing parameter" << std::endl;
				continue;
			}
			auto FoundSrc = TheTree.Find(NormPath);
			auto FoundDest = TheTree.Find(NormDest);
			std::vector<char> fdata;
			CPath NewElems(InputElements[1]);
			std::string Base = NewElems.BaseName();
			std::string NewPath = NormDest + "/" + Base;
						

			if (FoundSrc != TheTree.NotFound()){
				if(FoundSrc->ChildCount() == 0){

					if (NormDest=="/" || FoundDest != TheTree.NotFound()) {
						
						if (NormDest=="/"){
							TheTree.Root().AddChild("/"+Base);
							auto FoundCopy = TheTree.Find("/"+Base);
							std::vector<char> CoolData;
							FoundSrc->GetData(CoolData);
							FoundCopy->SetData(CoolData);	
						}
						else {
							if (FoundDest->AddChild(NewPath)) {
								auto FoundCopy = TheTree.Find(NewPath);
								std::vector<char> CoolData;
								FoundSrc->GetData(CoolData);
								FoundCopy->SetData(CoolData);
							}
							else {
								std::cout << "Error cp: failed to copy " + InputElements[1] + " to " + InputElements[2] << std::endl;
							}
							}
					}	

					else {
						auto vect = StringUtils::Split(NormDest,"/");
						if(vect[0]==""){
							vect.erase(vect.begin());
						}
						if(copy_helper(vect,TheTree.Root())){
							TheTree.Root().AddChild(NormDest);
							auto Foundme = TheTree.Find(NormDest);
							//FoundDest->AddChild(NewPath);
							//auto FoundCopy = TheTree.Find(NewPath);
							std::vector<char> CoolData;
							FoundSrc->GetData(CoolData);
							Foundme->SetData(CoolData);
						}
						else{
							std::cout<<"Error cp: failed to copy " + InputElements[1] + " to " + InputElements[2]<<std::endl;
						}
					}
					
				}

				else {
					std::cout<<"Error cp: " + InputElements[1] +  " is not a file"<<std::endl;
				}
			}
			else {
				std::cout << "Unknown file or directory: " + InputElements[1] << std::endl;
			}
		}
			
		else {
				if (InputElements[0] == "") {
					continue;
				}
				std::cout << "Unknown command: " + InputElements[0] << std::endl;
			}

	}

	
	return EXIT_SUCCESS;
}

//Depth first search function of the given directory 
void DFS(const std::string& path, CFileSystemTree::CEntry& entry) {
	std::vector<std::tuple< std::string, bool>> Entries;
	std::string PathWithSlash = path;
	if (PathWithSlash.back() != '/') {
		PathWithSlash += "/";
	}
	DirectoryListing::GetListing(path, Entries);
	for (auto& Entry : Entries) {

		if (std::get<1>(Entry)) {
			std::string DirName = std::get<0>(Entry);
			if (DirName != "." && DirName != "..") {
				entry.AddChild(DirName);
				DFS(PathWithSlash + DirName, *entry.Find(DirName));
			}
			else {
				continue;
			}
		}
		else {
			std::string FileName = std::get<0>(Entry);
			if (FileName != "." && FileName != "..") {
				std::vector<char> vdata;
				if (entry.AddChild(FileName)) {
					auto entryIter = entry.Find(FileName);
					//http://www.cplusplus.com/reference/istream/istream/read/
					std::string filePath = PathWithSlash + FileName;
					std::ifstream is (filePath,std::ifstream::binary);
					if (is){
						is.seekg (0, is.end);
						int length = is.tellg();
						is.seekg (0, is.beg);

						vdata.resize(length);
						is.read(vdata.data(), length);
						is.close();
						entry.Find(FileName)->SetData(vdata);
					}
				}
			}
		}
	}
}



///--------------------------------------------------------------
/// @brief 
///
/// 
/// Assumptions: 
/// 
/// Limitations: 
///
/// @param
///--------------------------------------------------------------

//Recursive function to check if Dest given in cp cmd is valid to add an entry
bool copy_helper (std::vector<std::string> vect, CFileSystemTree::CEntry entry){
	for (auto iter = entry.begin(); iter != entry.end(); iter++){
		if (vect[0]==iter->Name()){
			vect.erase(vect.begin());
			return copy_helper(vect,*iter)+ false;
		}
		else if (vect.size()==1){
			return true;
		}
	}
}


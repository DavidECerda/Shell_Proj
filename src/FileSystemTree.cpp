#include "FileSystemTree.h"
#include "StringUtils.h"
#include "Path.h"
#include <iostream>
#include <algorithm>

/// @brief	A struct to represent a file or directory name in the file system tree
struct Node {
	std::weak_ptr< Node > fileParent;
	std::string fileName;
	std::vector< std::shared_ptr< Node > > fileChild;
	std::vector< char > fileData;
	CFileSystemTree::CEntry fileEntry;
};

/// @brief	Compare function for nodes used for sorting the children alphabetically
/// @param	left - First node to compare
/// @param	right - Second Node to compare
bool Compare(std::shared_ptr<Node> left, std::shared_ptr<Node> right) {
	
	std::string leftTemp = left->fileName;
	std::string rightTemp = right->fileName;
	std::string leftName;
	std::string rightName;

	for (auto &ch : leftTemp){
		leftName += tolower(ch);
	}

	for (auto &ch : rightTemp){
		rightName += tolower(ch);
	}

	return leftName < rightName;
}

/// @brief	A struct the owns a shared pointer to the root node
///			Adds a layer of abstraction
struct CFileSystemTree::SImplementation {
	std::shared_ptr <Node> directoryRoot;	
};

/// @brief	A struct that owns a weak pointer to a node for each entry
struct CFileSystemTree::CEntry::SImplementation {
	std::weak_ptr <Node> cNode;
};

///	@brief	A struct that owns a shared pointer to a node for each entry and an number for iteration
struct CFileSystemTree::CEntryIterator::SImplementation {
	int iter = 0;	//which child index the iteration is on
	std::shared_ptr <Node> Iptr = nullptr;	//the parent whose children we iterate

};

///	@brief 	A struct that owns a shared pointer to a node for each entry and an number for iteration for a const/private entry
struct CFileSystemTree::CConstEntryIterator::SImplementation {
	int iter = 0;
	std::shared_ptr <Node> Iptr = nullptr;
};

/// @brief	CEntry constructor that makes a unique pointer to a SImplementation struct
CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()) {
}

/// @brief	CEntry constructor that makes a unique pointer to a SImplementation struct and inherits from another CEntry class
/// @param 	CEntry - Class that new class will be made from
CFileSystemTree::CEntry::CEntry(const CEntry& entry) : DImplementation(std::make_unique< SImplementation >()) {
	*this = entry;	//copy it
}

/// @brief destructor
CFileSystemTree::CEntry::~CEntry() {
//o-o\\ <- John Lennon
}

/// @brief	Equals perator overload function
/// @param	CEntry - entry that class will be set equal to
CFileSystemTree::CEntry& CFileSystemTree::CEntry::operator=(const CEntry& entry) {
	if(entry.DImplementation->cNode.lock() != nullptr){
		DImplementation->cNode = entry.DImplementation->cNode.lock();
	}
	return *this;
		
}

/// @brief determines if the entry is a valid
/// @return true if valid, false if not
bool CFileSystemTree::CEntry::Valid() const {
	if(auto CurrentPtr = DImplementation->cNode.lock()){
		return true;
	}
	else{
		return false;
	}
}

/// @brief Returns name of the entry
std::string CFileSystemTree::CEntry::Name() const {
	//find name, return name
	auto SptrCurrent = DImplementation->cNode.lock();
	return SptrCurrent->fileName;
}

/// @brief Returns the full path to the entry as a string
std::string CFileSystemTree::CEntry::FullPath() const {
	std::vector< std::string > temp;
	auto CurrentPtr = DImplementation->cNode.lock(); //what CEntry we are on
	//from the current it crawls up until it cannot
	//find a parent, it adds names in reverse
	do{
		std::string fileName = CurrentPtr->fileName;
		temp.insert(temp.begin(), fileName);
	}
	while (CurrentPtr = CurrentPtr->fileParent.lock());
	
	//brings it all together with some slashes
	std::string ret =  StringUtils::Join("/", temp);

	//if full path is called on root just return /
	if (temp.size()==1){
		return"/";
	}
	ret.erase(ret.begin());
	return ret;
}

/// @brief	Outline graciously given to use by the TA during Office Hours.
///			Returns the FileSystemTree as a string and represented as a tree 
/// @param	curr - Node the the recursion is currently on
/// @param	ancestor - bool vector that keeps track of the end of a nodes children
std::string HelperForStr(std::shared_ptr<Node> curr, std::vector<bool> ancestor){
	std::string ret;	//the returning string
	if (!ancestor.empty()){
		for(unsigned int i =0; i < ancestor.size()-1; i++){
			ret += ancestor[i] ? "|  " : "   ";
		}
		ret += ancestor.back() ? "|--" : "`--";
	}
	
	ret+= curr->fileName + "\n";
	for (auto child:curr->fileChild){
		if (child != curr->fileChild.back()){
			ancestor.push_back(true);
		}
		else {
			ancestor.push_back(false);
		}
		ret += HelperForStr(child,ancestor);
		ancestor.pop_back();
	}
	return ret;
	
		
/// @brief Uses HelperForStr to convert the CEntry to a tree in string form
}
std::string CFileSystemTree::CEntry::ToString() const{
	std::vector< bool > ancestor;
	std::string ret = HelperForStr(DImplementation->cNode.lock(),ancestor);
	ret.pop_back();
	return ret;
}
 
 /// @brief CFileSystemTree::CEntry::ToString()
CFileSystemTree::CEntry::operator std::string() const{
	return this->ToString();
}

/// @brief Renames a CEntry and will resort the entry and its sibling
/// @param name - new name for the entry
bool CFileSystemTree::CEntry::Rename(const std::string & name) {

	auto SptrCurrent = DImplementation->cNode.lock();
	auto ParentEntry = SptrCurrent->fileParent.lock();
	bool changeName = true;

	for (CEntryIterator TheIter = ParentEntry->fileEntry.begin(); TheIter != ParentEntry->fileEntry.end(); TheIter++){
		if (TheIter->Name() == name) {
			changeName = false;
			break;
		}
	}

	if (changeName) {
		SptrCurrent->fileName = name;
		std::sort(ParentEntry->fileChild.begin(),
			ParentEntry ->fileChild.end(),Compare);
	}

	return changeName;
}

/// @brief Returns the number of children of an entry
size_t CFileSystemTree::CEntry::ChildCount() const {
	if(Valid()){
		auto SptrCurrent = DImplementation->cNode.lock();
		//size_t ret = SptrCurrent->fileChild.size();
		size_t ret = 0;

		for (CEntryIterator Citer = SptrCurrent->fileEntry.begin(); Citer != SptrCurrent->fileEntry.end(); Citer++) {
			ret++;
		}
		return ret;
	}
	else{
		return 0;
	}
}

///-------------------------------------------------------------------
/// @brief Sets a existing entry to be the child of calling entry
/// @param name - name of the entry to set be as child of calling entry
/// @param iter - CEntryIterator that points to the entry to set as a child
/// @returns - true if the child was set successfully, 
///			   false if iter was not pointing to a valid entry 
///			   and/or a child already exists with given name
///-------------------------------------------------------------------
bool CFileSystemTree::CEntry::SetChild(const std::string & name, CEntryIterator & iter) {
	auto SptrCurrent = DImplementation->cNode.lock();
	//checks if name already exists, if it does return false
	for (CEntryIterator Citer = SptrCurrent->fileEntry.begin(); Citer != SptrCurrent->fileEntry.end(); Citer++) {
		if (Citer->Name() == name) {
			return false;
		}
	}
	/*
	If iter is to a nonvalid CEntry, returns false
	*/
	if (iter.DImplementation->Iptr == nullptr) {
		return false;
	}
	//If Centry iter is to valid Centry moves Centry to be child of current node, returns true
	else {
		auto Moveptr = iter->DImplementation->cNode.lock();
		SptrCurrent->fileChild.push_back(Moveptr);
		auto ParentPtr = Moveptr->fileParent.lock();
		Moveptr->fileParent = SptrCurrent;
		ParentPtr->fileChild[iter.DImplementation->iter].reset();
		ParentPtr->fileChild.erase(ParentPtr->fileChild.begin()+iter.DImplementation->iter);	
		std::sort(SptrCurrent->fileChild.begin(), SptrCurrent->fileChild.end(), Compare);
		return true;
	}
	
}

///-------------------------------------------------------------------
/// @brief	Adds a 
bool CFileSystemTree::CEntry::AddChild(const std::string & path, bool addall) {
	auto HoldPtr = DImplementation->cNode.lock();
	std::string full;
	std::shared_ptr<Node> CurrentPtr = DImplementation->cNode.lock();
	while (HoldPtr = HoldPtr->fileParent.lock()){
		CurrentPtr = HoldPtr;
	}
	if (path.compare(0,1,"/")!=0){
		full = (this->FullPath())+"/";
		full+=path;
		full.erase(full.begin());
	}
	else{
		full = path;
	}
	// if (full.compare(0,1,"/")!=0){
	// 	full.insert(0, "/");
	// }
	CPath ThyPath(full);
	std::string temp = std::string(ThyPath.NormalizePath());
	if (temp.compare(0,1,"/")!=0){
		return false;
	}
	auto Children = StringUtils::Split(temp, "/");
	if (Children[0] == "") {
		Children.erase(Children.begin());
	}
	//std::shared_ptr<Node> CurrentPtr = DImplementation->cNode.lock();
	
	for (unsigned int i = 0; i < Children.size(); i++) {
		bool newName = true;
		for (CEntryIterator iter = CurrentPtr->fileEntry.begin(); iter != CurrentPtr->fileEntry.end(); ++iter) {
			if (iter->Name() == Children[i]) {
				newName = false;

				if (i == Children.size() - 1) {
					return false;
				}
				
				else{
					CurrentPtr = CurrentPtr->fileChild[iter.DImplementation->iter];
					break;
				}
			}
		}
		if (newName) {
			if (!CurrentPtr->fileData.empty()){
				return false;
			}
			auto addedName = std::make_shared<Node>();
			addedName->fileName = Children[i];
			addedName->fileEntry.DImplementation->cNode = addedName;
			addedName->fileParent = CurrentPtr;
			CurrentPtr->fileChild.push_back(addedName);
			std::sort(CurrentPtr->fileChild.begin(), CurrentPtr->fileChild.end(), Compare);

			if (addall) {
				CurrentPtr = addedName;
			}
			else {
				break;
			}
		}
	}
	return true;
}

bool CFileSystemTree::CEntry::RemoveChild(const std::string& path) {
	auto HoldPtr = DImplementation->cNode.lock();
	std::string full;
	std::shared_ptr<Node> CurrentPtr = DImplementation->cNode.lock();
	while (HoldPtr = HoldPtr->fileParent.lock()){
		CurrentPtr = HoldPtr;
	}
	if (path.compare(0,1,"/")!=0){
		full = (this->FullPath())+"/";
		full+=path;
		full.erase(full.begin());
	}
	else{
		full = path;
	}
	if (full.compare(0,1,"/")!=0){
		full.insert(0, "/");
	}
	CPath ThyPath(full);
	std::string temp = std::string(ThyPath.NormalizePath());
	if (temp.compare(0,1,"/")!=0){
		return false;
	}
	auto Children = StringUtils::Split(temp, "/");
	if (Children[0] == "") {
		Children.erase(Children.begin());
	}
	//std::shared_ptr<Node> CurrentPtr = DImplementation->cNode.lock();
	
	for (unsigned int i = 0; i < Children.size(); i++) {
		for (CEntryIterator iter = CurrentPtr->fileEntry.begin(); iter != CurrentPtr->fileEntry.end(); ++iter) {
			if (iter->Name() == Children[i]) {
				if (i != Children.size() - 1) {
				CurrentPtr = CurrentPtr->fileChild[iter.DImplementation->iter];
				break;
				}
				else {
					CurrentPtr->fileChild[iter.DImplementation->iter].reset();
					CurrentPtr->fileChild.erase(CurrentPtr->fileChild.begin()+iter.DImplementation->iter);	
					return true;
				}
			}
		}
	}
	return false;
}




	/*
	auto CurrentPtr=DImplementation->cNode.lock();
	for (CEntryIterator Theiter = CurrentPtr->fileEntry.begin(); Theiter != CurrentPtr->fileEntry.end(); Theiter++) {
		if (Theiter->Name() == path) {
			
			return true;
		}
	}
	*/

bool CFileSystemTree::CEntry::SetData(const std::vector< char > & data) {
	//need to check on this
	auto SptrCurrent = DImplementation->cNode.lock();
	if (ChildCount() == 0) {
		SptrCurrent->fileData = data;
		return true;
	}
	else {
		return false;
	}
}

bool CFileSystemTree::CEntry::GetData(std::vector< char > & data) const {
	if(this->Valid()){
		auto SptrCurrent = DImplementation->cNode.lock();

		if (ChildCount() != 0) {
			return false;
		}
		else {
			data = SptrCurrent->fileData;
			return true;
		}
	}

	return false;
}

CFileSystemTree::CEntry& CFileSystemTree::CEntry::Parent() {
	auto SptrCurrent = DImplementation->cNode.lock();
	if(auto parentPtr = SptrCurrent->fileParent.lock()){
		return  parentPtr->fileEntry;
	}
	else{
		auto Sptr = std::make_unique<Node>();
		return Sptr->fileEntry;
	}
}

const CFileSystemTree::CEntry& CFileSystemTree::CEntry::Parent() const {
	auto SptrCurrent = DImplementation->cNode.lock();
	if(auto parentPtr = SptrCurrent->fileParent.lock()){
		return  parentPtr->fileEntry;
	}
	else{
		auto Sptr = std::make_unique<Node>();
		return Sptr->fileEntry;
	}
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string & name) {
	auto CurrentPtr = this->DImplementation->cNode.lock();
	CEntryIterator ret;
	for (CEntryIterator iter = CurrentPtr->fileEntry.begin(); iter != CurrentPtr->fileEntry.end(); iter++) {
		if (iter->Name() == name) {
			ret = iter;
			break;
		}
	}
	return ret;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string & name) const {
	auto CurrentPtr = this->DImplementation->cNode.lock();
	CEntryIterator ret;
	
	for (CEntryIterator iter = CurrentPtr->fileEntry.begin(); iter != CurrentPtr->fileEntry.end(); iter++) {
		if (iter->Name() == name) {
			ret = iter;
			break;
		}
	}
	return ret;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin() {
	CEntryIterator BeginPos;
	auto Sptr = DImplementation->cNode.lock();
	BeginPos.DImplementation->Iptr=Sptr;
	BeginPos.DImplementation->iter = 0;
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return BeginPos;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const {
	//std::cout << "@ line: " << __LINE__ << std::endl;
	CConstEntryIterator BeginPos;
	auto Sptr = DImplementation->cNode.lock();
	BeginPos.DImplementation->Iptr=Sptr;
	BeginPos.DImplementation->iter = 0;
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return BeginPos;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const {
	const CConstEntryIterator BeginPos;
	auto Sptr = DImplementation->cNode.lock();
	BeginPos.DImplementation->Iptr=Sptr;
	BeginPos.DImplementation->iter = 0;
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return BeginPos;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end() {
	CEntryIterator PastLastPos;
	auto Sptr = DImplementation->cNode.lock();
	PastLastPos.DImplementation->Iptr=Sptr;
	PastLastPos.DImplementation->iter = Sptr->fileChild.size();
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return PastLastPos;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const {
	CConstEntryIterator PastLastPos;
	auto Sptr = DImplementation->cNode.lock();
	PastLastPos.DImplementation->Iptr=Sptr;
	PastLastPos.DImplementation->iter = Sptr->fileChild.size();
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return PastLastPos;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const {
	const CConstEntryIterator PastLastPos;
	auto Sptr = DImplementation->cNode.lock();
	PastLastPos.DImplementation->Iptr=Sptr;
	PastLastPos.DImplementation->iter = Sptr->fileChild.size();
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return PastLastPos;
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique< SImplementation >()) {
	//std::cout << "@ line: " << __LINE__ << std::endl;
}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator & iter) : DImplementation(std::make_unique< SImplementation >()) {
	*this = iter;
	//std::cout << "@ line: " << __LINE__ << std::endl;
}

CFileSystemTree::CEntryIterator::~CEntryIterator() {
	// You code here
	//std::cout << "@ line: " << __LINE__ << std::endl;
}


CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator=(const CEntryIterator & iter) {
	DImplementation->iter = iter.DImplementation->iter;
	DImplementation->Iptr = iter.DImplementation->Iptr;
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator & iter) const {
	//std::cout << "@ line: " << __LINE__ << std::endl;
	if (DImplementation->Iptr == iter.DImplementation->Iptr 
	&& DImplementation->iter == iter.DImplementation->iter) {
		return true;
	}
	else {
		return false;
	}
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator & iter) const {
	//std::cout << "@ line: " << __LINE__ << std::endl;
	return (!operator==(iter));
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++() {
	DImplementation->iter++;
	return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int) {
	CEntryIterator temp = *this;
	++*this;
	return temp;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--() {
	DImplementation->iter--;
	return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int) {
	CEntryIterator temp = *this;
	--*this;
	return temp;
}

CFileSystemTree::CEntry& CFileSystemTree::CEntryIterator::operator*() const {
	return DImplementation->Iptr->fileChild[DImplementation->iter]->fileEntry;
}

CFileSystemTree::CEntry* CFileSystemTree::CEntryIterator::operator->() const {
	CEntry* temp = &(DImplementation->Iptr->fileChild[DImplementation->iter]->fileEntry);
	return temp;
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique< SImplementation >()) {
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator & iter) : DImplementation(std::make_unique< SImplementation >()) {
	*this = iter;
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator & iter) : DImplementation(std::make_unique< SImplementation >()) {
	*this = iter;
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator() {
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator & iter) {
	DImplementation->iter = iter.DImplementation->iter;
	DImplementation->Iptr = iter.DImplementation->Iptr;
	return *this;
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator & iter) const {
	if ((this == &iter) || (DImplementation->Iptr == iter.DImplementation->Iptr 
	&& DImplementation->iter == iter.DImplementation->iter)) {
		return true;
	}
	else {
		return false;
	}
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator & iter) const {
	return (!operator==(iter));
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++() {
	DImplementation->iter++;
	return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int) {
	CConstEntryIterator temp = *this;
	++* this;
	return temp;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--() {
	DImplementation->iter--;
	return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int) {
	CConstEntryIterator temp = *this;
	--* this;
	return temp;
}

const CFileSystemTree::CEntry& CFileSystemTree::CConstEntryIterator::operator*() const {
	CEntry& temp = DImplementation->Iptr->fileChild[DImplementation->iter]->fileEntry;
	return temp;
}

const CFileSystemTree::CEntry* CFileSystemTree::CConstEntryIterator::operator->() const {
	CEntry* temp = &(DImplementation->Iptr->fileChild[DImplementation->iter]->fileEntry);
	return temp;
}

CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()) {
	
	auto rootptr = std::make_shared< Node >();
	rootptr->fileName = "/";
	rootptr->fileEntry.DImplementation->cNode = rootptr;
	this->DImplementation->directoryRoot = rootptr;
}

CFileSystemTree::CFileSystemTree(const CFileSystemTree & tree) : DImplementation(std::make_unique< SImplementation >()) {
	*this = tree;
}

CFileSystemTree::~CFileSystemTree() {
}

CFileSystemTree& CFileSystemTree::operator=(const CFileSystemTree& tree) {
	if ((this != &tree) && (DImplementation->directoryRoot != tree.DImplementation->directoryRoot)) {
		*this = tree;
	}
	return *this;
}

CFileSystemTree::CEntry& CFileSystemTree::Root() {
//	CEntry& Root = (DImplementation->directoryRoot->fileEntry);
//	return Root;
	return DImplementation->directoryRoot->fileEntry;
}

const CFileSystemTree::CEntry& CFileSystemTree::Root() const {
	CEntry& Root = (DImplementation->directoryRoot->fileEntry);
	return Root;
}

std::string CFileSystemTree::ToString() const {
	return this->DImplementation->directoryRoot->fileEntry.ToString();
}

CFileSystemTree::operator std::string() const {
	return this->DImplementation->directoryRoot->fileEntry.ToString();
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string & path) {
	std::string temporal = path;
	if (temporal.compare(0,1,"/")==0){
		temporal.erase(temporal.begin());
	}
	auto trek = StringUtils::Split(temporal, "/");
	auto CurrentPtr = this->DImplementation->directoryRoot;
	CEntryIterator temp;
	for(unsigned int i = 0; i < trek.size(); i++){
		temp = CurrentPtr->fileEntry.Find(trek[i]);
		if(temp == NotFound()){
			break;
		}
		CurrentPtr = temp->DImplementation->cNode.lock();
	}
	return temp;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string & path) const {
	std::string temporal = path;
	if (temporal.compare(0,1,"/")==0){
		temporal.erase(temporal.begin());
	}
	auto trek = StringUtils::Split(temporal, "/");
	auto CurrentPtr = this->DImplementation->directoryRoot;
	CConstEntryIterator temp;
	for(unsigned int i = 0; i < trek.size(); i++){
		temp = CurrentPtr->fileEntry.Find(trek[i]);
		if(temp == NotFound()){
			break;
		}
		CurrentPtr = temp->DImplementation->cNode.lock();
	}
	return temp;
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound() {
	CEntryIterator ret;
	return ret;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const {
	CConstEntryIterator ret;
	return ret;
}

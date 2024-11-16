#include<iostream>
#include<string.h>
#include<fstream>
#include<map>
#include<queue>

// Encoding steps
// -> read file and store frequencies *
// -> create binary lookup tree using those frequencies *
// -> use tree to generate prefix code table
// -> write the header of the file
// 		-> define the starting and the ending of the header 
// 		-> write out the tree as the header or the frequency table that can be used to recreate the tree
// -> use the table to encode the file content into the output file (use bit strings and pack them into bytes)

//Custom Comparator

class BaseNode{
public:
	virtual bool isleaf() = 0;
	virtual int weight() = 0;
};	

class Compare{
public:
	bool operator()(BaseNode* a, BaseNode* b){
		if(a->weight() > b->weight()){
			return true;
		}
		return false;
	}
};

class LeafNode : public BaseNode{
private:
	char element;
	int w;
public:
	LeafNode(char el, int w){
		element = el;
		this->w = w;
	}

	//getter functions
	char value()
	{return element;}
	int weight() override
	{return w;}

	bool isleaf() override
	{return true;}
};

class InternalNode : public BaseNode{
private:
	int w;
	BaseNode* lft;
	BaseNode* rght;
public:
	InternalNode(BaseNode* l,BaseNode* r, int w){
		lft = l;
		rght = r;
		this->w = w;
	}

	//getter functions
	int weight() override
	{return w;}
	BaseNode* left(){return lft;}
	BaseNode* right(){return rght;}

	bool isleaf() override
	{return false;}

};


//Function to build the prefix table
void buildPrefTable(BaseNode* root,std::string pref,std::map<char,std::string>& table){
	if(root->isleaf()){
		table[((LeafNode*)root)->value()] = pref;
		return;
	}

	buildPrefTable(((InternalNode*)root)->left(),pref+"0",table);
	buildPrefTable(((InternalNode*)root)->right(),pref+"1",table);

}

int main(int argc , char* argv[]){

//Opening the file and reading it

	std::cout<<argv[1]<<std::endl;
	std::string file_name = argv[1];
	std::string file_type = file_name.substr(file_name.size()-4);
	std::cout<<file_type<<std::endl;
	if(file_type != ".txt"){
		std::cerr<<"Invalid file type\n";
		return 1;
	}

	std::ifstream file(file_name);

	std::string file_content = "";

	if(file.is_open()){
		std::string line;
		while ( file ) {
			std::getline(file,line);
			// std::cout<<line<<"\n";
			file_content += line+"\n";
		}
	}
	else{
		std::cerr<<"No such file\n";
		return 1;
	}
	int file_size = file_content.size();
	std::cout<<"\n"<<"Size of File is: "<<file_size<<"\n";

//Calculating the frequencies of characters in file

	std::map<char,int> mp;

	for(int i = 0;i<file_size;i++){
		mp[file_content[i]]++;
	}	

//testing
	// for(auto it:mp){
	// 	std::cout<<it.first<<" : "<<it.second<<"\n";
	// }

//Building the lookup binary tree using the frequencies

	std::priority_queue<BaseNode*,std::vector<BaseNode*>,Compare> cont;			//Have to make a custom comparator

	for(auto it:mp){
		char el = it.first;
		int freq = it.second;
		LeafNode* l = new LeafNode(el,freq);
		cont.push(l);
	}

	BaseNode* root = NULL;

	while(cont.size()>=2){
		BaseNode* a = cont.top();
		cont.pop();
		BaseNode* b = cont.top();
		cont.pop();
		int nweight = (a->weight())+(b->weight());
		InternalNode* nd = new InternalNode(a,b,nweight);
		root = nd;
		cont.push(nd);
	}


	//Making the prefix table
	std::map<char,std::string> prefix;

	buildPrefTable(root,"",prefix);

	for(auto it:prefix){
		std::cout<<it.first<<" -> "<<mp[it.first]<<" -> "<<it.second<<std::endl;
	}

	return 0;
}
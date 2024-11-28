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

void writeHeader(std::ofstream &outFile, const std::string &header) {
    size_t length = header.size();
    outFile.write(reinterpret_cast<const char *>(&length), sizeof(length)); // Write header length
    outFile.write(header.data(), length);                                   // Write header data
}

void writeBits(std::ofstream &outFile, unsigned char &buffer, int &bitCount, const std::string &bits) {
    for (char bit : bits) {
        buffer = (buffer << 1) | (bit - '0'); // Add bit to buffer
        bitCount++;

        if (bitCount == 8) {
            outFile.put(buffer); // Write full byte to file
            buffer = 0;          // Reset buffer
            bitCount = 0;
        }
    }
}

void flushBuffer(std::ofstream &outFile, unsigned char &buffer, int bitCount) {
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount); // Pad remaining bits with zeros
        outFile.put(buffer);
    }
}

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

	//building the header of the compressed file
	
	// steps to build the header
	// -> To build the header we have to encode the freq mapping in a single string
	// 		-> store each element of the map as "(size of string)#key:value" e.g. "4#c:34"
	//		-> concatenate all the strings
	//-> set the start of the header as (size of the concatenated string)#the string
	//		->this will define the start and ending of the header

	std::string file_header = "";

	for(auto it:mp){
		std::string key(1, it.first);
		std::string value = std::to_string(it.second);

		std::string pair = key + ":" + value;

		file_header += std::to_string(pair.size()) + "#" + pair;
	}

	file_header = std::to_string(file_header.size()) + "#" + file_header;

	std::cout<<"Header of the file is as follows:\n";
	std::cout<<file_header<<"\n";

// Finally Encoding the file and appending it to the header in the output file
	// -> Create the Outfile which will be a binary file
	// -> write the header data into the output file
	// -> append the encoded data to the output file

	// Creating the output file
	std::ofstream outfile("encoded.bin", std::ios::binary);

	// Check if the file is successfully opened
    if (!outfile) {
        std::cerr << "Error: Could not create or open the file!" << std::endl;
        return 1;
    }

	//writing the header
	writeHeader(outfile, file_header);

	// Writing encoded Data
	unsigned char buffer = 0;
	int bitcount = 0;

	for(char ch: file_content){
		std::string bits = prefix[ch];
		writeBits(outfile, buffer, bitcount, bits);
	}

	flushBuffer(outfile, buffer, bitcount);

	std::streampos enc_file_size = outfile.tellp();
	std::cout<<"File successfully encoded!\nSize of old file is: "<<file_size<<"\nSize of New file is: "<<enc_file_size<<"\n";

	outfile.close();

	return 0;
}
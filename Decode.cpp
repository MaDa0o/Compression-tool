#include<iostream>
#include<string.h>
#include<fstream>
#include<map>
#include<queue>

#include "huff_tree.h"
#include "PrefTable.h"

// Decoding Steps
// 	-> read encoded file it should be in .bin format
// 	-> extract the header
// 	-> decode the header and build back the frequency table from it
// 	-> use the frequency table to build the tree again
// 	-> Read the rest of the encoded file
// 	-> use the tree to decode the encoded strings
// 	-> put the decoded characters in the decoded file
// 	-> Decoding done

int main(int argc, char* argv[]){

	std::string file_name = argv[1];
	std::string file_type = file_name.substr(file_name.size()-4);
	if(file_type != ".bin"){
		std::cerr<<"Invalid file type\n";
		return 1;
	}

	//Reading the file
	std::ifstream file(file_name, std::ios::binary);

	std::string file_content = "";
	char byte;

	if(file.is_open()){
		while (file.read(&byte, 1)) {
        file_content += byte; // Store each byte as a character
    	}
	}
	else{
		std::cerr<<"No such file\n";
		return 1;
	}
	int file_size = file_content.size();
	std::cout<<"\n"<<"Size of File is: "<<file_size<<"\n";
	// std::cout<<file_content;

	// Extracting the Header

	size_t header_size = stoi(file_content.substr(0,file_content.find('#')));
	std::cout<<"\nsize of Header is "<<header_size<<"\n";

	std::string header_content = file_content.substr(file_content.find('#')+1,header_size);
	// std::cout<<"Header Content is: \n"<<header_content<<"\n";

	file_content = file_content.substr(file_content.find('#')+1);
	file_content = file_content.substr(header_size);

	// std::cout<<"Encoded file content:\n"<<file_content<<"\n";

	std::map<char,int> freq;

	while(!header_content.empty()){
		char key;
		int value=0;

		size_t pair_size = stoi(header_content.substr(0,header_content.find("#")));
		header_content = header_content.substr(header_content.find('#')+1);

		std::string pair = header_content.substr(0,pair_size);

		header_content = header_content.substr(pair_size);

		key = pair[0];
		value = stoi(pair.substr(2));
		// std::cout<<key<<" -> "<<value<<"\n";


		freq[key] = value;
	}

	std::cout<<"Size of freq map is: "<<freq.size()<<"\n";

	//build the huffman tree from the frequency map
	
	BaseNode* root = BuildTree(freq);

	std::cout<<"Tree rebuilt!!\n";

	return 0;
}
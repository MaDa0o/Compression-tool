#include<iostream>
#include<string.h>
#include<fstream>
#include<map>
#include<queue>

#include "huff_tree.h"
#include "encode_utility.h"
#include "PrefTable.h"

// Encoding steps
// -> read file and store frequencies *
// -> create binary lookup tree using those frequencies *
// -> use tree to generate prefix code table
// -> write the header of the file
// 		-> define the starting and the ending of the header 
// 		-> write out the tree as the header or the frequency table that can be used to recreate the tree
// -> use the table to encode the file content into the output file (use bit strings and pack them into bytes)


int main(int argc , char* argv[]){

//Opening the file and reading it

	std::string full_file_name = argv[1];
	std::string file_type = full_file_name.substr(full_file_name.size()-4);
	std::string file_name = full_file_name.substr(0,full_file_name.size()-4);
	if(file_type != ".txt"){
		std::cerr<<"Invalid file type\n";
		return 1;
	}

	std::ifstream file(full_file_name);

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
	std::cout<<"Size of freq map is: "<<mp.size()<<"\n";

	// for(auto it: mp){
	// 	std::cout<<it.first<<" -> "<<it.second<<"\n";
	// }

//Building the lookup binary tree using the frequencies

	BaseNode* root = BuildTree(mp);


	//Making the prefix table
	std::map<char,std::string> prefix;

	buildPrefTable(root,"",prefix);

	//building the header of the compressed file

	std::string file_header = buildHeader(mp);

// Finally Encoding the file and appending it to the header in the output file
	// -> Create the Outfile which will be a binary file
	// -> write the header data into the output file
	// -> append the encoded data to the output file

	// Creating the output file



	std::ofstream outfile(file_name + "_encoded.bin", std::ios::binary);

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
	std::cout<<"File successfully encoded!\nSize of original file is: "<<file_size<<"\nSize of compressed file is: "<<enc_file_size<<"\n";

	outfile.close();

	return 0;
}
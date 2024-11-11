#include<iostream>
#include<string.h>
#include<fstream>
#include<map>

int main(int argc , char* argv[]){
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
	// std::cout<<file_content;	
	int file_size = file_content.size();
	std::cout<<"\n"<<"Size of File is: "<<file_size<<"\n";

	std::map<char,int> mp;

	for(int i = 0;i<file_size;i++){
		mp[file_content[i]]++;
	}	

	for(auto it:mp){
		std::cout<<it.first<<" : "<<it.second<<"\n";
	}

	return 0;
}
void writeHeader(std::ofstream &outFile, const std::string &header) {
    size_t length = header.size();
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


std::string buildHeader(std::map<char,int>& mp){
	
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

	return file_header;
}
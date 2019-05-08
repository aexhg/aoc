#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <unordered_map>

static const std::unordered_map<char, std::string> charMap{
	{ '\\', "\\\\"},
	{'"', "\\\""},
};


int encodedStringCount(const std::string&s, bool print ){
	std::string ns = "\"";
	for(const char& ss : s){
		if(charMap.count(ss)){
			ns += charMap.at(ss);
		}
		else{
			ns += ss;
		}
	}
	ns += "\"";
	if(print){
		std::cout << "encodedStringCount: " << ns << std::endl;
	}
	return ns.size();
}

int main(int argv, char* argc[]){

	std::string filename = argc[1];
	int debug = 0;
	if(argv  >= 3)
		debug = std::stoi(argc[2]);

	std::ifstream is(filename);
	std::string line;
	static const std::regex regex = std::regex(R"([a-z]|\\x\w{2}|\\"|\\\\)", std::regex_constants::ECMAScript);
//	static const std::regex regex = std::regex(R"([a-z]+)");
	std::smatch match;
	int charCount = 0;
	int totalStringLen = 0;
	int totalEncodedSize = 0;

	while(std::getline(is, line)){
		int count = 0;
		int sizeCount = 0;
		int encCount = encodedStringCount(line, debug > 2);
		while(std::regex_search(line, match, regex)){
			if(debug > 2){
				std::cout << "regex match: " << line << std::endl;
				std::cout << "regex size: " << match.size() << std::endl;
				std::cout << match.str(0) << std::endl;
			}
			count++;
			sizeCount += match.str(0).size();
			line = match.suffix();
		}
		if(debug> 1){
			std::cout << "Found: " << count << " matches" << std::endl;
			std::cout << "Total size of string: " << sizeCount + 2<< std::endl;
			std::cout << "Encoded size: " << encCount << std::endl;
		}
		charCount += count;
		totalStringLen += (sizeCount + 2);
		totalEncodedSize += encCount;
	}
	std::cout << "charCount: " << charCount << std::endl;
	std::cout << "totalCount: " << totalStringLen << std::endl;
	std::cout << "Diff: " << totalStringLen - charCount << std::endl;
	std::cout << "Diff encoded: " << totalEncodedSize - totalStringLen << std::endl;
	return 0;
}

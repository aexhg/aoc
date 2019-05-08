#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const std::vector<std::string> vwl = {
		"a",
		"e",
		"i",
		"o",
		"u"
	};
const std::string vwls = "aeiou";
const std::vector<std::string> bad = {
		"ab",
		"cd",
		"pq",
		"xy"
	};


bool checkVowels(const std::string& s){

	int count = 0;
	for(auto &ss : s){
		if(vwls.find(ss)!=std::string::npos){
			count++;
		}
		if(count >= 3){
			return true;
		}
	}
	//for(auto &ss : vwl){
	//	if(s.find(ss) != std::string::npos){
	//		count++;
	//	}
	//	if(count >= 3){
	//		return true;
	//	}
	//}
	return false;
}

bool checkNotAllowed(const std::string& s){
	
	for(auto &ss: bad ){
		if(s.find(ss) != std::string::npos){
			return false;
		}
	}
	return true;
}

bool checkDoubles(const std::string& s){

	char c = *s.begin();
	for(int i = 1; i < s.size(); ++i){
		if(c == s[i]){
			return true;
		}
		c = s[i];
	}
	return false;
}

bool testPairs(const std::string& s){

	for(int i = 0; i < s.size()-2; ++i){
		const std::string sub = s.substr(i,2);
		if(s.find(sub,i+2) != std::string::npos){
			return true;
		}
	}
	return false;
}

bool testRepeat(const std::string& s){

	for(int i = 0; i < s.size()-2; ++i){
		const char &c = s[i];
		if(c == s[i+2]){
			return true;
		}
	}
	return false;
}

int main(int argv, char* argc[]){

	//const std::string test = "qjhvhtzxzqqjkmpb";
	//std::cout << "test double: " << checkDoubles(test)  << std::endl;
	//std::cout << "test vowels: " << checkVowels(test) << std::endl;
	//std::cout << "test not allowed: " << checkNotAllowed(test) << std::endl;
	//std::cout << "test pairs: " << testPairs(test) << std::endl;
	//std::cout << "test repeat: " << testRepeat(test) << std::endl;
		

	std::string line;
	std::ifstream is(argc[1]);

	int n = 0;
	while(is >> line){
		if(argv == 3){
			std::cout << "line : " << line << std::endl;
			std::cout << "test pairs: " << testPairs(line) << std::endl;
			std::cout << "test repeat: " << testRepeat(line) << std::endl;
			std::cout << "count inc: " << (testRepeat(line) && testPairs(line)) << std::endl; 
		}

		n += testPairs(line) && testRepeat(line);

	}

	std::cout << "There are " << n << " string satisfied" << std::endl;
	return 0;
}

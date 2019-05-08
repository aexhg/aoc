#include <iostream>
#include <string>
#include <sstream>


int main(int argv, char* argc[]){

	int ntimes = std::stoi(argc[1]);
	std::string s(argc[2]);

	bool debug = argv > 3;

	for(int i = 0; i < ntimes; ++i){
		int pos = 0; 
		int starting = 0;
		std::string ss;
		if(debug){
			std::cout << "So far:" << s << std::endl;
		}
		while(pos != s.size()){
			char c = s[pos];
			while(pos != s.size()){
				if(s[pos] != c){
					break;
				}
				++pos;
			}
			int count = pos - starting;
			starting = pos;
			ss += std::to_string(count) + c;
		}
		s = std::move(ss);	
	}
	if(debug){
		std::cout << "Final: " << s << std::endl;
	}

	std::cout<< "string size: " << s.size() << std::endl;
	
	return 0;
}

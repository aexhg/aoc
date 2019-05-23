#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>
#include "threadpool.hpp"


long long decompress(std::string s){
	long long size = 0;
	if(s == "(12x2)NXCFFANKSYYU"){
		std::cout << s << std::endl;
	}

	for(auto f = s.find("("); f != std::string::npos; ){
		size += f;
		s = s.substr(f+1);
		int x = std::stoi(s);
		auto fx = s.find("x");
		int y = std::stoi(s.substr(fx+1));
		auto f2 = s.find(")");
		s = s.substr(f2+1);
		size += decompress(s.substr(0, x)) * y;
		s = s.substr(x);
		f = s.find("(");
	}
	size += s.size();
	return size;
}

int main(int argv, char* argc[]){


	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1]));
			std::getline(is, line);){
		long long size = 0;
		size = decompress(line);	
		std::cout << size << std::endl;
		
	}

	return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "md5.hpp"
#include "threadpool.hpp"

int main(int argv, char* argc[]){
	
	const std::string s = argc[1];

	auto nextHash = [](const std::string& s, int n){
		const std::string c = "00000";
		std::string hash = md5::md5(s+std::to_string(n));
		while(c != hash.substr(0,5)){
			n++;
			hash = md5::md5(s+std::to_string(n));
			if(n==3231929){
				std::cout << n << " " << hash << std::endl;
			}
		}
		return std::make_pair(hash, n);
	};
	
	int n = 0;
	std::string code="XXXXXXXX";
	
	for(int count = 0;count < 8; ){

		auto [hash, c] = nextHash(s, n);
		n = c+1;
		try{
			auto spos = hash.substr(5,1); 
			int pos = std::stoi(spos);
			if(pos < 8 && code[pos] == 'X'){
				char cc = hash[6];
				code[pos] = cc;
				++count;
				std::cout << code << std::endl;
			}
		}
		catch(std::invalid_argument& e){
			std::cout << "invalid position: " << hash[5] << std::endl;
		}
		//code += hash[5];	
	}
	std::cout << code << std::endl;

	return 0;
}

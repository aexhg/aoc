#include <iostream>
#include "md5.hpp"

int main(){
	//609043
	const std::string s = "bgvyzdsv"; //"abcdef";
	const std::string c = "000000";

	int n = 0;//254575;

	while(c != md5(s+std::to_string(n)).substr(0,6)){
		n++;
		if(n % 100000 == 0){
			std::cout << n << std::endl;
			std::cout << md5(s+std::to_string(n)) << std::endl;
		}
	}

	std::cout << "n: " << n << std::endl;
	std::cout << "md5: " << md5(s+std::to_string(n)) << std::endl;	
	return 0;
}

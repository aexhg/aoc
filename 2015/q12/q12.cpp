#include <iostream>
#include <regex> 
#include <string>
#include <fstream>

int main(int argv, char* argc[]){

	std::string filename = argc[1];
	//std::ifstream is(filename);

	//std::string line;
	const std::regex regex("-?\\d+");
	const std::regex red(R"((\{[^{}]*\}))");
	std::smatch match;	
	int sum = 0;
	for(auto [line, is] = std::make_pair(std::string{}, std::ifstream(filename)); std::getline(is, line); ){
		//std::cout << line << std::endl;
		while(std::regex_search(line, match, red)){
			std::string sm = match.str();
			if(int n = sm.find(R"(:"red")"); n != std::string::npos){
				int pos = match.position(0);
				int size = match.str().size();
				line.replace(pos, size, "XXX");
				//std::cout << " R  " << std::endl;
				//std::cout << line << std::endl;
			}
			else {
				std::string linec = match.str();
				std::smatch matchc;
				int m = 0;
				while(std::regex_search(linec, matchc, regex)){
					m += std::stoi(matchc.str());
					linec = matchc.suffix();
				}
				int p = match.position(0); 
				int size = match.str().size();
				line.replace(p, size, std::to_string(m));
				//std::cout << " Q " << std::endl;
				//std::cout << line << std::endl;
			}	
		}
		std::cout << line << std::endl;
		while(std::regex_search(line, match, regex)){
			sum += std::stoi(match.str());
			//std::cout << n << std::endl;
			line = match.suffix();
		}
	}
	std::cout << "Sum: " << sum << std::endl;

	return 0;
}

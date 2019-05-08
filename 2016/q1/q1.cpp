#include <iostream>
#include <regex>
#include <fstream>
#include <string> 
#include <map>

int main(int argv, char* argc[]){
	
	auto coord = std::pair<int, int>{0,0};
	int direction = 0;
	std::map<std::pair<int,int>, int> locs;
	locs.insert({coord, 0});
	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1])); 
			std::getline(is, line); ){
		for(std::smatch match; std::regex_search(line, match, std::regex(
						"(L|R)(\\d+),?"));){
			auto turn = match.str(1);
			int n = std::stoi(match.str(2));
			line = match.suffix();

			if(turn == "R"){
				direction += 90;
			}
			else if(turn == "L"){
				direction -= 90;
			}

			int gdirection = (direction / 90) % 4;
			for(int i = 0; i < n; ++i){
				if(gdirection == 1){
					coord = {coord.first+1, coord.second};
				}
				else if(gdirection == 2){
					coord = {coord.first, coord.second-1};
				}
				else if(gdirection == 3){
					coord = {coord.first-1, coord.second};
				}
				else if(gdirection == 0){
					coord = {coord.first, coord.second+1};
				}
				if(locs.count(coord)){
					std::cout << "Location: " << coord.first << ":" << coord.second << std::endl;
					std::cout << std::abs(coord.first)+std::abs(coord.second) << std::endl;
					return 0;	
				}
				locs.insert({coord, 0});
//				std::cout << turn << "-" << n << std::endl;
			}
		}
	}
	std::cout << coord.first << ":" << coord.second << std::endl;
	std::cout << std::abs(coord.first)+std::abs(coord.second) << std::endl;

	return 0;
}

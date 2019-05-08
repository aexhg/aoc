#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>


int main(int argv, char* argc[]){

	const static std::regex r(
			"(children|cats|samoyeds|pomeranians|akitas|vizslas|goldfish|trees|cars|"
			"perfumes): (\\d+)");

	const std::unordered_map<std::string,int> mfcsam{
		{"children", 3},
		{"cats", 7},
		{"samoyeds", 2},
		{"pomeranians", 3},
		{"akitas", 0},
		{"vizslas", 0},
		{"goldfish", 5},
		{"trees", 3},
		{"cars", 2},
		{"perfumes", 1},
	};

	std::vector<int> matches;
	for( auto [line, is, i] = std::make_tuple(std::string{}, std::ifstream(argc[1]), int());
			std::getline(is, line); ++i){
		std::unordered_map<std::string, int> m;
		for(std::smatch match; std::regex_search(line, match, r);){
			//std::cout << line << std::endl;
			std::string tp = match.str(1);
			int quantity = std::stoi(match.str(2));
			//std::cout << tp << " " << quantity << std::endl;
			line = match.suffix();
			m[tp] = quantity;
		}
		bool match = true;
		for(auto& kv: m){
			if(kv.first == "cats" || kv.first == "trees"){
				if(kv.second <= mfcsam.at(kv.first)){
					match = false;
				}
			}
			else if(kv.first == "pomeranians" || kv.first == "goldfish"){
				if(kv.second >= mfcsam.at(kv.first)){
					match = false;
				}
			}
			else{
				if(kv.second != mfcsam.at(kv.first)){
					match = false;
				}
			}
		}
		if(match){
			matches.push_back(i);
		}
	}

	std::cout << "Matches: " << std::endl;
	for(int i : matches){
		std::cout << i+1 << std::endl;
	}

	return 0;
}

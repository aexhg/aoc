#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>


int main(int argv, char* argc[]){

	auto doCount = [](const std::vector<std::string>& v, int col){
		std::vector<std::pair<char, int>> p;
		std::unordered_map<char, int> count;
		count.reserve(v.size());
		p.reserve(v.size());

		for(const auto& s: v){
			++count[s[col]];
		}
		for(const auto& kv : count){
			p.push_back(kv);
		}
		std::sort(p.begin(), p.end(), [](const auto& s, const auto& t){
			return s.second < t.second;});
		return (*p.begin()).first;
	};

	std::vector<std::string> bc;
	bc.reserve(10000);

	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1]));
			std::getline(is, line); ){
		bc.emplace_back(std::move(line));
	}

	const int size = bc[0].size();
	std::string message;	
	for(int i = 0; i < size; ++i){
		message += doCount(bc, i);
	}
	std::cout << message << std::endl;



	return 0;
}

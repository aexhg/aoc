#include <iostream>
#include <regex>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <unordered_map>

#include "threadpool.hpp"

int main(int argv, char* argc[]){
	auto rotate = [](char x, int n){
		int z = int(x)+n;
		while(z > int('z')){
			z -= (int('z') - int('a') + 1);
		}
		return char(z);
	};

	auto checkRealRoom = [&rotate](const std::vector<std::string>& names, int sectorId, const std::string& checkSum){
		std::unordered_map<char, int> counts;
		for(const auto& n : names){
			for(char c : n){
				counts[c]++;
			}
		}
		std::vector<std::pair<char, int>> vcount;
		vcount.reserve(counts.size());
		for(const auto& kv : counts){
			vcount.push_back({kv.first, kv.second});
		}

		std::sort(vcount.begin(), vcount.end(),
				[](const auto& p, const auto& q){
					if(p.second == q.second){
						return p.first < q.first;
					}
					return p.second > q.second;
				});
		std::string fullString;
		for(const auto& s : vcount){
			fullString += s.first;
		}
		if(fullString.find(checkSum) != std::string::npos){
			std::string rotated = std::to_string(sectorId) + ":";
			for(const auto &n : names){
				for(char c : n){
					rotated += rotate(c, sectorId);
				}
				rotated += " ";
			}
			return rotated;
		}
		return std::string(); 
	};

	threadpool::ThreadPool tp(4);
	//using result = std::result_of<decltype(checkRealRoom)()>::type;
	std::vector<std::future<std::string>> futures;	
	for(auto [line, is] = std::make_tuple(std::string{}, std::ifstream(argc[1])); 
			std::getline(is, line); ){
		std::vector<std::string> names;
		int sectorId;
		std::string checksum;
		for(std::smatch match; std::regex_search(line, match, std::regex("([a-z]+)|(\\d+)|\\[([a-z]+)\\]"));){
			if(match[1].matched){
				names.push_back(match[1].str());
			}
			else if(match[2].matched){
				sectorId = std::stoi(match[2].str());
			}
			else if(match[3].matched){
				checksum = match[3].str();
			}
			line = match.suffix();
		}
		//for(const auto& n : names){
		//	std::cout << n << std::endl;
		//}
		//std::cout << sectorId << std::endl;
		//std::cout << checksum << std::endl;
		futures.emplace_back(tp.enqueue(checkRealRoom, names, sectorId, checksum));
		//std::cout << v << std::endl;
		//for(auto p : v){
		//	std::cout << p.first << " : " << p.second << std::endl;
		//	std::cout << p << std::endl;
		//}
		//std::cout << "-----" << std::endl;
	}
	//int sum = 0;
	for(auto& f : futures){
		//sum += f.get();
		std::cout << f.get() << std::endl;
	}
	//std::cout << "Sum: " << sum << std::endl;
	return 0;
}

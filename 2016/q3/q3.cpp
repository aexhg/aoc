#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>

int main(int argv, char* argc[]){

	int count = 0;
	auto testTriangle = [](int a, int b, int c){
		return a + b > c && a + c > b && b + c > a;
	};
	
	auto testVecTriangle = [&testTriangle](const std::vector<std::tuple<int, int, int>>& t){
		auto [a, b, c] = t[0];
		auto [d, e, f] = t[1];
		auto [g, h, i] = t[2];
		
		return int(testTriangle(a,d,g))
			+ int(testTriangle(b, e, h))
			+ int(testTriangle(c, f, i));
		
	};

	std::vector<std::tuple<int, int, int>> tris;
	tris.reserve(3);

	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1]));
			std::getline(is, line);){
		if(auto [r, m] = std::make_tuple(std::regex("\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)"), std::smatch());
				std::regex_match(line, m, r)){
			auto [a, b, c] = std::make_tuple(
					std::stoi(m.str(1)),
					std::stoi(m.str(2)),
					std::stoi(m.str(3)));
			tris.push_back({a,b,c});
			if(tris.size() == 3){
				count += testVecTriangle(tris);
				tris.clear();
			}
		}
	}

	std::cout << "Valid: " << count << std::endl;

	return 0;
}

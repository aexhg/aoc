#include <iostream>
#include <regex>
#include <vector>
#include <fstream>

#include "matrix.hpp"

int main(int argv, char* argc[]){
	
	std::regex r("(rotate (column|row) (x|y)=(\\d+) by (\\d+))|(rect (\\d+)x(\\d+))");
	const static int sizei = 6;
	const static int sizej = 50;
	matrix::Matrix m(sizei, sizej);
	
	int linecount = 0;
	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1]));
			std::getline(is, line); ){
		if(std::smatch match; std::regex_match(line, match, r)){
			linecount++;
			if(match[1].matched){
				std::string xy = match[3].str();
				int rc = std::stoi(match[4].str());
				int by = std::stoi(match[5].str());
				if(xy == "x"){
					m.rotateCol(rc, by);
				} else {
					m.rotateRow(rc, by);
				}
			}
			if(match[6].matched){
				int x = std::stoi(match[7].str());
				int y = std::stoi(match[8].str());
				for(int i = 0; i < y; ++i){
					for(int j = 0; j < x; ++j){
						m(i, j) = 1;
					}
				}
			}
			std::cout << m << std::endl;
		}
	}
	int sum = 0;
	for(int i = 0; i < m.sizei(); ++i){
		for(int j = 0; j < m.sizej(); ++j){
			sum += m(i,j);
		}
	}
	std::cout << "Lines: " << linecount << std::endl;
	std::cout << "Sum: " << sum << std::endl;
	return 0;
}

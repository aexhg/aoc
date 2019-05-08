#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <unordered_map>


struct Hash{
	std::size_t operator()(const std::tuple<int,int>& t) const noexcept {
		auto t1 = std::hash<int>{}(std::get<0>(t));
		auto t2 = std::hash<int>{}(std::get<1>(t));
		return t1 ^ ( t2 << 1);
	};
};

std::ostream& operator<<(std::ostream &o, const std::tuple<int,int>& t){
	o << "(" << std::get<0>(t) << "," << std::get<1>(t) << ")";
	return o;
}


std::tuple<int,int> operator+(const std::tuple<int,int>& t, const std::tuple<int,int>& s){
	//std::cout << t << ", " << s << std::endl;
	return std::make_tuple(std::get<0>(t)+std::get<0>(s), std::get<1>(t)+std::get<1>(s));
}

int main(int argc, char *argv[]){

	std::unordered_map<std::tuple<int,int>,int, Hash> grid;
	std::ifstream is(argv[1]);
	const std::unordered_map<char,std::tuple<int,int>> m{
		{'>', {1,0}},
		{'<', {-1,0}},
		{'^', {0,1}},
		{'v', {0,-1}}
	};
	char c;
	auto currentPosition = std::make_tuple(0,0);
	auto roboPosition = std::make_tuple(0,0);
	grid[currentPosition] = 2;
	int turn = 1;
	while(is.get(c)){
		const auto dir = m.at(c);
		//std::cout << "Direction: " << dir << std::endl;
		//std::cout << "Current position: " << currentPosition << std::endl;
		if(turn % 2 == 1){
			currentPosition = currentPosition + dir;		
			grid[currentPosition] += 1;
		}
		else {
			roboPosition = roboPosition + dir;
			grid[roboPosition] += 1;
		}
		turn++;

	}
	std::cout << "Final position: " << currentPosition << std::endl;

	//int countGreaterThanOne = 0;
	//for(auto it = grid.begin(); it != grid.end(); ++it){
	//	if(it->second > 1){
	//		countGreaterThanOne++;
	//	}
	//}

	//std::cout << "Number greater than one: " << countGreaterThanOne << std::endl;
	std::cout << "Size is: " << grid.size() << std::endl;

	return 0;
}

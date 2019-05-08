#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>

class Point{
	public:
		Point() :
			_x(0),
			_y(0)
		{}

		Point(int x, int y) : 
		_x(x),
		_y(y)
		{}

		int X() const {
			return _x;
		}

		int Y() const {
			return _y;
		}

		Point Up() const {
			return {_x, _y - 1};
		}

		Point Down() const {
			return {_x, _y + 1};
		}

		Point Left() const {
			return {_x - 1, _y};
		}
		
		Point Right() const {
			return {_x + 1, _y};
		}

		void operator+(const Point& p){
			_x += p.X();
			_y += p.Y();
		}

	private:
		int _x;
		int _y;
};

bool operator==(const Point& p1, const Point& p2){
	return p1.X() == p2.X() && p2.Y() == p1.Y();
}

Point operator+(const Point&p1, const Point&p2){
	return {p1.X() + p2.X(), p1.Y() + p2.Y()};
}

size_t hash(const Point& p1){
	return std::hash<int>{}(p1.X()) ^ ( std::hash<int>{}(p1.Y()) << 1 );
};

struct CheckBoundary{
	const std::unordered_set<Point, decltype(&hash)> badPoints = {
		{
			{-1, 0},
			{0, -1},
			{1, -2},
			{2, -3},
			{3, -2},
			{4, -1},
			{5, 0}, 
			{4, 1},
			{3, 2},
			{2, 3},
			{1, 2},
			{0, 1},
		//	{-2, 0},
		//	{-2,-1},
		//	{-2,1},
		//	{2, 0},
		//	{2,-1},
		//	{2, 1},
		//	{-1, -2},
		//	{0, -2},
		//	{1, -2},
		//	{-1, 2},
		//	{0, 2},
		//	{1, 2},
		//	},
		},
		100,
		hash
	};

	bool operator()(const Point& p1){
		if(badPoints.count(p1)){
			return false;
		}
		return true;
	}
};


struct ToNumber {
	const std::unordered_map<Point, std::string, decltype(&hash)> numbers = {
		{
			{{0,0}, "5"},
			{{1,0}, "6"},
			{{2,0}, "7"},
			{{3,0}, "8"},
			{{4,0}, "9"},
			{{1,1}, "A"},
			{{2,1}, "B"},
			{{3,1}, "C"},
			{{2,2}, "D"},
			{{1,-1}, "2"},
			{{2, -1}, "3"},
			{{3, -1}, "4"},
			{{2, -2}, "1"},
			//{{-1, -1}, 1},
			//{{0, -1}, 2},
			//{{1,-1}, 3},
			//{{-1, 0}, 4},
			//{{0, 0}, 5},
			//{{1, 0}, 6},
			//{{-1, 1}, 7},
			//{{0, 1}, 8},
			//{{1,1}, 9},
		},
		100,
		hash
	};

	std::string operator()(const Point& p){
		return numbers.at(p);
	}
};

std::ostream& operator<<(std::ostream& out, const Point& pt){
	out << pt.X() << " : " << pt.Y() << "@ " << ToNumber()(pt);
	return out;
}


int main(int argv, char* argc[]){


	Point pt;
	std::string code;

	std::unordered_map<char, std::function<Point(const Point&)>> funcs{
		{'R', [](const Point& p){ return p.Right();}},
		{'L', [](const Point& p){ return p.Left();}},
		{'U', [](const Point& p){ return p.Up();}},
		{'D', [](const Point& p){ return p.Down();}},
		//{'R', std::bind(&Point::Right, std::placeholders::_1)},
		//{'L', std::bind(&Point::Left, &pt)},
		//{'U', std::bind(&Point::Up, &pt)},
		//{'D', std::bind(&Point::Down, &pt)},
	};
	CheckBoundary cb;
	for(auto [line, is] = std::make_tuple(std::string{}, std::ifstream(argc[1]));
			std::getline(is, line); ){
		for(const char& s: line){
			auto newPoint = funcs.at(s)(pt);	
			if(cb(newPoint)){
					pt = newPoint;
			}
		}
		code += ToNumber()(pt);
	}

	std::cout << pt << std::endl;
	std::cout << code << std::endl;

	return 0;
}

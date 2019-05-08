#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>

class Point{
	public:
		Point() :
			_x(0),
			_y(0)
		{}

		Point(int x, int y) : 
		_x(x),
		_y(x)
		{}

		int X() const {
			return _x;
		}

		int Y() const {
			return _y;
		}

		void Up() {
			if(_y >= 0){
				_y -= 1;
			}
		}

		void Down() {
			if(_y <= 0){
				_y += 1;
			}
		}

		void Left() {
			if(_x >= 0){
				_x -= 1;
			}
		}
		
		void Right() {
			if( _x <= 0){
				_x += 1;
			}
		}

		void operator+(const Point& p){
			_x += p.X();
			_y += p.Y();
		}

		int toNumber() const{
			if(_x == -1){
				if(_y == -1){
					return 1;
				}
				else if(_y == 0){
					return 4;
				}
				else if(_y == 1){
					return 7;
				}
			}
			else if(_x == 0){
				if(_y == -1){
					return 2;
				}
				else if(_y == 0){
					return 5;
				}
				else if(_y == 1){
					return 8;
				}
			}
			else if(_x == 1){
				if(_y == -1){
					return 3;
				}
				else if(_y == 0){
					return 6;
				}
				else if(_y == 1){
					return 9;
				}
			}
		}

	private:
		int _x;
		int _y;
};


std::ostream& operator<<(std::ostream& out, const Point& pt){
	out << pt.X() << " : " << pt.Y() << "@ " << pt.toNumber();
	return out;
}

int main(int argv, char* argc[]){


	Point pt;
	std::string code;

	std::unordered_map<char, std::function<void()>> funcs{
		{'R', std::bind(&Point::Right, &pt)},
		{'L', std::bind(&Point::Left, &pt)},
		{'U', std::bind(&Point::Up, &pt)},
		{'D', std::bind(&Point::Down, &pt)},
	};

	for(auto [line, is] = std::make_tuple(std::string{}, std::ifstream(argc[1]));
			std::getline(is, line); ){
		for(const char& s: line){
			funcs.at(s)();	
		}
		code += std::to_string(pt.toNumber());
	}

	std::cout << pt << std::endl;
	std::cout << code << std::endl;

	return 0;
}

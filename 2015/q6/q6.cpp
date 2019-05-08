#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <regex>
#include <unordered_map>

class Matrix{
	public:
		Matrix(int size, int initial = 0) : 
			_size(size),
			_m(size * size, initial){
		}

		
		const int& operator()(int i, int j) const {
			return _m[i * _size + j];
		}

		int& operator()(int i, int j) {
			return _m[i * _size + j];
		}

		const int size() const {
			return _size;
		}

	private:
		int _size;
		std::vector<int> _m;
};

int on(const int& i){
	return i+1; // 1;
}

int off(const int& i){
	return std::max(0, i-1);//0;
}

int toggle(const int& x){
	return x +2;//x == 1 ? 0 : 1;
}

using idx_t = std::pair<int,int>;
void turnOnOff(Matrix &v, idx_t low, idx_t high, const std::function<int(const int&)> &f){

	for(int i = low.first; i <= high.first; ++i){
		for(int j = low.second; j <= high.second; ++j){
			const int state = v(i,j);
			//const int newState = a==action::On ? 1 : (a==action::Off ? 0 : (state == 1 ? 0 : 1));
			v(i,j) = f(state);
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Matrix& m){

	for(int i =0; i < m.size(); ++i){
		for(int j = 0; j < m.size(); ++j){
			out << (m(i,j) == 0 ? "." : "O");
		}
		out << std::endl;
	}
	return out;
}


int main(int argv, char *argc[]){
	
	const std::string filename(argc[1]);
	const bool debug = argv >= 3;

	Matrix m(1000);
	//std::cout << m << std::endl;

	std::function<int (const int&)> fOn = std::bind(on, std::placeholders::_1);
	std::function<int (const int&)> fOff = std::bind(off, std::placeholders::_1);
	std::function<int (const int&)> fTog = std::bind(toggle, std::placeholders::_1);

	const std::unordered_map<std::string, std::function<int (const int&)>> mf
	{
		{"turn on", fOn}, {"turn off", fOff}, {"toggle", fTog}
	};


	std::ifstream is(filename);
	std::string line;

	const static std::regex rgx("(toggle|turn on|turn off) ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)");
	std::smatch matches;

	while(std::getline(is, line)){
		if(std::regex_search(line, matches, rgx)){
			const std::string f = matches[1];
			idx_t idx1 = {std::stoi(matches[2]), std::stoi(matches[3])};
			idx_t idx2 = {std::stoi(matches[4]), std::stoi(matches[5])};

			turnOnOff(m, idx1, idx2, mf.at(f));
		}
	}
	if(debug){
		std::ofstream out("output.txt");
		out << m;
		out.close();
	}
	int count = 0;
	for(int i = 0; i < m.size(); ++i)
		for(int j = 0; j < m.size(); ++j)
			count += m(i,j); 
	std::cout << "Count: " << count << std::endl; 
		
	
	return 0;
 }

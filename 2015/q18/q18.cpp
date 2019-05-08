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

		const std::vector<int> neighbours(int i, int j) const {
			int n1 = (i - 1) * _size + j - 1;
			int n2 = i * _size + j - 1;
			int n3 = (i + 1) * _size + j - 1;
			return { _m[n1], _m[n1 + 1], _m[n1 + 2],
				 _m[n2], _m[n2 + 2],
				 _m[n3], _m[n3 + 1], _m[n3 + 2]};
		}

		const std::vector<int> data() const {
			return _m;
		}

	private:
		int _size;
		std::vector<int> _m;
};

std::ostream& operator<<(std::ostream& out, const Matrix& m){

	for(int i =0; i < m.size(); ++i){
		for(int j = 0; j < m.size(); ++j){
			out << (m(i,j) == 0 ? "." : "X");
		}
		out << std::endl;
	}
	return out;
}

int main(int argv, char *argc[]){
	
	const std::string filename(argc[1]);
	const int N = std::stoi(argc[2]);
	const int M = std::stoi(argc[3]);

	Matrix m(M+2);

	for(auto [line, is, i] = std::make_tuple(std::string{}, std::ifstream(filename), int{1}); 
			std::getline(is, line); ++i){
		for(int j = 0; j < line.size(); ++j){
			m(i,j+1) = line[j] == '#' ? 1 : 0;
		}
	}

	//corners
	m(1,1) = m(M,M) = m(M,1) = m(1,M) = 1;

	for(int t = 0; t < N; ++t){
		Matrix mc(m);
		for(int i = 1; i <= M; ++i){
			for(int j = 1; j <= M; ++j){
				int sum = 0;
				for(auto c : m.neighbours(i,j)){
					sum += c;
				}
				if(m(i,j) == 1 && (sum == 2 || sum == 3)){
					mc(i,j) = 1;
				}
				else if(m(i,j) == 1) { 
					mc(i,j) = 0;
				}
				if(m(i,j) == 0 && sum == 3){
					mc(i,j) = 1;
				}
				else if(m(i,j) == 0) {
					mc(i,j) = 0;
				}
			}
		}
		m = std::move(mc);
		m(1,1) = m(M,M) = m(M,1) = m(1,M) = 1;
	}

	int lightsOn = 0;
	for(int c : m.data()){
		lightsOn += c;
	}
	std::cout << "Lights on: " << lightsOn << std::endl;
	return 0;
 }

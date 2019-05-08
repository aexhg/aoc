#include <iostream>
#include <vector>

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


int main(int argv, char* argc[]){
	static const int I = 3010;
	static const int J = 3019;


	long long val = 20151125;
	static const int multiplier = 252533;
	static const int divisor = 33554393;

	Matrix m(10000);
//
	for(int i = 0;  i < 10000; ++i){
		int idx = i;
		for(int j = 0; j <= i;){
			m(idx,j) = val;	
		//	if(idx == I - 1 && j == J - 1){
		//		std::cout << idx << "," << j <<  "=" << val << std::endl;
		//		return 0;
		//	}
			val = (val * multiplier) % divisor;
			++j;
			--idx;
		}
		
	}
	std::cout << m(I-1, J-1) << std::endl;
	return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

int main(int argv, char* argc[]){

	std::vector<int> v;
	for(auto [line, is] = std::make_tuple(std::string{}, std::ifstream(argc[1]));
			std::getline(is, line);){
		const int l = std::stoi(line);
		v.push_back(l);
	}


	static const int N = std::stoi(argc[2]);
	std::sort(v.begin(), v.end());
	static const int P = 2 << (v.size() - 1);

	std::unordered_map<int, int> combinations;
	for(int i = 0; i < P; ++i){
		int sum = 0;
		int ncontainers = 0;
		for(int j = 0; j < v.size(); ++j){
			if(i & (1 << j)){
				ncontainers++;
				sum += v[j];
			}
		}
		if(sum == N){
			combinations[ncontainers] += 1;
		}
	}

	for(const auto& kv: combinations){
		std::cout << kv.first << " " << kv.second << std::endl;
	}

	return 0;
}

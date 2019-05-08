#include <iostream>
#include <string>
#include <vector>

std::vector<int> calculate(int nelfs, int nhouses){

	std::vector<int> houses(nhouses,0);

	for(int i = 0; i < nelfs; ++i){
		for(int j = (i+1), k =0; j <= nhouses/2 && k < 50; j+=(i+1), ++k){
			if((j % (i+1))==0){
				houses[j-1] += (i+1)*11;
			}
		}
	}
	return houses;
}

int main(int argc, char* argv[]){
	
	int target = std::stoi(argv[1]);
	int nelfs = std::stoi(argv[2]);
	int nhouses = nelfs;
	
	auto v = calculate(nelfs, nhouses);

	//for(int i : v){
	//		std::cout << i << std::endl;
	//	}

	for(int i = 0; i < v.size(); ++i){
		if(v[i] >= target){
			std::cout << "House[" << i+1 << "] has " << v[i] << " presents" << std::endl;
			break;
		}
	}
	return 0;
}

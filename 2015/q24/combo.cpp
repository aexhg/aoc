#include <iostream>
#include <algorithm>
#include <vector>

int main(){
	
	using v_t = std::vector<int>;
	v_t v{1,2,3,4};

	v_t c{0,0,1,1};

	do{
		for(int i = 0; i < c.size(); ++i){
			if(c[i]){
				std::cout << v[i] << ", ";
			}
		}
		std::cout << std::endl;
	}while(std::next_permutation(c.begin(), c.end()));
	return 0;

}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


int main(int argc, char * argv[]){

	std::ifstream is(argv[1]);

	std::string s;
	int total = 0;
	int totalRibbon = 0;
	while(is >> s){
		const int f1 = s.find("x");
		const int length = std::stoi(s.substr(0, f1));
		const int f2 = s.find("x",f1+1);
		const int width = std::stoi(s.substr(f1+1, 5));
		const int height = std::stoi(s.substr(f2+1, -1));
		const int a1 = length * width;
		const int a2 = length * height;
		const int a3 = width * height;
		const int totalwrapping = 2* a2 + 2 * a1 + 2 * a3 ;
		//std::cout << length << "x" << width << "x" << height << "=" << totalwrapping << std::endl;
		const int minSurface = std::min(a1, std::min(a2, a3));
		//std::cout << "area: " << totalwrapping + minSurface << std::endl;
		const int volume = length * width * height; 
		const int minPerimeter = 2 * std::min(width+height, std::min(width+length, length + height));
		total += minSurface + totalwrapping;
		totalRibbon += minPerimeter + volume;
	}
	std::cout << "Total: " << total << std::endl;;
	std::cout << "Ribbon: " << totalRibbon << std::endl;
	return 0;
}


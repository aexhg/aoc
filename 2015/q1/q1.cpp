#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
	std::ifstream is(argv[1]);

	int floor = 0; 
	int pos = 0;
	char c;
	while(is.get(c)){
		pos++;
		floor += c == '(' ? 1 : -1;
		if(floor == -1){
			std::cout << "Entered basement at position: " << pos << std::endl;
			break;
		}
		
	}
	std::cout << "The floor is: " << floor << std::endl;

	return 0;
}

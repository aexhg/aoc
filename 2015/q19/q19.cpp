#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <regex>

int main(int argv, char* argc[]){

	const static std::regex r("(\\S+) => (\\S+)");
	std::vector<std::pair<std::string, std::string>> moleculesTransforms;
	std::string molecule;
	for(auto [line, is] = std::make_tuple(std::string{}, std::ifstream(argc[1]));
			std::getline(is, line);){
		if(std::smatch m; std::regex_match(line, m, r)){
			moleculesTransforms.push_back({m.str(1), m.str(2)});
		}

		if(std::smatch m; std::regex_match(line, m, std::regex("(\\S+)"))){
			molecule = m.str();
		}
	}

	std::unordered_set<std::string> transforms;
	for(const auto& kv : moleculesTransforms){
		int pos = molecule.rfind(kv.first, 0);
		//std::cout << kv.first << "=>" << kv.second << std::endl;
		for(; pos != std::string::npos;){
			std::string s = molecule;
			s.replace(pos, kv.first.size(), kv.second);
			transforms.insert(s);
			//std::cout << s << std::endl;
			pos = molecule.find(kv.first, pos+1);
		}
	}

	std::cout << "Size: " << transforms.size() << std::endl;
	int nsteps = 0;

	//for(auto& kv : moleculesTransforms){
	//	std::reverse(kv.first.begin(), kv.first.end());
	//	std::reverse(kv.second.begin(), kv.second.end());
	//}

	//std::reverse(molecule.begin(), molecule.end());
	//std::sort(moleculesTransforms.begin(), moleculesTransforms.end(),
	//		[](auto k1, auto k2){ return k1.second.size() > k2.second.size();});
	std::sort(moleculesTransforms.begin(), moleculesTransforms.end());

	std::random_device rd;
	std::mt19937 g(rd());
	int shuffles = 0;
	for(bool found = false; !found; ){
		std::string mol = molecule;
		for(;;){
			std::string s = mol;
			if(mol == "e"){
				found = true;
				break;
			}
			
			for(const auto& kv : moleculesTransforms){
				int pos = mol.find(kv.second, 0);
				for(; pos != std::string::npos;){
					mol.replace(pos, kv.second.size(), kv.first);
					nsteps++;
					pos = mol.find(kv.second, pos + 1);
				}
			}

			if(s == mol){
				mol = molecule;
				nsteps = 0;
				std::shuffle(moleculesTransforms.begin(), moleculesTransforms.end(), g);
				shuffles++;
				break;
			}
		}
	}

	std::cout <<  "Steps backwards: " << nsteps << " With shuffles: " << shuffles  << std::endl;

	for(const auto &kv : moleculesTransforms){
		std::cout << kv.first << " : " << kv.second << std::endl;
	}
	return 0;
}

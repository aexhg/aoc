#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_set>
#include <unordered_map>

int main(int argv, char* argc[]){

	const static std::regex r("^(\\w+) would (lose|gain) (\\d+) happiness units by sitting next to (\\w+).$");
	std::string filename(argc[1]);
	std::unordered_set<std::string> names;
	std::unordered_map<std::string, std::unordered_map<std::string, int>> values;
	for(auto [is, line] = std::make_pair(std::ifstream(filename), std::string{});  std::getline(is, line); ){
		if(std::smatch match; std::regex_match(line, match, r)){
			std::string name1 = match.str(1);
			std::string name2 = match.str(4);
			std::string gainOrLose = match.str(2);
			int value = std::stoi(match.str(3));

			names.insert(name1);
			names.insert(name2);
			names.insert("me");

			values[name1].insert({name2, gainOrLose == "gain" ? value : -value});
			values["me"].insert({name2, 0});
			values["me"].insert({name1, 0});
			values[name1].insert({"me", 0});
			values[name2].insert({"me", 0});
		}
	}

	//for(const std::string& s: names){
	//	std::cout << s << std::endl;
	//}

	//for(auto& kv : values){
	//	std::cout << kv.first << std::endl;
	//	for(auto& qp: kv.second){
	//		std::cout << qp.first << " " << qp.second << std::endl;
	//	}
	//}
	
	std::vector<std::string> vnames;
	vnames.reserve(names.size());
	std::copy(names.begin(), names.end(), std::back_inserter(vnames));

	std::sort(vnames.begin(), vnames.end());
	int maxScore = std::numeric_limits<int>::min();
	std::vector<std::string> bestCombo;
	bestCombo.reserve(names.size());

	for(; std::next_permutation(vnames.begin(), vnames.end()); ){
		int m = 0; 
		for(int i = 0; i < vnames.size(); ++i){
			int n1 = i == 0 ? vnames.size() - 1 : i-1;
			int n2 = i == vnames.size() - 1 ? 0 : i+1;
			const std::string name = vnames[i];
			const std::string name1 = vnames[n1];
			const std::string name2 = vnames[n2];
			int v1 = values.at(name).at(name1);
			int v2 = values.at(name).at(name2);
			m += v1 + v2;
		}
		if(m > maxScore){
			maxScore = m;
			bestCombo = vnames;
		}
		
	} 
	for(const std::string& s: bestCombo){
		std::cout << s << std::endl;
	}

	std::cout << "max score: " << maxScore << std::endl;
	return 0;
}

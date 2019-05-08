#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "threadpool.hpp"

int main(int argv, char* argc[]){

	auto check = [](const std::string& s, size_t start){
		
		auto [seenInBracket, seen] = std::make_tuple(false, false);
		for(auto [i, inBracket] = std::make_tuple(start, false); i < s.size() - 3; ++i){
			if(s[i] == '['){
				inBracket = true;
			}
			else if(s[i] == ']'){
				inBracket = false;
			}
			if((s[i]==s[i+3]) && (s[i+1] == s[i+2]) && !(s[i] == s[i+1])){
				if(inBracket){
					seenInBracket = true;
					continue;
				}
				seen  = true;
			}
		}
		return seenInBracket ? false : seen;
	};

	auto checkt = [](const std::string& s, size_t start){

		std::unordered_set<std::string> seqso, seqsi;
		bool inBracket = false;
		for(size_t i = 0; i < s.size()-2; ++i){
			if(s[i] == '['){
				inBracket = true;
			}
			else if(s[i] == ']'){
				inBracket = false;
			}
			if( (s[i] == s[i+2]) && (s[i+1] != s[i])){

				std::string seq;
				if(inBracket){
					seq.push_back(s[i]);
					seq.push_back(s[i+1]);
					seq.push_back(s[i]);
					seqsi.insert(seq);
				}
				else{
					seq.push_back(s[i+1]);
					seq.push_back(s[i]);
					seq.push_back(s[i+1]);
					seqso.insert(seq);
				}
			}
		}

		for(auto s : seqsi){
			if(seqso.count(s)){
				return true;
			}
		}
		return false;
	};

	std::vector<std::future<bool>> futures;
	threadpool::ThreadPool tp(4);

	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1])); 
			std::getline(is, line); ){
		
		futures.emplace_back(tp.enqueue(checkt, line, 0));

	}

	int count = 0;
	for(auto& f : futures){
		count += f.get();
	}

	std::cout << "Valid count: " << count << std::endl;
	return 0;
}

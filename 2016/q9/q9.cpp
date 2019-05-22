#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>
#include "threadpool.hpp"

int main(int argv, char* argc[]){


	const static std::regex r = std::regex("(\\((\\d+)x(\\d+)\\))");
	auto decompress = [](auto const& line){
		std::string s;
		s.reserve(line.size()*1000);
		for(auto f1 = line.find("("); f1 != std::string::npos;){
			auto f2 = line.find(")");
			auto fx = line.find("x", f1+1);
			auto x = std::stoi(line.substr(f1+1));
			auto y = std::stoi(line.substr(fx+1));
			auto subs = line.substr(f2+1, x);
		
			for(int k = 0; k < y; ++k){
				s += subs;
			}
			f1 = line.find("(", f1 + 1);
		}
		return s.size();
		//for(int i = 0; i < line.size();){
		//	if(line[i]=='('){
		//		auto sub = line.find(")", i);
		//		auto substr = line.substr(i, sub - i + 1);
		//		if(std::smatch match; sub != std::string::npos &&
		//				std::regex_match(substr, match, r)){
		//			int x = std::stoi(match.str(2));
		//			int y = std::stoi(match.str(3));

		//			auto subs = line.substr(sub+1, x);
		//			for(int k = 0; k < y; ++k){
		//				s += subs;
		//			}

		//			i += (sub - i + 1 + x);
		//		}
		//	} else {
		//		s += line[i];
		//		++i;
		//	}
		//}
		//return s;
	};

	auto decompressR = [&decompress](const auto& line){
		std::string s = line;
		int size = 0;
		int pos = 0;
		for(auto m = std::smatch(); std::regex_search(s, m, r); ){
			auto x = std::stoi(m.str(2));
			auto y = std::stoi(m.str(3));
			pos = m.position();
			auto sub =  m.str();
			auto prf = std::string(m.prefix());
			auto suff = std::string(m.suffix());
			auto tail = suff.substr(0,x);
			size += decompress(sub+tail);
			size += prf.size();
			if(x+1 < tail.size()){
				s = tail.substr(x+1);
			} else {
				s  = "";
			}
			
		}
		return size;
	};

//	threadpool::ThreadPool tp(4);

	for(auto [line, is] = std::make_tuple(std::string(), std::ifstream(argc[1]));
			std::getline(is, line);){
		size_t size = 0;
		
//		std::vector<std::future<size_t>> futs;
		for(std::smatch m; std::regex_search(line, m, r);){
			const auto pos = m.position();
			int x = std::stoi(m.str(2));
			std::stringstream ss;
			ss << m.prefix() << m.str() << std::string(m.suffix()).substr(0,x);
//			std::cout << m.prefix() << std::endl;
//			std::cout << m.str() << std::endl;
//			std::cout << std::string(m.suffix()).substr(0,x) << std::endl;
			//auto sub = std::string(m.prefix()) +  m.str() + std::string(m.suffix()).substr(0, x);
			auto sub = ss.str();
			line = std::move(std::string(m.suffix()).substr(x));
			//std::cout << std::string(m.str(0)) << " : " << sub; 
			size += decompressR(std::move(sub));
			std::cout << " : " << size << std::endl;

//			futs.emplace_back(tp.enqueue(decompressR, std::move(sub)));
		} 
		if(line.size()){
			size += line.size();
		}

//		for(auto& f : futs){
//			size += f.get();
//		}

		std::cout << size << std::endl;
		
	}

	return 0;
}

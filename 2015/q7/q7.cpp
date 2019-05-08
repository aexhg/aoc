#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

using graph_t = boost::adjacency_list<
		boost::vecS,
	      	boost::vecS,
		boost::directedS,
		boost::property<boost::vertex_name_t, std::string>>;
using map_t = boost::property_map<graph_t, boost::vertex_name_t>::type;
using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
using nodeMap_t = std::unordered_map<std::string, vertex_t>;
using valueMap_t = std::unordered_map<std::string, char16_t>;
using function_t = std::function<char16_t(const valueMap_t&)>;
using functionMap_t = std::unordered_map<std::string, function_t>;

vertex_t& checkNodeCreate(graph_t& G, nodeMap_t& nodeMap, map_t nodeVertexMap, const std::string& name)
{
	if(nodeMap.count(name) == 0){
		auto v = boost::add_vertex(G);
		nodeMap[name] = v;
		nodeVertexMap[v] = name;
	}
	return nodeMap[name];
	
}

char16_t fset(const valueMap_t& m, char16_t value){
	return value;
}

char16_t fsetv(const valueMap_t& m, const std::string& s){
	return m.at(s);
}

char16_t fand(const valueMap_t& m, const std::string& s, const std::string& t){
	return m.at(s) & m.at(t);
}

char16_t fandi(const valueMap_t& m, const std::string& s, char16_t i){
	return i & m.at(s);
}

char16_t forr(const valueMap_t& m, const std::string& s, const std::string& t){
	return m.at(s) | m.at(t);
}

char16_t forri(const valueMap_t& m, const std::string& s, char16_t i){
	return i |  m.at(s);
}

char16_t fnot(const valueMap_t& m, const std::string& s){
	return ~m.at(s);
}

char16_t frshift(const valueMap_t& m, const std::string& s, char16_t n){
	return m.at(s) >> n;
}

char16_t flshift(const valueMap_t& m, const std::string& s, char16_t n){
	return m.at(s) << n;
}

int main(int argv, char* argc[]){

	std::ifstream is(argc[1]);
	int debug = std::stoi(argc[2]);

	std::string line;

	using graph_t = boost::adjacency_list<
		boost::vecS,
	      	boost::vecS,
		boost::directedS,
		boost::property<boost::vertex_name_t, std::string>>;
	graph_t G;

	nodeMap_t  nodeVertexMap;
	valueMap_t valueMap;
	functionMap_t functionMap;
	map_t nodeName = boost::get(boost::vertex_name, G); 
	
	std::smatch match;
	while(std::getline(is, line) && line.size()){
		const static std::regex rgx1("((\\d+)\\s)((OR|AND)\\s)([a-z]+) -> ([a-z]+)",
			std::regex_constants::ECMAScript);
		const static std::regex rgx2("((\\w+)\\s)((OR|AND)\\s)([a-z]+) -> ([a-z]+)",
			std::regex_constants::ECMAScript);
		const static std::regex rgx3("((\\w+)\\s)((LSHIFT|RSHIFT)\\s)(\\d+) -> ([a-z]+)",
			std::regex_constants::ECMAScript);
		const static std::regex rgx4("NOT\\s(\\w+) -> (\\w+)",
			std::regex_constants::ECMAScript);
		const static std::regex rgx5("(\\d+) -> (\\w+)",
			std::regex_constants::ECMAScript);
		const static std::regex rgx6("(\\w+) -> (\\w+)",
			std::regex_constants::ECMAScript);
		if(std::regex_match(line, match, rgx1)){
			char16_t x = std::stoi(match.str(2));
			std::string y = match.str(5);
			std::string z = match.str(6);
			auto v = checkNodeCreate(G, nodeVertexMap, nodeName, y);
			auto w = checkNodeCreate(G, nodeVertexMap, nodeName, z);
			boost::add_edge(v, w, G);
			if(match.str(4)=="AND"){
				function_t f = 	std::bind(fandi, std::ref(valueMap), y, x);
				functionMap[z] = f;
			}
			if(match.str(4)=="OR"){
				function_t f = std::bind(forri, std::ref(valueMap), y, x);
				functionMap[z] = f;
			}
		}
		else if(std::regex_match(line, match, rgx2)){
			std::string x = match.str(2);
			std::string y = match.str(5);
			std::string z = match.str(6);
			auto u = checkNodeCreate(G, nodeVertexMap, nodeName, x);
			auto v = checkNodeCreate(G, nodeVertexMap, nodeName, y);
			auto w = checkNodeCreate(G, nodeVertexMap, nodeName, z);
			boost::add_edge(v, w, G);
			boost::add_edge(u, w, G);
			if(match.str(4)=="AND"){
				function_t f = 	std::bind(fand, std::ref(valueMap), y, x);
				functionMap[z] = f;
			}
			if(match.str(4)=="OR"){
				function_t f = std::bind(forr, std::ref(valueMap), y, x);
				functionMap[z] = f;
			}
		}
		else if(std::regex_match(line, match, rgx3)){
			auto u = checkNodeCreate(G, nodeVertexMap, nodeName, match.str(2));
			auto v = checkNodeCreate(G, nodeVertexMap, nodeName, match.str(6));
			boost::add_edge(u, v, G);
			if(match.str(4)=="LSHIFT"){
				function_t f = std::bind(flshift, std::ref(valueMap), match.str(2), std::stoi(match.str(5)));
				functionMap[match.str(6)] = f;
			}
			if(match.str(4)=="RSHIFT"){
				function_t f = std::bind(frshift, std::ref(valueMap), match.str(2), std::stoi(match.str(5)));
				functionMap[match.str(6)] = f;
			}
		}
		else if(std::regex_match(line, match, rgx4)){
			auto v = checkNodeCreate(G, nodeVertexMap, nodeName, match.str(1));
			auto w = checkNodeCreate(G, nodeVertexMap, nodeName, match.str(2));
			boost::add_edge(v, w, G);
			function_t f = std::bind(fnot, std::ref(valueMap), match.str(1));
			functionMap[match.str(2)] = f;
		}
		else if(std::regex_match(line, match, rgx5)){
			char16_t value = std::stoi(match.str(1));
			function_t f = std::bind(fset, std::ref(valueMap), value); 
			functionMap[match.str(2)] = f;
		}
		else if(std::regex_match(line, match, rgx6)){
			auto v = checkNodeCreate(G, nodeVertexMap, nodeName, match.str(1));
			auto w = checkNodeCreate(G, nodeVertexMap, nodeName, match.str(2));
			function_t f = std::bind(fsetv, std::ref(valueMap), match.str(1));
			functionMap[match.str(2)] = f;
			boost::add_edge(v, w, G);
		}
		else{
			std::cout << "No regex matched: " << line << std::endl;
			return -1;
		}
	}

	if(debug > 1){

		for(auto& kv : functionMap){
			std::cout << "key: " << kv.first << " length: " << kv.first.size() << std::endl;
		}
	}

	std::vector<boost::graph_traits<graph_t>::vertex_descriptor> vertices;
	boost::topological_sort(G, std::back_inserter(vertices));
	if(debug>2){
		for(auto it = vertices.rbegin(); it != vertices.rend(); ++it){
			const std::string name = nodeName[*it];
			std::cout << name << std::endl;
		}
	}
	
	for(auto it = vertices.rbegin(); it != vertices.rend(); ++it){

		const std::string name = nodeName[*it];
		if(debug){
			std::cout << "name: " << name << std::endl;
		}
		auto f = functionMap.at(name);
		char16_t value = f(valueMap);
		valueMap[name] = value;
		
		if(debug){
			std::cout << nodeName[*it] << std::endl;
			for(auto &kv : valueMap)
				std::cout << kv.first << " : " << kv.second << std::endl;
		}
	}
	for(auto &kv : valueMap)
		std::cout << kv.first << " : " << kv.second << std::endl;

	if(valueMap.count("a")){
		std::cout << "a: " << valueMap.at("a") << std::endl;
	}
	return 0;
}


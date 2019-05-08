#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using graph_t = boost::adjacency_list<
		boost::vecS,
	      	boost::vecS,
		boost::undirectedS,
		boost::property<boost::vertex_name_t, std::string>,
		boost::property<boost::edge_weight_t, double>>;

using vertexmap_t = boost::property_map<graph_t, boost::vertex_name_t>::type;
using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
using edgemap_t = boost::property_map<graph_t, boost::edge_weight_t>::type;
using edge_t = boost::graph_traits<graph_t>::edge_descriptor;

int main(int argv, char* argc[])
{
	int debug =0;
	if(argv >= 3){
		debug = std::stoi(argc[2]);
	}

	std::unordered_map<std::string, vertex_t> nodeMap;
	graph_t G;
	vertexmap_t vertexmap = boost::get(boost::vertex_name, G);
	edgemap_t edgemap = boost::get(boost::edge_weight, G);

	const std::string filename = argc[1];
	std::ifstream is(filename);
	const std::regex rgx("(\\w+) to (\\w+) = (\\d+)");
	std::smatch match;
	std::string line;

	while(std::getline(is, line)){
		if(std::regex_match(line, match, rgx)){
			if(debug > 3){
				std::cout << "line " << line << " " << "regex match size: " << match.size() << std::endl;
				std::cout << "match 1 " << match.str(1) << std::endl;
				std::cout << "match 2 " << match.str(2) << std::endl;
			}
			const std::string u = match.str(1);
			const std::string v = match.str(2);
			int distance = std::stoi(match.str(3));
			vertex_t uv, vv;
			if(nodeMap.count(u)){
				uv = nodeMap.at(u);
			} 
			else{
				uv = boost::add_vertex(G);
				vertexmap[uv] = u;
				nodeMap[u] = uv;
			}
			if(nodeMap.count(v)){
				vv = nodeMap.at(v);
			}
			else{
				vv = boost::add_vertex(G);
				vertexmap[vv] = v;
				nodeMap[v] = vv;
			}
			auto [edge, inserted] = boost::add_edge(uv, vv, G);
			edgemap[edge] = distance;
		}
	}



	if(debug > 2){
		auto [begin, end] = boost::vertices(G);
		std::cout << "Vertices: " << std::endl;
		for( ; begin != end; ++begin){
			std::cout << vertexmap[*begin] << std::endl;
		}
		std::cout << "Edges: " << std::endl;
		auto [eb, ee] = boost::edges(G);
		for( ; eb != ee; ++eb){
			std::cout << edgemap[*eb] << std::endl;
			std::cout << vertexmap[boost::source(*eb, G)] << std::endl;
			std::cout << vertexmap[boost::target(*eb, G)] << std::endl;
		}
	}
	
	std::vector<std::string> vertexNames;
	auto [begin,end] = boost::vertices(G);
	std::transform(begin, end, std::back_inserter(vertexNames), [&vertexmap](const vertex_t&t){
			return vertexmap(t);});

	std::sort(vertexNames.begin(), vertexNames.end());
	double minWeight = std::numeric_limits<double>::max();
	double maxWeight = std::numeric_limits<double>::min();
	std::vector<std::string> chosenRoute;
	std::vector<std::string> maxRoute;

	while(std::next_permutation(vertexNames.begin(), vertexNames.end())){
		double routeWeight = 0;
		for(int i = 1; i < vertexNames.size(); ++i){
			auto s = nodeMap[vertexNames[i]];
			auto t = nodeMap[vertexNames[i-1]];
			auto [edge, exists] = boost::edge(s, t, G);
			if(!exists){
				std::cout << "Could not find edge for: " << vertexNames[i] << "->"
					<< vertexNames[i-1] << std::endl;
				return -1;
			}
			routeWeight += edgemap[edge];
		}
		if(routeWeight < minWeight){
			minWeight = routeWeight;
			chosenRoute = vertexNames;
		}
		if(routeWeight > maxWeight){
			maxWeight = routeWeight;
			maxRoute = vertexNames;
		}
	}
	std::cout << "Min distance: " << minWeight << std::endl;
	std::cout << "Max distance: " << maxWeight << std::endl;

	std::cout << "Route min: " << std::endl;
	for(const auto& s: chosenRoute){
		std::cout << s << std::endl;
	}

	std::cout << "Route max: " << std::endl;
	for(const auto& s: maxRoute){
		std::cout << s << std::endl;
	}

	
	//std::vector<edge_t> edges;
	//boost::kruskal_minimum_spanning_tree(G, std::back_inserter(edges));
	//
	//double totalWeight = 0;
	//for(const auto& v : edges){
	//	
	//	auto weight = edgemap[v];
	//	totalWeight += weight;
	//	
	//	if(debug > 2){
	//		auto source = boost::source(v, G);
	//		auto target = boost::target(v, G);
	//		std::cout << "Source: " << vertexmap[source] << " Target: " 
	//			<< vertexmap[target] << " Weight: " << weight << std::endl;
	//	}
	//}
	//std::cout << " Total weight: " << totalWeight << std::endl;

}

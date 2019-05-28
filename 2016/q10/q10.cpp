#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/topological_sort.hpp>

struct Robo {
	int id;
	bool output;
	std::vector<int> values;
};

//enum Edge { MIN, MAX };
enum EdgeType {LOW, HIGH};
struct Edge {
	EdgeType edgeType;
};


using graph_t = boost::adjacency_list<
		boost::vecS,
	      	boost::vecS,
		boost::directedS,
		Robo,
		Edge>;
		//boost::property<boost::vertex_name_t, int, boost::property<boost::vertex_color_t, int>>,
		//boost::property<boost::edge_weight_t, int>>;

//using vertex_name_map_t = boost::property_map<graph_t, boost::vertex_name_t>::type;
//using vertex_color_map_t = boost::property_map<graph_t, boost::vertex_color_t>::type;
using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
//using edgemap_t = boost::property_map<graph_t, boost::edge_weight_t>::type;
using edge_t = boost::graph_traits<graph_t>::edge_descriptor;
using robo_map_t = std::unordered_map<int, vertex_t>; 
using output_map_t = std::unordered_map<int, vertex_t>;

struct VertexWriter{
	VertexWriter(const graph_t& G) : _G(G)  {}
	template<typename V>
	void operator()(std::ostream& os, const V& v){
		os << " [label=" << _G[v].id << " color=" << (_G[v].output ? "blue" : "red" ) << "]";
	}
	const graph_t& _G;
};

struct EdgeWriter{
	EdgeWriter(const graph_t& G) : _G(G)  {}
	template<typename E>
	void operator()(std::ostream& os, const E& e){
		os << " [color=" << (_G[e].edgeType == EdgeType::HIGH ? "blue" : "red" ) << "]";
	}
	const graph_t& _G;
};

auto& createOrGetRobo(robo_map_t& roboMap,
		graph_t& g,
		int roboId){
	if(!roboMap.count(roboId)){
		auto robo = Robo{roboId, false};
		auto v = boost::add_vertex(robo, g);
		roboMap.emplace(roboId, v);
	}
	return roboMap.at(roboId);
}

auto& createOrGetOutput(robo_map_t& outputMap,
		graph_t& g,
		int outputId){
	if(!outputMap.count(outputId)){
		auto output = Robo{outputId, true};
		auto v = boost::add_vertex(output, g);
		outputMap.emplace(outputId, v);
	}
	return outputMap.at(outputId);
}


int main(int argv, char* argc[]){
	graph_t G;
	robo_map_t roboMap;	
	output_map_t outputMap;

	for(auto [is, line] = std::make_pair(std::ifstream(argc[1]), std::string());
			std::getline(is, line);){
		std::vector<std::string> strs;
		boost::split(strs, line, boost::is_any_of(" "));
		if(strs[0] == "value"){
			int value = std::stoi(strs[1]);
			int bot = std::stoi(strs[5]);
			auto& vertex = createOrGetRobo(roboMap,G, bot);
			G[vertex].values.push_back(value);

		}
		if(strs[0] == "bot"){
			int bot = std::stoi(strs[1]);
			int low = std::stoi(strs[6]);
			int high = std::stoi(strs[11]);
			bool botOutputLow = strs[5] == "output" ? true : false;
			bool botOutputHigh = strs[10] == "output" ? true : false;
			
			auto& vertex = createOrGetRobo(roboMap,
					G,
					bot);

			if(botOutputLow){
				auto& outputVertex = createOrGetOutput(outputMap,
						G,
						low);
				auto e = boost::add_edge(vertex, outputVertex, G);
				G[e.first].edgeType = EdgeType::LOW;
			} else {
				auto& robo = createOrGetRobo(roboMap,
						G,
						low);
				auto e = boost::add_edge(vertex, robo, G);
				G[e.first].edgeType = EdgeType::LOW;
			}

			if(botOutputHigh){
				auto& outputVertex = createOrGetOutput(outputMap,
						G,
						high);
				auto e = boost::add_edge(vertex, outputVertex,  G);
				G[e.first].edgeType = EdgeType::HIGH;
			} else {
				auto& robo = createOrGetRobo(roboMap,
						G,
						high);
				auto e = boost::add_edge(vertex, robo, G);
				G[e.first].edgeType = EdgeType::HIGH;
			}

		}
	}
	
	//boost::dynamic_properties dp;
	//dp.property("color", boost::make_function_property_map<Graph*>([](Graph const* g) { return g->m_property->orientation; }));
	//dp.property("color", boost::get(&Robo::output, G));
	//dp.property("label", boost::get(&Robo::id, G));
	//dp.property("node_id", boost::get(boost::vertex_index, G));
	//
	//boost::write_graphviz_dp(std::cout, G, dp);
	//boost::write_graphviz(std::cout, G, VertexWriter(G), EdgeWriter(G));
	
	std::vector<vertex_t> vertices;
	boost::topological_sort(G, std::back_inserter(vertices));

	for(auto it = vertices.rbegin(); it != vertices.rend(); ++it){
		auto vType = G[*it].output ? "Output" : "Vertex";
		std::cout << vType << ": " << G[*it].id << std::endl;
		auto rng = boost::out_edges(*it, G);
		for(auto ir = rng.first; ir != rng.second; ++ir){
			auto lowhigh = G[*ir].edgeType == EdgeType::LOW ? "LOW" : "HIGH";
			auto target = boost::target(*ir, G);
			auto vType2 = G[target].output ? "Output" : "Vertex";

			std::cout << "   " << lowhigh << " " << vType2 << "  range: " << G[target].id << std::endl;
		}
	}

	return 0;
}


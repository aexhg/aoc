#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>

template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
   if ((first == last) || (first == k) || (last == k))
      return false;
   Iterator itr1 = first;
   Iterator itr2 = last;
   ++itr1;
   if (last == itr1)
      return false;
   itr1 = last;
   --itr1;
   itr1 = k;
   --itr2;
   while (first != itr1)
   {
      if (*--itr1 < *itr2)
      {
         Iterator j = k;
         while (!(*itr1 < *j)) ++j;
         std::iter_swap(itr1,j);
         ++itr1;
         ++j;
         itr2 = k;
         std::rotate(itr1,j,last);
         while (last != j)
         {
            ++j;
            ++itr2;
         }
         std::rotate(k,itr2,last);
         return true;
      }
   }
   std::rotate(first,k,last);
   return false;
}

struct Microchips {
  static const int Hydrogen=1;
  static const int Lithium=2;
};

struct Generators {
  static const int Hydrogen=-Microchips::Hydrogen;
  static const int Lithium=-Microchips::Lithium;
};

struct State {
  int liftFloor;
  std::unordered_map<int, std::unordered_set<int>> floorItems;
};

std::size_t hash(const State& s){
  std::size_t seed = 0; 
  boost::hash_combine(seed, s.liftFloor);
  for(const auto& kv : s.floorItems){
    boost::hash_combine(seed, kv.first);
    for(const auto& i : kv.second){
      boost::hash_combine(seed, i);
    }
  }
  return seed;
}


bool compare(const State& s, const State& p){
  return hash(s)==hash(p);
}

struct vertex_state_t {
  using kind = boost::vertex_property_tag;
};

using vertex_property_t = 
  boost::property<boost::vertex_color_t, boost::default_color_type,
  boost::property<boost::vertex_rank_t, unsigned int,
  boost::property<boost::vertex_distance_t, unsigned int,
  boost::property<boost::vertex_predecessor_t, unsigned int,
  boost::property<vertex_state_t, State>>>>>;

using edge_property_t = 
  boost::property<boost::edge_weight_t, unsigned int>;

using graph_t = 
 boost::adjacency_list<boost::listS, boost::vecS,
  boost::undirectedS, vertex_property_t, edge_property_t>;

using vertex_t = graph_t::vertex_descriptor;
using vertex_state_map_t = boost::property_map<graph_t, vertex_state_t>;
using vertex_predecessor_map_t = boost::property_map<graph_t, boost::vertex_predecessor_t>;
using edge_weight_map_t = boost::property_map<graph_t, boost::edge_weight_t>;

bool validFloors(const std::unordered_set<int>& f){

  if(std::all_of(f.begin(), f.end(), [](auto i){return i>0;})){
    return true;
  } else if(std::all_of(f.begin(), f.end(), [](auto i){return i < 0;})){
    return true;
  } else {
    for(auto i : f){
      if(i > 0 && !f.count(-i)){
        return false;
      }
    }
  }
  return true;
}

std::vector<State> generateStates(const State& s){
  std::vector<State> states;

  for(int c = 1; c <= 2; ++c){

    auto begin = s.floorItems.at(s.liftFloor).begin();
    auto end = s.floorItems.at(s.liftFloor).end();
    std::vector<int> items(begin, end);
    std::sort(items.begin(), items.end());
    const int oldFloor= s.liftFloor;
    for(int f = -1; f <=1; f += 2){
      
      const int newFloor = oldFloor+f;

      do {
        std::unordered_map<int, std::unordered_set<int>> newFloors(s.floorItems);
        if(newFloor>=0 && newFloor<4){
          for(int k = 0; k < c; ++k) {
            newFloors[newFloor].insert(items[k]);
            newFloors[oldFloor].erase(items[k]);
          }
          if(validFloors(newFloors.at(newFloor)) && validFloors(newFloors.at(oldFloor))){
            states.emplace_back(State{newFloor, newFloors});
          }
        }

      } while(next_combination(items.begin(), items.begin()+c, items.end()));

    }
  }
  return states;
}


std::ostream& operator<<(std::ostream& os, const State& s){
  //os << s.floorItems.size() << std::endl;
  //for(const auto& kv  : s.floorItems){
  os << "Hash: " << hash(s) << std::endl;
  for(int i = 3; i >= 0; --i){
    auto floor = i; 
    auto items = s.floorItems.at(i);
    os << floor << " <> " ;
    if(floor == s.liftFloor){
      os << " E " << " -- " ;
    } else {
      os << "   " << " -- " ;
    }
    for(const auto& it : items){
      os << it << " -- " ;
    }
    os << std::endl;
  }
  return os; 
}

struct Found {
};

class AStarVisitor : public boost::default_astar_visitor {
  public:
    AStarVisitor(vertex_t goal) : goal(goal) {
    }
    
    template<typename Vertex, typename Graph>
    void examine_vertex(Vertex u, Graph& G){
      if(u == goal){
        throw Found();
      }

      auto stateMap = boost::get(vertex_state_t(), G);
      auto thisState = stateMap[u];
      const auto newStates = generateStates(thisState);
      for(const auto& s : newStates){
        if(seenStates.count(s)){
          //auto& v = seenStates[s];
          //boost::add_edge(u, v, G);
        } else {
          //auto v = boost::add_vertex(vertex_property_t(boost::white_color), G);
          //seenStates.emplace(s, v);
          //boost::add_edge(u, v, G);
        }
      }
    }
  private:
    vertex_t goal;
    std::unordered_map<State, vertex_t, decltype(&hash),
      decltype(&compare)> seenStates;
};

class AStarHeuristic : public boost::astar_heuristic<graph_t, int> {
  public:
    AStarHeuristic() {}
    int operator()(vertex_t V){
      return 1;
    }
};

int main(int argv, char* argc[]){

  auto state = State{
    0, 
    {
      {0, {Microchips::Lithium, Microchips::Hydrogen}},
      {1, {Generators::Hydrogen}},
      {2, {Generators::Lithium}},
      {3, {}},
    }
  };

  auto finalState = State{
    3,
    {
      {0, {}},
      {1, {}},
      {2, {}},
      {3, {Microchips::Lithium, Microchips::Hydrogen, Generators::Hydrogen, Generators::Lithium}}
    }
  };
  
  std::cout << "Initial hash: " << hash(state) << " Final hash: " << hash(finalState) << std::endl;
  std::cout << state << std::endl;
  std::cout << finalState << std::endl;

// auto states = generateStates(state);
// for(const auto& s : states){
//   std::cout << s << std::endl;
// }
//states = generateStates(finalState);
// for(const auto& s : states){
//   std::cout << s << std::endl;
// }


 auto G = graph_t();

 auto v = boost::add_vertex(vertex_property_t(boost::white_color), G); 
 auto goalVertex = boost::add_vertex(vertex_property_t(boost::white_color), G);

 auto graphStateMap = boost::get(vertex_state_t(), G);
 graphStateMap[v] = state;
 graphStateMap[goalVertex] = finalState;

 auto vis = AStarVisitor(goalVertex);
 boost::astar_search(G, v, AStarHeuristic(),
     boost::visitor(vis).color_map(boost::get(boost::vertex_color, G)).
     rank_map(boost::get(boost::vertex_rank, G)).
     distance_map(boost::get(boost::vertex_distance, G)).
     predecessor_map(boost::get(boost::vertex_predecessor, G)));


  return 0;
}

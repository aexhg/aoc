#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
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
  static const int Thulium=3;
  static const int Plutonium=4;
  static const int Strontium=5;
  static const int Promethium=6;
  static const int Ruthenium=7;
  static const int Elerium=8;
  static const int Dilithium=9;
};

struct Generators {
  static const int Hydrogen=-Microchips::Hydrogen;
  static const int Lithium=-Microchips::Lithium;
  static const int Thulium=-Microchips::Thulium;
  static const int Plutonium=-Microchips::Plutonium;
  static const int Strontium=-Microchips::Strontium;
  static const int Promethium=-Microchips::Promethium;
  static const int Ruthenium=-Microchips::Ruthenium;
  static const int Elerium=-Microchips::Elerium;
  static const int Dilithium=-Microchips::Dilithium;
};

struct State{
  int liftFloor;
  std::unordered_map<int, std::unordered_set<int>> floorItems;
  int distance = 0;
  std::size_t hash = 0;
  //std::unique_ptr<State> prevState;
};

std::size_t hash(const State& s){
  std::size_t seed = 13; 
  boost::hash_combine(seed, s.liftFloor);
  std::vector<std::pair<int, std::vector<int>>> v;
  for(const auto&kv : s.floorItems){
    auto vv = std::vector<int>(kv.second.begin(), kv.second.end());
    std::sort(vv.begin(), vv.end());
    v.emplace_back(kv.first, std::move(vv));
  }
  std::sort(v.begin(), v.end());
  for(const auto&kv : v){
    boost::hash_combine(seed, kv.first);
    boost::hash_combine(seed, kv.second.size());
    auto v = std::vector<int>(kv.second.begin(), kv.second.end());
    std::sort(v.begin(), v.end());
    for(const auto& i : v){
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
 boost::adjacency_list<boost::vecS, boost::vecS,
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

std::unordered_set<size_t> seenStates;
std::vector<State> generateStates(const State& s){
  std::vector<State> states;

  //const auto thisHash = hash(s);
  for(int c = 1; c <= 2; ++c){
    if(c <= s.floorItems.at(s.liftFloor).size()){
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
            auto newHash = hash(State{newFloor, newFloors});
            if(!seenStates.count(newHash) && validFloors(newFloors.at(newFloor)) &&
                validFloors(newFloors.at(oldFloor))){
             //   newHash != thisHash){
              const auto distance = s.distance + 1;
              states.emplace_back(State{newFloor, newFloors, distance});
              seenStates.emplace(newHash);
            }
          }

        } while(next_combination(items.begin(), items.begin()+c, items.end()));

      }
    }
  }
  return states;
}

std::ostream& operator<<(std::ostream& os, const State& s){
  //os << s.floorItems.size() << std::endl;
  //for(const auto& kv  : s.floorItems){
  std::vector<std::pair<int, std::vector<int>>> v;
   for(const auto&kv : s.floorItems){
     auto vv = std::vector<int>(kv.second.begin(), kv.second.end());
     std::sort(vv.begin(), vv.end());
     v.emplace_back(kv.first, std::move(vv));
  }
  std::sort(v.begin(), v.end(), [](const auto& p, const auto& q){return p.first > q.first;});

  os << "Hash: " << hash(s) << std::endl;
  for(const auto& kv : v){
  //for(int i = 3; i >= 0; --i){
    auto floor = kv.first; 
    auto items = kv.second; //s.floorItems.at(i);
    os << floor << " <> " ;
    if(floor == s.liftFloor){
      os << " E " << " -- " ;
    } else {
      os << "   " << " -- " ;
    }
    for(const auto& it : items){
      os << it << " -- " ;
    }
    os << "\n";
  }

  os << "\nDistance: " << s.distance;
  return os; 
}

bool operator==(const State& s, const State& t){
  return hash(s) == hash(t);
}
struct Found {
};

template<typename Vertex, typename Graph>
class AStarVisitor : public boost::default_astar_visitor {
  public:
    AStarVisitor(Vertex startVertex, Vertex goalVertex, Graph& graph) : G(graph), goalVertex(goalVertex) {
      auto sm = boost::get(vertex_state_t(), G);
      goal = sm[goalVertex];
      auto start = sm[startVertex];
      //std::cout << start << std::endl;
      //std::cout << goal << std::endl;
      seenStates.emplace(hash(start), startVertex);
      seenStates.emplace(hash(goal), goalVertex);
    }
    
    void examine_vertex(Vertex u, const Graph& ){
      
      auto stateMap = boost::get(vertex_state_t(), G);
      auto thisState = stateMap[u];
      if(thisState == goal){
        std::cout << "Found final state: " << thisState << std::endl;
        throw Found();
      }

      //std::cout << thisState << std::endl;
      const auto newStates = generateStates(thisState);
      for(auto& s : newStates){
        auto h = hash(s);
        //if(auto it = seenStates.find(h); it != seenStates.end() && (*it).second  == goalVertex){
        if(seenStates.count(h)){
          auto& v = seenStates[h];
          boost::add_edge(u, v, G);
        //if(s == goal){
          //boost::add_edge(u, goalVertex, 1, G);
        } else {
          auto v = boost::add_vertex(vertex_property_t(boost::white_color), G);
          stateMap[v] = s;
          seenStates.emplace(std::move(h), v);
          boost::add_edge(u, v, 1, G);
        }
        if(s == goal){
          std::cout << "goal in state" << std::endl;
          std::cout << thisState << std::endl;
          //throw Found();
        }
      }
    }
  private:
    Graph& G;
    State goal;
    Vertex goalVertex;
    std::unordered_map<size_t, Vertex> seenStates;
};

int stateDistance(const State& s){
      int n = 0;
      int f= s.floorItems.size(); 
      for(const auto& kv : s.floorItems){
        n += kv.second.size()*(f- kv.first - 1);
      }
      return std::max(0, 2 * (n - f));
 }

class AStarHeuristic : public boost::astar_heuristic<graph_t, int> {
  public:
    AStarHeuristic(const State& goal, const graph_t& G, int topFloor) : 
      goal(goal), G(G), topFloor(topFloor) {}
    int operator()(vertex_t V){
      auto sm = boost::get(vertex_state_t(), G);
      auto state = sm[V];
  
      int n = 0;
      
      for(const auto& kv : state.floorItems){
        n += kv.second.size()*(topFloor - kv.first);
      }
      std::cout << "H   : " << state << " : " << n << std::endl;
      return n;
    }

    const State& goal;
    const graph_t& G;
    int topFloor;
};

int main(int argv, char* argc[]){

  auto state = State{
    0, 
    {
      //{0, {Microchips::Lithium, Microchips::Hydrogen}},
      //{1, {Generators::Hydrogen}},
      //{2, {Generators::Lithium}},
      //{3, {}},
        {0, {Microchips::Thulium, Generators::Thulium, Generators::Plutonium, Generators::Strontium,
              /*Generators::Elerium, Microchips::Elerium, Generators::Dilithium, Microchips::Dilithium*/}},
        {1, {Microchips::Plutonium, Microchips::Strontium}},
        {2, {Generators::Promethium, Microchips::Promethium, Generators::Ruthenium, Microchips::Ruthenium}},
        {3, {}}
    }
  };

  auto finalState = State{
    3,
    {
      {0, {}},
      {1, {}},
      {2, {}},
      {3, {Microchips::Thulium, Generators::Thulium, Microchips::Plutonium, Generators::Plutonium,
            Microchips::Strontium, Generators::Strontium,
            Microchips::Promethium, Generators::Promethium, Microchips::Ruthenium, Generators::Ruthenium,
          /*Microchips::Elerium, Generators::Elerium, Microchips::Dilithium, Generators::Dilithium*/}}
     // {0, {}},
     // {1, {}},
     // {2, {}},
     // {3, {Microchips::Lithium, Microchips::Hydrogen, Generators::Hydrogen, Generators::Lithium}}
    }
  };

  std::cout << "Initial hash: " << hash(state) << " Final hash: " << hash(finalState) << std::endl;
  std::cout << state << std::endl;
  std::cout << finalState << std::endl;


  std::queue<State> q;
  auto stateDistanceQueue = [](const State& s, const State& t){
    return stateDistance(s) > stateDistance(t);
  };
  //kkkstd::priority_queue<State, std::vector<State>, decltype(stateDistanceQueue)> q(stateDistanceQueue);
  q.emplace(state);
  int level = 0;
  std::unordered_map<int, State> hashStates;

  while(!q.empty()){
    auto s = std::move(q.front());
    q.pop();
    
    auto thisHash = hash(s);
    hashStates.emplace(thisHash, s);
    if(s == finalState){
      std::cout << "Found state:" <<std::endl;
      std::cout << s << std::endl;
      std::cout << " Level: " << level << std::endl;
      finalState = s;
      break;
    }
    //std::cout << s << std::endl;
    for(auto & ss: generateStates(s)){
      ///ss.prevState = std::make_unique(s);
      ss.hash = thisHash;
      q.emplace(std::move(ss));
    }
    ++level;

  }

  auto s = finalState.hash;
  auto ihash = hash(state);
  while(s != ihash){
    std::cout << hashStates.at(s) << std::endl;
    s = hashStates.at(s).hash;
  }

  //auto s = finalState;
  //while(s != state){
  //  std::cout << s << std::endl;
  //  s = *s.prevState;
  //}
//  std::cout << " Test " << testState << std::endl;
//   const auto states = generateStates(testState);
//   for(const auto& s : states){
//     std::cout << s << std::endl;
//     const auto statesA = generateStates(s);
//     std::cout << " SUB STATE" << std::endl;
//     for(const auto& ss : statesA){
//       std::cout << ss << std::endl;
//     }
//   }
//   std::cout << " End " << std::endl;
//states = generateStates(finalState);
// for(const auto& s : states){
//   std::cout << s << std::endl;
// }

// auto G = graph_t();
//
// auto v = boost::add_vertex(vertex_property_t(boost::white_color), G); 
// auto goalVertex = boost::add_vertex(vertex_property_t(boost::white_color), G);
//
// auto graphStateMap = boost::get(vertex_state_t(), G);
// graphStateMap[v] = state;
// graphStateMap[goalVertex] = finalState;
//
// auto vis = AStarVisitor<vertex_t, graph_t>(v, goalVertex, G);
// try{
// boost::astar_search(G, v, AStarHeuristic(finalState, G, 3),
//     boost::visitor(vis).color_map(boost::get(boost::vertex_color, G)).
//     rank_map(boost::get(boost::vertex_rank, G)).
//     distance_map(boost::get(boost::vertex_distance, G)).
//     predecessor_map(boost::get(boost::vertex_predecessor, G)));
// } catch(Found& e) {
//
//   std::vector<vertex_t> path;
//   auto pmap = boost::get(boost::vertex_predecessor, G);
//   for(auto v = goalVertex; ;v = pmap[v]){
//     path.push_back(v);
//     if(pmap[v] == v){
//       break;
//     }
//   }
//
//   std::cout << "Path size: " << path.size() << std::endl;
//   for(auto it = path.rbegin(); it != path.rend(); ++it){
//     std::cout << graphStateMap[*it] << std::endl;
//   }
//   
// }


  return 0;
}

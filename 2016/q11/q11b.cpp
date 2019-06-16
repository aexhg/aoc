#include<iostream>
#include<type_traits>
#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/join.hpp>

template<typename E, typename V>
struct Graph{
  std::unordered_map<E, std::vector<V>> edges;

  std::vector<V> neighbours(const V& v) const{
    return neighbours_impl(v);
  }

  void addEdge(const V& v, const V& u){
    if(edges.count(v)){
      edges.at(v).push_back(u);
    } else {
      edges.emplace(v, {u});
    }
  }

  virtual std::vector<V> neighbours_impl(const V& v) const {
    return edges.at(v);
  }

  using vertex_t = V;
  using edge_t = E;
};

template<typename E, typename V, typename S>
struct ImplGraph : public Graph<E, V> {

  ImplGraph(const S& s) : s(s) {}

  std::vector<V> neighbours_impl(const V& v) const override {
    auto newStates = std::move(s(v));
    return newStates;
  }

  S s;
};

template<typename G, typename V>
auto bfs(const G& g, const typename G::vertex_t& start, const typename G::vertex_t& goal, const V& visitor){

  using vertex_t = typename G::vertex_t;
  std::unordered_set<vertex_t> visited;
  std::queue<vertex_t> q;
  q.push(start);
  visited.emplace(start);

  std::unordered_map<vertex_t, vertex_t> path;
  path[start] = start;

  while(!q.empty()){
    auto v = std::move(q.front());
    q.pop();
    visitor(v);
    if(v == goal){
      return path;
    }
    for(const auto& n : g.neighbours(v)){
      if(!visited.count(n)){
        q.push(n);
        visited.emplace(n);
        path[n] = v;
      }
    }
  }
  return path;
}

template<typename G, typename V, typename C, typename H>
auto astar(const G& g, const typename G::vertex_t& start,
    const typename G::vertex_t& goal, const V& visitor,
    const C& costFunction, const H& heuristic){

  using vertex_t = typename G::vertex_t;
  using cost_t = typename std::result_of<C(vertex_t,vertex_t)>::type;
  using heuristic_t = cost_t;//std::result_of<H(vertex_t,vertex_t)>::type;
  using pair_t = std::pair<heuristic_t, vertex_t>;
  std::priority_queue<pair_t, std::vector<pair_t>, std::greater<pair_t>> q;
  q.push({heuristic_t(), start});

  std::unordered_map<vertex_t, vertex_t> path;
  std::unordered_map<vertex_t, cost_t> cost;
  path[start] = start;
  cost[start] = cost_t();

  while(!q.empty()){
    auto [p, v] = std::move(q.top());
    q.pop();
    visitor(v);
    if(v == goal){
      return path;
    }
    for(const auto& n : g.neighbours(v)){
      auto newcost = cost.at(v) + costFunction(v, n);
      if(!cost.count(n) || newcost < cost.at(n)){
        cost[n] = newcost;
        auto h = heuristic(n, goal);
        q.push({newcost + h, n});
        path[n] = v;
      }
    }
  }
  return path;
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
  State(int liftFloor, 
      std::map<int, std::list<int>> const& floorItems) :
    liftFloor(liftFloor),
    floorItems(floorItems),
    iid(hash()) {}

  auto getId() const { 
    return iid;
  }

 public:
  const int liftFloor;
  const std::map<int, std::list<int>> floorItems;
 private:
  const std::size_t iid = 0;

  std::size_t hash(){
    std::size_t seed = 13; 
    boost::hash_combine(seed, liftFloor);
    for(auto& kv : floorItems){
      auto floor = kv.first;
      auto items = kv.second;
      items.sort();
      boost::hash_combine(seed, floor);
      boost::hash_combine(seed, items.size());
      for(auto& i : items){
        boost::hash_combine(seed, i);
      }
    }
    return seed;
  }
};

std::ostream& operator<<(std::ostream& os, const State& s){

  os << "Hash: " << s.getId() << std::endl;
  for(auto it = s.floorItems.rbegin(); it != s.floorItems.rend(); ++it ){
    auto floor = (*it).first; 
    auto items = (*it).second; //s.floorItems.at(i);
    items.sort();
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

  return os; 
}

bool validFloors(const std::list<int>& f){

  if(std::all_of(f.begin(), f.end(), [](auto i){return i>0;})){
    return true;
  } else if(std::all_of(f.begin(), f.end(), [](auto i){return i < 0;})){
    return true;
  } else {
    for(auto i : f){
      if(i > 0 && (std::find(f.begin(), f.end(), -i) == f.end())){
        return false;
      }
    }
  }
  return true;
}

std::vector<State> generateStates(const State& s){
  std::vector<State> states;

  for(int c = 1; c <= 2; ++c){
    const int nFloorItems = s.floorItems.at(s.liftFloor).size();
    if(c <= nFloorItems){
      const int oldFloor= s.liftFloor;
      for(int f = -1; f <=1; f += 2){
        
        const int newFloor = oldFloor+f;
        const int nFloors = s.floorItems.size();
        if(newFloor>=0 && newFloor<nFloors){
          std::vector<int> choice(nFloorItems, 0);
          std::fill(choice.end() - c, choice.end(), 1);
          do {
            std::map<int, std::list<int>> newFloors(s.floorItems);
            auto items = s.floorItems.at(oldFloor);
            for(auto [k, it] = std::make_tuple(int(), items.begin()); it != items.end(); ++k, ++it) {
              if(choice[k]){
                newFloors[newFloor].push_back(*it);
                newFloors[oldFloor].remove(*it);
             }
            }

            if(validFloors(newFloors.at(newFloor)) &&
               validFloors(newFloors.at(oldFloor))){
               states.emplace_back(State{newFloor, newFloors});
            }
          } while(std::next_permutation(choice.begin(), choice.end()));
        }
      }
    }
  }
  return states;
}

int main(int argv, char* agrc[]){
  auto initialState = State{
    0, 
    {
 //     {0, {Microchips::Lithium, Microchips::Hydrogen}},
 //     {1, {Generators::Hydrogen}},
 //     {2, {Generators::Lithium}},
 //     {3, {}},
        {0, {Microchips::Thulium, Generators::Thulium, Generators::Plutonium, Generators::Strontium,
              Generators::Elerium, Microchips::Elerium, Generators::Dilithium, Microchips::Dilithium}},
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
         Microchips::Elerium, Generators::Elerium, Microchips::Dilithium, Generators::Dilithium}}
//      {0, {}},
//      {1, {}},
//      {2, {}},
//      {3, {Microchips::Lithium, Microchips::Hydrogen, Generators::Hydrogen, Generators::Lithium}}
    }
  };

  std::unordered_map<int, State> stateMap;

  std::cout << initialState << " to\n " << finalState << std::endl;

  stateMap.emplace(initialState.getId(), initialState);

  auto lamGenerateStates = [&stateMap](auto h){
    auto states = generateStates(stateMap.at(h));
    std::vector<std::size_t> newStates;
    newStates.reserve(states.size());
    for(auto & s : states){
      if(!stateMap.count(s.getId())){
        stateMap.emplace(s.getId(), s);
        newStates.push_back(s.getId());
      }
    }
    return newStates;
  };

  ImplGraph<std::size_t, std::size_t, decltype(lamGenerateStates)> G(lamGenerateStates);
  
  int nvisits = 0;
  auto visitor = [&nvisits](const auto& v){
    ++nvisits;
    //std::cout << stateMap.at(v) << std::endl;
  };

  auto cost = [](const auto& v, const auto& u){
    if(u == v){
      return 0;
    } else {
      return 1;
    }
  };

  auto heuristic = [&stateMap](const auto& v, const auto& u){
    if(v == u){
      return 0;
    } else {
      int n = 0;
      auto s = stateMap.at(v);
      int f = s.floorItems.size(); 
      for(const auto& kv : s.floorItems){
        n += kv.second.size()*(f - kv.first - 1);
      }
      return n*3;
      //return 0;//-(int)stateMap.at(v).floorItems.at(3).size() * 10;
 
    }
  };

  auto path = astar(G, initialState.getId(), finalState.getId(), visitor, cost, heuristic);
  std::vector<State> pathv;
  pathv.reserve(path.size());

  auto current = finalState.getId();
  while(current != initialState.getId()){
    auto s = stateMap.at(current);
    pathv.push_back(s);
    current = path.at(current);
  }

  pathv.push_back(initialState);

  std::cout << "Size: " << pathv.size()  - 1 << " with n states: " << nvisits << std::endl;
  for(auto it = pathv.rbegin(); it != pathv.rend(); ++it){
    std::cout << *it << std::endl;
    std::cout << "H() " << heuristic(it->getId(), finalState.getId()) << std::endl;
  }

//  auto visitor = [](const char& c){
//    std::cout << c << std::endl;
//  };
//  Graph<char, char> G{{
//    {'A', {'B'}},
//    {'B', {'A', 'C', 'D'}},
//    {'C', {'A'}},
//    {'D', {'E', 'A'}},
//    {'E', {'B'}}
//  }};
//
//  bfs(G, 'A', 'E', visitor);
}

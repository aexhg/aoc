#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct item{
	std::string name;
	int cost;
	int damage;
	int armour;
};

struct player{
	int damage;
	int hitpoints;
	int armour;

	void attack(const player& p){
		int hit = std::max(p.damage - armour, 1);
		hitpoints -= hit;
	}

	bool alive() const {
		return hitpoints > 0;
	}
};

std::ostream& operator<<(std::ostream& out, const player& p){
	out << std::endl;
	out << "\tDamage: " << p.damage << std::endl;
	out << "\tHitPoints: " << p.hitpoints << std::endl;
	out << "\tArmour: " << p.armour;
	return out;
}

std::ostream& operator<<(std::ostream& out, const item& i){
	out << std::endl;
	out << "\tName: " << i.name << std::endl;
	out << "\tCost: " << i.cost << std::endl;
	out << "\tDamage: " << i.damage << std::endl;
	out << "\tArmour: " << i.armour;
	return out;
}

int main(int argv, char* argc[]){

	const std::vector<item>  weapons{
		{"Dagger",        8,     4,       0},
		{"Shortsword",   10,     5,       0},
		{"Warhammer",    25,     6,       0},
		{"Longsword",    40,     7,       0},
		{"Greataxe",     74,     8,       0},
	};

	const std::vector<item> armours{
		{"None", 	 0 ,     0,       0},
		{"Leather",      13,     0,       1},
		{"Chainmail",    31,     0,       2},
		{"Splintmail",   53,     0,       3},
		{"Bandedmail",   75,     0,       4},
		{"Platemail",   102,     0,       5},
	};

	const std::vector<item> rings1{
		{"None",        0   ,   0  ,     0},
		{"None",        0   ,   0  ,     0},
		{"Damage+1",    25  ,   1  ,     0},
		{"Damage+2",    50  ,   2  ,     0},
		{"Damage+3",   100  ,   3  ,     0},
		{"Defense+1",   20  ,   0  ,     1},
		{"Defense+2",   40  ,   0  ,     2},
		{"Defense+3",   80  ,   0  ,     3},
	};

	player boss{8, 109, 2};
	player player1{0, 100, 0};
	
	auto playRound = [](player& p1, player &p2){
			
		for(int rounds=0; p1.alive() && p2.alive(); rounds++){
			player& p = (rounds % 2) == 0 ? p1 : p2;
			player& o = (rounds % 2) == 0 ? p2 : p1;
			p.attack(o);
		}
	};

	int minCost = std::numeric_limits<int>::max();
	int maxCost = std::numeric_limits<int>::min();
	std::tuple<item, item, item, item> items;
	std::tuple<player, player> players;

	std::tuple<item, item, item, item> itemsmax;
	std::tuple<player, player> playersmax;

	for(const auto w : weapons){
		for(int j = 0; j <  rings1.size(); ++j){
			for(int k = j+1; k < rings1.size() ; ++k){
				for(const auto a : armours){
	
					const auto r1 = rings1[j];
					const auto r2 = rings1[k];
	
					int totalCost = w.cost + a.cost + r1.cost + r2.cost;
					auto p1 = player1; 
					auto p2 = boss;
	
					p1.damage += (w.damage + r1.damage + r2.damage);
					p1.armour += (a.armour + r1.armour + r2.armour);
	
					playRound(p1, p2);
	
					if(p1.alive() && totalCost < minCost){
						minCost = totalCost;
						items = std::make_tuple(w, a, r1, r2);
						players = std::make_tuple(p1, p2);
					}
					if(p2.alive() && totalCost > maxCost){
						maxCost = totalCost;
						itemsmax = std::make_tuple(w, a, r1, r2);
						playersmax = std::make_tuple(p1, p2);
					}
				}
			}
		}
	}

	std::cout << std::get<0>(players) << std::endl;
	std::cout << std::get<1>(players) << std::endl;

	std::cout << std::get<0>(items) << std::endl;
	std::cout << std::get<1>(items) << std::endl;
	std::cout << std::get<2>(items) << std::endl;
	std::cout << std::get<3>(items) << std::endl;
	std::cout << "Total Cost: " << 
		std::get<0>(items).cost +
		std::get<1>(items).cost +
		std::get<2>(items).cost +
		std::get<3>(items).cost 
		<< std::endl;
	
	std::cout << std::get<0>(playersmax) << std::endl;
	std::cout << std::get<1>(playersmax) << std::endl;
	
	std::cout << std::get<0>(itemsmax) << std::endl;
	std::cout << std::get<1>(itemsmax) << std::endl;
	std::cout << std::get<2>(itemsmax) << std::endl;
	std::cout << std::get<3>(itemsmax) << std::endl;
	std::cout << "Total Cost: " << 
		std::get<0>(itemsmax).cost +
		std::get<1>(itemsmax).cost +
		std::get<2>(itemsmax).cost +
		std::get<3>(itemsmax).cost 
		<< std::endl;
	
	return 0;
}

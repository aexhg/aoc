#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include <random>

struct player;


struct spell{
	std::string name;
	int cost;
	int time;
	std::function<void (player& p, int time)> applyPlayer;
	std::function<void (player& p, int time)> applyOpp;
};


struct player{
	int damage;
	int hitpoints;
	int armour;
	int mana;

	void battle(player& p){
		int hit = std::max(p.damage - armour, 1);
		hitpoints -= hit;
	}

	bool alive() const {
		return hitpoints > 0;
	}

	void dead() {
		hitpoints = -1;
	}
};

std::ostream& operator<<(std::ostream& out, const player& p){
	out << std::endl;
	out << "\tDamage: " << p.damage << std::endl;
	out << "\tHitPoints: " << p.hitpoints << std::endl;
	out << "\tArmour: " << p.armour << std::endl;
	out << "\tMana: " << p.mana;
	return out;
}

std::ostream& operator<<(std::ostream& out, const spell& s){
	out << std::endl;
	out << "\tName: " << s.name << std::endl;
	out << "\tCost: " << s.cost << std::endl;
	out << "\tTime: " << s.time<< std::endl;
	return out;
}

int main(int argv, char* argc[]){

	static const int N = argv >= 2 ? std::stoi(argc[1]) : 100000;
	static const bool DEBUG = argv >= 5 ? true : false;
	static const int seed = argv >= 4 ? std::stoi(argc[3]) : 7;
	static const bool hard = argv >= 3 && *argc[2] == 't' ? true : false;
	if(hard){
		std::cout << "HARD mode" << std::endl;
	}
	const std::vector<spell>  spells{
		{
			"Magic Missle",
			53,
			0,
			nullptr,
			[](player& p, int time){
				p.hitpoints -= 4;
			}
		},
		{
			"Shield",
			113,
			6,
			[](player& p, int time){
			  	if(time>0){
					p.armour = 7;
			  	}
			  	else{
				  	p.armour = 0;
			  	}},
			nullptr
		},
		{
			"Drain",
			73,
			0,
			[](player& p, int time){
				p.hitpoints += 2;
			},
			[](player& p, int time){
				p.hitpoints -= 2;
			}
		},
		{
			"Poison",
			173,
			6,
			nullptr,
			[](player& p, int time){
				p.hitpoints -= 3;
			}
		},
	       	{
			"Recharge",
			229,
			5,
			[](player& p, int time){
				p.mana += 101;
			},
			nullptr
		},
	};
	
	const player boss{8, 55, 0, 0};
	const player player1{0, 50, 0, 500};

	auto applySpells = [debug = DEBUG](player& p, player& b, std::vector<spell>& spells){

		//for(auto& s : spells){
		//	if(s.name == "Shield" && s.time == 0){
		//		s.applyPlayer(p, 0);
		//	}
		//}		

		for(auto& s : spells){
			if(debug){
				std::cout << "Apply spell: " << s << std::endl;
			}
			if(s.applyPlayer){
				s.applyPlayer(p, s.time);
			}
			if(s.applyOpp){
				s.applyOpp(b, s.time);
			}
			s.time -= 1;
		}


		auto begin = std::remove_if(
					spells.begin(),
					spells.end(),
					[](const spell& s){ return s.time <= 0; });

		if(begin != spells.end()){
			for(auto it = begin; it != spells.end(); ++it){
				if(debug){
					std::cout << "Delete: " << it->name << std::endl;
				}
				if(it->name == "Shield"){
					it->applyPlayer(p, 0);
				}
			}
			spells.erase(begin, spells.end());
		}
	};

	std::default_random_engine gen(seed);
	std::uniform_int_distribution<int> dist(0,4);
	
	auto chooseSpell = [&spells, &dist, &gen](std::vector<spell>& activeSpells){
		for(;;){
			int ispell = dist(gen);
			auto sp = spells[ispell];
			if(auto it = std::find_if(
						activeSpells.begin(),
						activeSpells.end(),
						[&sp](const spell& s){
							return s.name == sp.name;
						}); it == activeSpells.end()){
				return sp;
			}
		}
	};

	auto applyRound = [debug = DEBUG, &spells, &applySpells, &chooseSpell, hard = hard](
			player& player1,
			player& boss, 
			std::vector<spell>& activeSpells){
		int totalCost = 0;
		for(int k = 0; player1.alive() && boss.alive(); k++){

			if(debug){
				std::cout << "k=" << k << std::endl;
			}
			if(hard){
				player1.hitpoints -= 1;
				if(!player1.alive()){
					return std::make_tuple(false, totalCost);
				}
			}
			applySpells(player1, boss, activeSpells);
			if((k%2)==0){
				auto spell = chooseSpell(activeSpells);
				int cost = spell.cost;
				if(player1.mana >= cost){
					if(debug){
						std::cout << "Cast spell: " << spell.name << std::endl;
					}
					totalCost += cost;
					player1.mana -= cost;
					activeSpells.push_back(spell);
				}
				else{
					if(debug){
						std::cout << "Unable to cast: " << spell << std::endl;
					}
					return std::make_tuple(false, totalCost);
				}
			}
			else{
				player1.battle(boss);
			}

			if(debug){
				std::cout << "Player: " << player1 << std::endl;
				std::cout << "Boss: " << boss << std::endl;
			}
		}
		return std::make_tuple(player1.alive(), totalCost);
	};

	int minCost = std::numeric_limits<int>::max();
	for(int i = 0; i < N; ++i){
		auto p = player1;
		auto b = boss;
		std::vector<spell> activeSpells;
		activeSpells.reserve(1000);
		auto outcome = applyRound(p, b, activeSpells);
		if(std::get<0>(outcome) && std::get<1>(outcome) < minCost){
			minCost = std::get<1>(outcome);
		}
	}

	std::cout << "Min cost: " << minCost << std::endl;

	return 0;
}

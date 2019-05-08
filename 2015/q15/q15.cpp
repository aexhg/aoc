#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

struct Ingredient{
	std::string name;
	int capacity;
	int durability;
	int flavor;
	int texture;
	int calories;
};

std::ostream& operator<<(std::ostream& out, const Ingredient& i){
	out << "Capacity: " << i.capacity << " " << 
		"Durability: " << i.durability << " "  << 
		"Flavor: " << i.flavor << " "  << 
		"Texture: " << i.texture << " "  << 
		"Calories: " << i.calories << std::endl;
	return out;
}

int weight(const std::vector<Ingredient>& ingredients, const std::vector<int>& weights){

	int calories = 0;
	int totalWeight = 0;
	std::vector<int> ww(4, 0);

	for(int i = 0; i < ingredients.size(); ++i){
		const auto w = weights[i];
		const auto ing = ingredients[i];
		ww[0] += w * ing.capacity;
		ww[1] += w * ing.durability;
		ww[2] += w * ing.flavor;
		ww[3] += w * ing.texture;
		calories += w * ing.calories;
	}

	return calories == 500 ?
		std::max(0, ww[0]) * std::max(0, ww[1]) * std::max(0, ww[2]) * std::max(0, ww[3]) :
		0;
}

int main(int argv, char* argc[]){

	const static std::regex r("^(\\w+): capacity (-?\\d+), durability (-?\\d+), flavor (-?\\d+), texture (-?\\d+), calories (-?\\d+)$");

	std::vector<Ingredient> ingredients;
	for(auto [line, is] = std::make_pair(std::string{}, std::ifstream(argc[1])); std::getline(is, line);){
		if(std::smatch match; std::regex_match(line, match, r)){
			std::string ingredient = match.str(1);
			int capacity = std::stoi(match.str(2));
			int durability = std::stoi(match.str(3));
			int flavor = std::stoi(match.str(4));
			int texture = std::stoi(match.str(5));
			int calories = std::stoi(match.str(6));
			ingredients.emplace_back(Ingredient{
					ingredient,
					capacity,
					durability,
					flavor,
					texture,
					calories});
			std::cout << ingredients.back() << std::endl;
		}
	}

	static const int N = 100;
	int score = 0;

	std::vector<int> weights(4);
	for(int i = 0; i <= 100  ; ++i){
		for(int j = 0; j <= 100-i; ++j){
			for(int k = 0; k <= 100-i-j; ++k){
				weights[0] = i;
				weights[1] = j;
				weights[2] = k;
				weights[3] = 100 - i - j - k;
				score = std::max(score, weight(ingredients, weights));
			}
		}
	}

	std::cout << "Best score: " << score << std::endl;

	return 0;
}

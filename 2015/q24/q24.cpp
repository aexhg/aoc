#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>


int main(int argv, char* argc[]) {

	std::vector<int> weights = {
		1,
		3,
		5,
		11 ,
		13 ,
		17 ,
		19 ,
		23 ,
		29 ,
		31 ,
		37 ,
		41 ,
		43 ,
		47 ,
		53 ,
		59 ,
		67 ,
		71 ,
		73 ,
		79 ,
		83 ,
		89 ,
		97 ,
		101 ,
		103 ,
		107 ,
		109 ,
		113 ,
	};

	const int N = std::accumulate(weights.begin(), weights.end(), 0) / 4;

	std::cout << "N=" << N << std::endl;
	std::vector<int> combos(weights.size(), 0);
	std::vector<std::pair<int, long long>> qe;
	qe.reserve(10000);

	bool found = false;

	for (int i = 2; i < weights.size()-1 && !found; ++i) {

		std::fill(combos.begin(), combos.end(), 1);
		std::fill(combos.begin(), combos.end() - i , 0);

		do {
			int weight = 0;
			long long qes = 1;
			for (int j = 0; j < weights.size(); ++j) {
				if (combos[j]) {
					weight += weights[j];
					qes *= weights[j];
				}
			}
			if (weight == N) {
				qe.push_back({
					i,
					qes });
				found = true;
			}

		} while (std::next_permutation(combos.begin(), combos.end()));
	}

	std::sort(qe.begin(), qe.end());
	std::cout << "Combo: " << qe[0].first << " : " << qe[0].second << std::endl;
	return 0;
}

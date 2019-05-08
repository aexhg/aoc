#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <experimental/algorithm>
class Reindeer{
	public:
		Reindeer(const std::string& name, int flySpeed, int flyTime, int restTime):
			_name(name),
			_flySpeed(flySpeed),
			_flyTime(flyTime),
			_restTime(restTime),
			_nextFlyTime(0),
			_nextRestTime(0),
			_distance(0),
			_points(0)
	 	{ }

		int distance() const {
			return _distance;
		}
		
		const std::string name() const {
			return _name;
		}

		int nextFlyTime() const {
			return _nextFlyTime;
		}

		int nextRestTime() const {
			return _nextRestTime;
		}

		const std::string print() const {
			return _name + ": " + 
				"distance: " + std::to_string(_distance) + " " + 
				"next fly time: "  + std::to_string(_nextFlyTime) + " "  + 
				"next rest time: " + std::to_string(_nextRestTime) + " " + 
				"points: " + std::to_string(_points);
		}

		void increment(){
			if(_nextFlyTime < _flyTime){
				_distance += _flySpeed;
				_nextFlyTime++;
				return;
			}

			if(_nextRestTime < _restTime){
				_nextRestTime++;
				return;
			}
			_nextFlyTime = 0;
			_nextRestTime = 0;

			_distance += _flySpeed;
			_nextFlyTime++;
		}

		void addPoint(){
			_points++;
		}

	private:
		const std::string _name;
		const int _flySpeed;
		const int _flyTime;
		const int _restTime;
		int _nextFlyTime;
		int _nextRestTime;
		int _distance;
		int _points;
};

int main(int argv, char* argc[]){

	const static std::regex r("^(\\w+) can fly (\\d+) km/s for (\\d+) seconds, but then must rest for (\\d+) seconds.$");
	std::vector<Reindeer> reindeers;
	const int totalFlyTime = std::stoi(argc[2]);

	for(auto [line, is] = std::make_pair(std::string(), std::ifstream(argc[1])); std::getline(is, line); ){

		std::smatch match;
		if(std::regex_match(line, match, r)){
			const std::string name = match.str(1);
			const int flySpeed = std::stoi(match.str(2));
			const int flyTime = std::stoi(match.str(3));
			const int restTime = std::stoi(match.str(4));
			reindeers.push_back(Reindeer(name, flySpeed, flyTime, restTime));
		}
	}
	
	for(int t = 0; t < totalFlyTime; ++t){
		for(auto& v : reindeers){
			v.increment();
			//std::cout << v.name() << " " << v.distance() << std::endl;
		}

		int maxdist = reindeers[0].distance();
		for(int i = 1; i < reindeers.size(); ++i){
			if(reindeers[i].distance() >= maxdist){
				maxdist = reindeers[i].distance();
			}
		}

		for(auto& r : reindeers){
			if(r.distance() >= maxdist){
				r.addPoint();
			}
		}
	}
	
	for(const auto &v : reindeers){
		std::cout << v.print() << std::endl;
	}

	return 0;
}

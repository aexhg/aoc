#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <functional>
#include <unordered_map>

int main(int argv, char* argc[]) {

	const static std::regex r(
		"^((hlf|tpl|inc) (a|b))|(jmp ((\\+|-)?(\\d+)))|(jio (a|b), ((\\+|-)?(\\d+)))|(jie (a|b), ((\\+|-)?(\\d+)))$"
	);

	std::unordered_map<std::string, std::function<void()>> functionMapOne;
	std::unordered_map<std::string, std::function<int()>> functionJmp;
	std::unordered_map<int, std::string> instructions;
	std::unordered_map<std::string, unsigned int> registers{
		{ "a", 1 },
	{ "b", 0 },
	};

	for (auto[line, is, i] = std::make_tuple(std::string(), std::ifstream(argc[1]), int());
		std::getline(is, line); ++i) {
		if (std::smatch match; std::regex_match(line, match, r)) {
			if (match[1].matched) {
				//		std::cout << match.str(2) << " : " << match.str(3) << std::endl;
				auto l = [instruction = match.str(2), &registers, reg = match.str(3)]() {
					if (instruction == "hlf") {
						registers[reg] /= 2;
					}
					else if (instruction == "tpl") {
						registers[reg] *= 3;
					}
					else if (instruction == "inc") {
						registers[reg]++;
					}
				};
				functionMapOne[match.str(1)] = l;
				instructions[i] = match.str(1);
			}
			if (match[4].matched) {
				//		std::cout << "jmp " << match.str(5) << std::endl
				auto j = [jump = std::stoi(match.str(5))]() {
					return jump;
				};
				functionJmp[match.str(4)] = j;
				instructions[i] = match.str(4);
			}
			if (match[8].matched) {
				//		std::cout << "jio " << match.str(9) << " : " << match.str(10) << std::endl;
				auto j = [jump = std::stoi(match.str(10)), &registers, reg= match.str(9)]() {
					if (registers.at(reg) == 1) {
						return jump;
					}
					return 1;
				};
				functionJmp[match.str(8)] = j;
				instructions[i] = match.str(8);
			}
			if (match[13].matched) {
				//		std::cout << "jie " << match.str(14) << " : " << match.str(15) << std::endl;
				auto j = [jump = std::stoi(match.str(15)), &registers, reg = match.str(14)]() {
					if ((registers.at(reg) % 2) == 0) {
						return jump;
					}
					return 1;
				};
				functionJmp[match.str(13)] = j;
				instructions[i] = match.str(13);
			}
		}
	}

	const int size = instructions.size();

	for (int i = 0; ;) {
		const std::string instruction = instructions.at(i);
		//std::cout << instruction << std::endl;
		if (functionMapOne.count(instruction)) {
			functionMapOne.at(instruction)();
			++i;
		}
		else if (functionJmp.count(instruction)) {
			i += functionJmp.at(instruction)();
		}
		if (i >= size) {
			break;
		}
	}


	for (const auto& kv : registers) {
		std::cout << kv.first << " : " << kv.second << std::endl;
	}

	return 0;
}

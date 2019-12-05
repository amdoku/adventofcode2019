#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
std::vector<T> readInput(const std::string &path) {
	std::ifstream input(path);
	std::istream_iterator<T> start(input), end;
	std::vector<T> data(start, end);
	input.close();
	return data;
}

int32_t calculateFuel(const int32_t mass) {
	return (mass / 3) - 2;
}

int32_t calculateFuelForFuel(const int32_t fuelMass) {
	if (fuelMass <= 0) return 0;
	return fuelMass + calculateFuelForFuel(calculateFuel(fuelMass));
}

int32_t calculateFuelRequired(const int32_t mass) {
	auto fuelForMass = calculateFuel(mass);
	auto fuelForFuel = calculateFuelForFuel(fuelForMass);
	return fuelForFuel;
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cout << "Expected an argument." << std::endl;
		return -2;
	}

	auto masses = readInput<int32_t>(argv[1]);
	auto totalFuel = 0;
	for(auto&& mass : masses) {
		totalFuel += calculateFuelRequired(mass);
	}
	std::cout << "Total fuel: " << totalFuel << std::endl;
}

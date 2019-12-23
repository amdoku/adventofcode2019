#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <utility>
#include <set>
#include <iostream>
#include <algorithm>
#include <iterator>

#include "blackMagicGenerator.hpp"
#include "common_types.hpp"

// comma seperated template code from: https://stackoverflow.com/a/21838123
template<class T, char sep=','>
struct comma_sep { //type used for temporary input
	T t; //where data is temporarily read to
	operator const T&() const {return t;} //acts like an int in most cases
};
template<class T, char sep>
std::istream& operator>>(std::istream& in, comma_sep<T,sep>& t)
{
	if (!(in >> t.t)) //if we failed to read the int
		return in; //return failure state
	if (in.peek() == sep) //if next character is a comma
		in.ignore(); //extract it from the stream and we're done
	else //if the next character is anything else
		in.clear(); //clear the EOF state, read was successful
	return in; //return
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> t) {
	out << "[" << t.size() << "| ";
	std::copy(t.cbegin(), t.cend(), std::ostream_iterator<T>(out, ","));
	out << (t.size() > 0 ? "\b]" : "]");
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T> t) {
	out << "[" << t.size() << "| ";
	std::copy(t.cbegin(), t.cend(), std::ostream_iterator<T>(out, ","));
	out << (t.size() > 0 ? "\b]" : "]");
	return out;
}

std::ostream& operator<<(std::ostream& out, const BlackMagicGenerator& t) {
	return t.operator<<(out);
}

std::istream& operator>>(std::istream& in, Direction& t) {
	if (!(in >> t.c))
		return in;
	in.clear();
	return in;
}

std::istream& operator>>(std::istream& in, WireToken& t) {
	if (!(in >> t.dir >> t.length))
		return in;
	in.clear();
	return in;
}

template <typename T>
std::vector<T> readTokens(std::string& line) {
	std::stringstream in(line);
	std::istream_iterator<comma_sep<WireToken>> start(in), end;
	std::vector<T> tokens{start, end};
	return tokens;
}

template <typename T, typename P>
std::vector<T> readInput(const std::string &path) {
	std::ifstream input{path};
	std::vector<T> data;
	std::string line;
	while(std::getline(input, line)) {
		std::vector<P> tokens = readTokens<P>(line);
		data.push_back(tokens);
	}
	input.close();
	return data;
}

std::set<PuzzleCount> explodeIntoPoints(std::vector<WireToken> &vec) {
	BlackMagicGenerator begin(vec.cbegin(), vec.cend()), end;
	return std::set<PuzzleCount>(begin, end);
}

auto& log() {
	return std::cout;
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cout << "Expected an argument.\n";
		return -2;
	}
	// read lines of file
	// per line create a vector of { orientation, distance } (rad coords)
	auto data = readInput<std::vector<WireToken>, WireToken>(argv[1]);
	log() << "Wires found: " << data.size() << "\n";

	auto wire1 = data.at(0);
	auto wire2 = data.at(1);

	auto wire1Coordinates = explodeIntoPoints(wire1);
	auto wire2Coordinates = explodeIntoPoints(wire2);

	// build set of equidistant points for each wire (points on a grid)
	// find intersectio points
	std::vector<std::pair<PuzzleCount, PuzzleCount>> crossOvers;
	auto intersectionLambda = [&crossOvers, &wire2Coordinates](auto&& t) -> void {
		if (t.c.x == 0 && t.c.y == 0) return; // remove origin

		auto crossOver = wire2Coordinates.find(t);
		if (crossOver != wire2Coordinates.end()) {
			crossOvers.push_back(std::make_pair(t, *crossOver));
		}
	};
	std::for_each(wire1Coordinates.cbegin(), wire1Coordinates.cend(), intersectionLambda);

	log() << "crossOvers size: " << crossOvers.size() << std::endl;
	log() << crossOvers << std::endl;

	// calculate manhattan distances
	auto manhattanLambda = [](const std::pair<PuzzleCount, PuzzleCount>& t) -> auto {
		return std::abs(t.first.c.x) + std::abs(t.first.c.y);
	};
	std::set<int> manhattanDistances;
	std::transform(crossOvers.cbegin(), crossOvers.cend(),
		std::inserter(manhattanDistances, manhattanDistances.begin()),
		manhattanLambda);

	// calculate the other distance
	auto otherDistanceLambda = [](const std::pair<PuzzleCount, PuzzleCount>& t) -> int {
		return t.first.step + t.second.step;
	};
	std::set<int> otherDistances;
	std::transform(crossOvers.cbegin(), crossOvers.cend(),
		std::inserter(otherDistances, otherDistances.begin()),
		otherDistanceLambda);


	auto solution = *std::min_element(manhattanDistances.begin(), manhattanDistances.end());
	auto solution2 = *std::min_element(otherDistances.begin(), otherDistances.end());

	// find min with respect to manhattan distance to origin point
	log() << "Solution part1: " << solution
			<< "\nSolution part2: " << solution2 << std::endl;
	// ???
	// Bacon & Profit
}

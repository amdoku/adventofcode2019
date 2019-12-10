#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <utility>
#include <set>
#include <iostream>
#include <algorithm>

#include "blackMagicIterator.hpp"
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

std::ostream& operator<<(std::ostream& out, const BlackMagicIterator& t) {
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

std::set<Coordinate> explodeIntoPoints(std::vector<WireToken> &vec) {
	BlackMagicIterator begin(vec.cbegin(), vec.cend()), end;
	return std::set<Coordinate>(begin, end);
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
	// find intersection points
	std::set<Coordinate> crossOvers;
	std::set_intersection(wire1Coordinates.cbegin(), wire1Coordinates.cend(),
						wire2Coordinates.cbegin(), wire2Coordinates.cend(),
						std::inserter(crossOvers, crossOvers.begin()));

	log() << "crossOvers size: " << crossOvers.size() << std::endl;
	log() << crossOvers << std::endl;

	auto manhattanLambda = [](const Coordinate& t) -> int {
		return std::abs(t.x) + std::abs(t.y);
	};
	std::set<int> manhattanDistances;

	std::transform(crossOvers.cbegin(), crossOvers.cend(),
		std::inserter(manhattanDistances, manhattanDistances.begin()),
		manhattanLambda);

	// find min with respect to manhattan distance to origin point
	log() << "Solution: " << *(++std::min_element(manhattanDistances.begin(), manhattanDistances.end())) << std::endl;
	// ???
	// Bacon & Profit
}

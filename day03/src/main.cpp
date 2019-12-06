#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>

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
	out << "[" << t.size() << "|";
	std::copy(t.cbegin(), t.cend(), std::ostream_iterator<T>(out, ","));
	out << "\b]";
	return out;
}

struct Direction {
	char c;
};

struct WireToken {
	struct Direction dir;
	int length;
};

std::ostream& operator<<(std::ostream& out, const Direction& t) {
	return out << t.c;
}

std::ostream& operator<<(std::ostream& out, const WireToken& t) {
	return out << "(" << t.dir << "," << t.length << ")";
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
	log() << "size: " << data.size() << "\n";
	for(auto&& wire : data) {
		log() << wire << "\n";
	}
	// build set of equidistant points for each wire (points on a grid)
	// find intersection points
	// find min with respect to manhattan distance to origin point
	// ???
	// Bacon & Profit.tTtzW
}

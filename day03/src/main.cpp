#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <type_traits>

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

template <typename T, typename P>
std::vector<T> readInput(const std::string &path) {
	std::ifstream input{path};
	std::istream_iterator<P> start{input}, end;
	std::vector<T> data{start, end};
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
	// build set of equidistant points for each wire
	// find intersection points
	// find min with respect to manhattan distance to origin point
	// ???
	// Bacon & Profit.
}

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
	std::ifstream input(path);
	std::istream_iterator<P> start(input), end;
	std::vector<T> data(start, end);
	input.close();
	return data;
}

auto& log() {
	return std::cout;
}

typedef int(*binary_op)(int, int); // signature for all valid template params

template<typename T>
T add(T a, T b) {
	return a + b;
}

template<typename T>
T mul(T a, T b) {
	return a * b;
}

template<binary_op P>
void executeOp(std::vector<int>& data, int op1, int op2, int op3) {
	const int op1Val = data.at(op1);
	const int op2Val = data.at(op2);
	const int destVal = data.at(op3);
	auto result = P(data.at(op1Val), data.at(op2Val));
	data.at(destVal) = result;
}

int execute(std::vector<int>& data, const int PC) {
	const int opcode = data.at(PC);

	switch(opcode) {
		case 1: executeOp<add>(data, PC + 1, PC + 2, PC + 3); return 4;
		case 2: executeOp<mul>(data, PC + 1, PC + 2, PC + 3); return 4;
		default:
			return -1;
	}
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cout << "Expected an argument.\n";
		return -2;
	}
	// holds both program and memory
	auto data = readInput<int, comma_sep<int>>(argv[1]);
	log() << "size: " << data.size() << std::endl;

	// adjustments for data/input
	data.at(1) = 12;
	data.at(2) = 2;

	int PC = 0;
	while (data.at(PC) != 99) {
		PC += execute(data, PC);
	}
	log() << "data[0]:" << data.at(0) << std::endl;
}

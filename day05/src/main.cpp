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

typedef int(*binary_op)(int, int); // signature for all valid template params
typedef std::vector<int> Memory;

enum OperatorMode {
	POSITION = 0,
	IMMEDIATE = 1
};

template<int offset>
struct Operand {
	enum OperatorMode mode;

	int eval(Memory& mem, const int programCounter) const {
		if (mode == OperatorMode::IMMEDIATE) {
			return mem.at(programCounter + offset);
		} else {
			auto address = mem.at(programCounter + offset);
			return mem.at(address);
		}
	}
};

struct Instruction {
	int instructionPointer;
	int opcode;
	struct Operand<1> op1;
	struct Operand<2> op2;
	struct Operand<3> op3;
};

template<int operatorNumber>
OperatorMode decodeOpCode(const int val) {
	static_assert(!(operatorNumber == 0), "Zeroth operator is a no. no.");

	return ((val % (10 ^ (2 + operatorNumber)) / (10 ^ (1 + operatorNumber)))) == 0 ?
		OperatorMode::POSITION : OperatorMode::IMMEDIATE;
}

template<typename T>
T add(T a, T b) {
	return a + b;
}

template<typename T>
T mul(T a, T b) {
	return a * b;
}

struct Instruction decodeInstruction(Memory& data, const int PC) {
	int val = data.at(PC);

	const int opcode = val % 100;
	const OperatorMode op1mode = decodeOpCode<1>(val);
	const OperatorMode op2mode = decodeOpCode<2>(val);

	return {
		PC,
		opcode,
		{op1mode}
	};
}

template<binary_op operation>
int executeInstruction(Memory& data, const Instruction instruction) {
	const int op1Val = instruction.op1.eval(data, instruction.instructionPointer);
	const int op2Val = instruction.op2.eval(data, instruction.instructionPointer);
	const int destVal = instruction.op3.eval(data, instruction.instructionPointer);
	auto result = operation(data.at(op1Val), data.at(op2Val));
	data.at(destVal) = result;
	return 4;
}

int storeFn(Memory& data, const Instruction instruction) {
	const int op1Val = instruction.op1.eval(data, instruction.instructionPointer);
	data.at(op1Val) = 0;//input;
	return 2;
}

int outputFn(Memory& data, const Instruction instruction) {
	const int op1Val = instruction.op1.eval(data, instruction.instructionPointer);
	std::cout << data.at(op1Val);
	return 2;
}

int execute(Memory& data, const int PC) {
	const Instruction operation = decodeInstruction(data, PC);

	log() << "OpCode: " << operation.opcode << std::endl;

	switch(operation.opcode) {
		case 1: return executeInstruction<add>(data, operation);
		case 2: return executeInstruction<mul>(data, operation);
		case 3: return storeFn(data, operation);
		case 4: return outputFn(data, operation);
		default:
			return -1;
	}
}

Memory getProgram(const char* filename) {
	auto input = readInput<int, comma_sep<int>>(filename);
	auto val = Memory{input.begin(), input.end()};
	return val;
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cout << "Expected an argument.\n";
		return -2;
	}
	// holds both program and memory
	auto data = getProgram(argv[1]);
	log() << "size: " << data.size() << std::endl;

	int PC = 0;
	int input = 15;

	// Part1
	log() << "==============================================\n"
		<< "Execute with input: " << input << std::endl;
	{
		// adjustments for data/input
		//data.at(1) = input;

		bool inRange = PC < data.size();

		while (data.at(PC) != 99 && inRange) {
			PC += execute(data, PC);
			inRange = PC < data.size();
		}

		if (inRange) {
			log() << "Ran out of memory.";
		} else {
			log() << "data[0]:" << data.at(0) << std::endl;
		}
	}
}

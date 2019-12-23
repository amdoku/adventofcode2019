#include <iostream>
#include <array>
#include <vector>

int main(int argc, char** argv) {
	std::cout << "Day4" << std::endl;
	const int start = 353096;
	const int end = 843212;

	auto buf = std::array<char, 7>();
	int current = start;

	auto hasDoubleAdjacentDigits = [](std::array<char, 7> data) -> bool {
		// TODO allow for 111122 to pass
		return (data[0] == data[1] && data[1] != data[2])
				|| (data[1] == data[2] && data[2] != data[3])
				|| (data[2] == data[3] && data[3] != data[4])
				|| (data[3] == data[4] && data[4] != data[5])
				|| (data[4] == data[5] && data[4] != data[3]);
	};

	auto notDecreasing = [](std::array<char, 7> data) -> bool {
		return !(data[0] > data[1]
				|| data[1] > data[2]
				|| data[2] > data[3]
				|| data[3] > data[4]
				|| data[4] > data[5]);
	};
	std::vector<int> passwords;

	while (current < end) {
		std::sprintf(buf.data(), "%d", current);

		bool ok = hasDoubleAdjacentDigits(buf) && notDecreasing(buf);
		if(ok) {
			passwords.push_back(current);
		}
		current++;
	}


	std::cout << "Poosibilities: " << passwords.size() << std::endl;

	return 0;
}
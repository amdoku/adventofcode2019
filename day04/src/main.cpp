#include <iostream>
#include <array>
#include <vector>
#include <functional>

int main(int argc, char** argv) {
	std::cout << "Day4" << std::endl;
	const int start = 353096;
	const int end = 843212;

	auto buf = std::array<char, 7>();
	int current = start;

	auto hasDoubleAdjacentDigitsOfNumber = [](std::array<char, 7> data, char t) -> bool {
		return     (data[0] == t && data[1] == t && t != data[2])
				|| (data[1] == t && data[2] == t && t != data[3])
				|| (data[2] == t && data[3] == t && t != data[4])
				|| (data[3] == t && data[4] == t && t != data[5])
				|| (data[4] == t && data[5] == t && t != data[3]);
	};

	auto hasDoubleAdjacentDigits = [&test = hasDoubleAdjacentDigitsOfNumber](std::array<char, 7> data) -> bool {
		bool ok = false; // until proven otherwise
		int repetitionCount = 0;
		char prev = 0, current = 0;
		for(int idx = 0; idx < 7; idx++) {
			current = data[idx];
			if (current == prev) { repetitionCount++; }
			else if (repetitionCount == 1) { return true; }
			else { repetitionCount = 0; }
			prev = data[idx];
		}
		return repetitionCount == 1;
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
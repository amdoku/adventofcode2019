#pragma once

#include <iostream>
#include <iomanip>
#include <iterator>
#include <vector>
#include <limits>

#include "common_types.hpp"

/*
 * This generator takes a single element and derives multiple values from it.
 *
 * parent_iterator	The type of iterator used to access the base elements
 *					(i.e. vector<int>::const_iterator)
 * T	The type that is dereferenced (i.e. for vector<int>::const_iterator
 * 		that would be int)
 */

class BlackMagicGenerator : public std::iterator<std::forward_iterator_tag, PuzzleCount, std::ptrdiff_t, const PuzzleCount*, const PuzzleCount&> {
	using base = std::iterator<std::forward_iterator_tag, Coordinate, std::ptrdiff_t, const PuzzleCount*, const PuzzleCount&>;
	using self = BlackMagicGenerator;
	using parent_iterator = decltype(std::vector<WireToken>())::const_iterator;
public:
	BlackMagicGenerator() : baseStart(), baseEnd(), iteration(std::numeric_limits<decltype(iteration)>::max()) { };
	BlackMagicGenerator(parent_iterator start, parent_iterator end)
		: baseStart(start), baseEnd(end) {
			updateDirection();
		};

	self operator++() {
		if (baseStart != baseEnd) {
			if (iteration == baseStart->length) {
				++baseStart;
				iteration = 0;
				updateDirection();
			}

			if (baseStart != baseEnd) { // previous increment might have given us an endIterator
				iteration++;
				currentVal += direction;
			}
		} else {
			iteration = std::numeric_limits<decltype(iteration)>::max();
			direction = { 0, 0 };
		}

		return *this;
	}

	self operator++(int) = delete;
	self operator--() = delete;
	self operator--(int) = delete;

	base::reference operator*() {
		return currentVal;
	}

	friend bool operator>(const BlackMagicGenerator& lhs, const BlackMagicGenerator& rhs) {
		if (lhs.iteration == std::numeric_limits<decltype(lhs.iteration)>::max()
			&& rhs.iteration == std::numeric_limits<decltype(rhs.iteration)>::max()) return false;
		if (lhs.baseStart < rhs.baseStart) return true;
		if (lhs.baseStart > rhs.baseStart) return false;
		return lhs.iteration > rhs.iteration;
	}

	friend bool operator<(const BlackMagicGenerator& lhs, const BlackMagicGenerator& rhs) {
		if (lhs.iteration == std::numeric_limits<decltype(lhs.iteration)>::max()
			&& rhs.iteration == std::numeric_limits<decltype(rhs.iteration)>::max()) return false;
		return !(lhs > rhs) && lhs != rhs;
	}

	friend bool operator==(const BlackMagicGenerator& lhs, const BlackMagicGenerator& rhs) {
		if (lhs.iteration == std::numeric_limits<decltype(lhs.iteration)>::max()
			&& rhs.iteration == std::numeric_limits<decltype(rhs.iteration)>::max()) return true;
		return lhs.baseStart == rhs.baseStart && lhs.iteration == rhs.iteration;
	}

	friend bool operator!=(const BlackMagicGenerator& lhs, const BlackMagicGenerator& rhs) {
		return !(lhs == rhs);
	}

	std::ostream& operator<<(std::ostream& out) const {
		return out << "BMI[iteration=" << iteration
				<< ", baseStart!=baseEnd=" << (baseStart != baseEnd)
				<< ", " << ((baseStart != baseEnd) ? *baseStart : WireToken{{'Z'}, -1})
				<< ", currentVal=" << currentVal
				<< "]";
	}

private:
	PuzzleCount currentVal{0,0,0};
	CoordinateDelta direction{0,0};
	parent_iterator baseStart;
	parent_iterator baseEnd;
	uint32_t iteration{0};

	void updateDirection() {
		if (baseStart == baseEnd) {
			iteration = std::numeric_limits<decltype(iteration)>::max();
		}
		else if (baseStart->dir.c == 'L') { direction = { 1, 0 }; }
		else if (baseStart->dir.c == 'R') { direction = { -1 , 0 }; }
		else if (baseStart->dir.c == 'D') { direction = { 0, -1 }; }
		else if (baseStart->dir.c == 'U') { direction = { 0, 1 }; }
		else {
			std::cout << "Encountered direction: '" << baseStart->dir.c << "'. Aborting." << std::endl;
			std::terminate();
		}
	}
};

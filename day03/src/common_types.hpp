#pragma once

#include <cstddef>
#include <functional>
#include <iostream>

struct Direction {
	char c;
};

struct WireToken {
	struct Direction dir;
	int length;
};

struct Coordinate {
	int x;
	int y;
};

struct PuzzleCount {
	Coordinate c;
	uint64_t step;
};

bool operator<(const PuzzleCount& lhs, const PuzzleCount& rhs) {
	if (lhs.c.x < rhs.c.x) return true;
	if (lhs.c.x == rhs.c.x) return lhs.c.y < rhs.c.y;
	return false;
}

bool operator==(const PuzzleCount& lhs, const PuzzleCount& rhs) {
	return lhs.c.x == rhs.c.x && lhs.c.y == rhs.c.y;
}

bool operator<(const Coordinate& lhs, const Coordinate& rhs) {
	if (lhs.x < rhs.x) return true;
	if (lhs.x == rhs.x) return lhs.y < rhs.y;
	return false;
}

bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct CoordinateDelta {
	int dX;
	int dY;
};

std::ostream& operator<<(std::ostream& out, const Direction& t) {
	return out << t.c;
}

std::ostream& operator<<(std::ostream& out, const WireToken& t) {
	return out << "(" << t.dir << "," << t.length << ")";
}

std::ostream& operator<<(std::ostream& out, const Coordinate& t) {
	return out << "(" << t.x << "," << t.y << ")";
}

std::ostream& operator<<(std::ostream& out, const PuzzleCount& t) {
	return out << t.c << "@" << t.step;
}

std::ostream& operator<<(std::ostream& out, const std::pair<PuzzleCount, PuzzleCount>& t) {
	return out << "[" << t.first << " | " << t.second << "]";
}

Coordinate operator+(const Coordinate& lhs, const CoordinateDelta& rhs) {
	return { lhs.x + rhs.dX, lhs.y + rhs.dY };
}

PuzzleCount operator+(const PuzzleCount& lhs, const CoordinateDelta& rhs) {
	return { lhs.c + rhs, lhs.step + 1 };
}

Coordinate operator+=(Coordinate& lhs, const CoordinateDelta& rhs) {
	lhs.x+=rhs.dX;
	lhs.y+=rhs.dY;
	return lhs;
}

PuzzleCount operator+=(PuzzleCount& lhs, const CoordinateDelta& rhs) {
	lhs.c += rhs;
	++lhs.step;
	return lhs;
}

namespace std {
	template<>
	struct hash<Coordinate> {
		using argument_type = Coordinate;
		using result_type = std::size_t;

		result_type operator()(const argument_type& t) const {
			return t.x * 31 + t.y;
		}
	};

	template<>
	struct hash<PuzzleCount> {
		using argument_type = PuzzleCount;
		using result_type = std::size_t;

		result_type operator()(const argument_type& t) const {
			return t.step + t.c.x * 31 + t.c.y * 63;
		}
	};
}
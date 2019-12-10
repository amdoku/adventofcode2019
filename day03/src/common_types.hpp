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

Coordinate operator+(const Coordinate& lhs, const CoordinateDelta& rhs) {
	return { lhs.x + rhs.dX, lhs.y + rhs.dY };
}

Coordinate operator+=(Coordinate& lhs, const CoordinateDelta& rhs) {
	lhs.x+=rhs.dX;
	lhs.y+=rhs.dY;
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
}
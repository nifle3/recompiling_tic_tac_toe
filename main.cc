#include <array>
#include <string_view>
#include <tuple>
#include <ranges>
#include <iostream>
#include <print>

#ifndef TURNS
#define TURNS ""
#endif

#ifndef COUNT_TURNS
#define COUNT_TURNS 0
#endif

consteval int parse_int(auto&& range)
{
    int result = 0;

    for (char c : range)
        result = result * 10 + (c - '0');

    return result;
}

template<size_t SIZE>
consteval auto parse_moves(std::string_view str) {
	std::array<std::tuple<int,int>, SIZE> moves;

    for (auto [i, move] : str | std::views::split('.') | std::views::enumerate) {
        if (move.empty()) {
            continue;
	}

        int values[2]{};

        for (auto [j, value] : move | std::views::split(',') | std::views::enumerate) {
            values[j] = parse_int(value);
        }

        moves[i] = {values[0], values[1]};
    }

	return moves;
}

consteval auto build_field_from_moves(auto moves) {
	std::array<char, 9> field;
	field.fill(' ');

	for (const auto [index, value] : std::views::enumerate(moves)) {
		const auto [x, y] = value;
		const auto i = x + (y * 3);
		if (index % 2 == 0) {
			field[i] = 'O';
		} else {
			field[i] = 'X';
		}
	} 

	return field;
}

consteval auto build_empty_field() {
	std::array<char, 9> field;
	field.fill(' ');
	return field;
}

void print_field(auto field) {
	std::println("{}|{}|{}", field[0], field[1], field[2]);
	std::println("-----");
	std::println("{}|{}|{}", field[3], field[4], field[5]);
	std::println("-----");
	std::println("{}|{}|{}", field[6], field[7], field[8]);
}

void print_turn(int count_turns) {
	char turn = 'O';
	if (count_turns % 2 != 0) {
		turn = 'X';
	}
	
	std::println("Turn {} - {}", count_turns, turn);
}

std::tuple<int, int> get_user_input() {
	int x, y;
	std::cin >> x >> y;
	return {x, y};
}

int main() {
	constexpr std::string_view all_moves = TURNS;
	constexpr int count_turns = COUNT_TURNS;

	if constexpr (count_turns == 0) {
		constexpr auto field = build_empty_field();
		print_field(field);
		print_turn(count_turns);
		const auto [x, y] = get_user_input();
		std::println("{} {}", x, y);
	} else {
		constexpr auto moves = parse_moves<count_turns>(all_moves);
		constexpr auto field = build_field_from_moves(moves);
		print_field(field);
		print_turn(count_turns);
		const auto [x, y] = get_user_input();
	}
}

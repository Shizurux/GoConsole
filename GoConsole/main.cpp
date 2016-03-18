#include <iostream>
#include <string>
#include <vector>

#include <conio.h>
#include <Windows.h>

#define BLACK 0
#define YELLOW 6
#define WHITE 7
#define GREY 8
#define LRED 12
#define LPURPLE 13
#define LYELLOW 14
#define BWHITE 15

std::string gxy(SHORT x, SHORT y) {
	static auto outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(outputHandle, { x, y });
	return "";
}

std::string sc(WORD fg, WORD bg) {
	static auto outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(outputHandle, fg + bg*16);
	return "";
}

char printMenu() {
	system("cls");
	std::cout << gxy(1, 1) << sc(LYELLOW, BLACK) << "GoConsole ";
	std::cout << sc(YELLOW, BLACK) << "v0.1";

	std::cout << gxy(4, 3) << sc(WHITE, BLACK) << "1) Play";
	std::cout << gxy(4, 4) << "2) Exit";

	return _getch();
}

class GoBoard {
	unsigned short boardSize;
	std::vector<std::vector<char>> board;

	bool currentPlayer; // false = black, true = white

public:
	explicit GoBoard(unsigned short boardSize) {
		this->boardSize = boardSize;
		
		board.resize(boardSize);
		for (auto i = 0; i < static_cast<int>(board.size()); i++) {
			board[i].resize(boardSize);
			for (auto j = 0; j < static_cast<int>(board.size()); j++) {
				board[i][j] = '.';
			}
		}

		currentPlayer = false;
	}

	bool checkMove(std::string move) const {
		if (move[0] < 65 && move[0] >= 65 + boardSize && move[1] < 65 && move[1] >= 65 + boardSize) {
			return false;
		}

		if (move[0] < 97 && move[0] >= 97 + boardSize && move[1] < 97 && move[1] >= 97 + boardSize) {
			return false;
		}

		unsigned short x, y;

		if (move[0] >= 65 && move[0] < 65 + boardSize) {
			x = static_cast<int>(move[0]) - 65;
			y = static_cast<int>(move[1]) - 97;
		}
		else {
			x = static_cast<int>(move[0]) - 97;
			y = static_cast<int>(move[1]) - 65;
		}

		if (board[x][y] != '.') {
			return false;
		}

		return true;
	}

	void placePiece(std::string move) {
		unsigned short x, y;

		if (move[0] > 65 && move[0] < 65 + boardSize) {
			x = static_cast<int>(move[0]) - 65;
			y = static_cast<int>(move[1]) - 97;
		}
		else {
			x = static_cast<int>(move[0]) - 97;
			y = static_cast<int>(move[1]) - 65;
		}

		board[x][y] = currentPlayer ? 'o' : 'x';
		currentPlayer = !currentPlayer;
	}

	bool nextTurn() {
		std::cout
			<< gxy(boardSize + 10, 10)
			<< sc(currentPlayer ? BWHITE : GREY, BLACK)
			<< (currentPlayer ? "WHITE" : "BLACK");

		std::cout << sc(WHITE, BLACK) << "'s turn.";
		
		std::string move;
		while (true) {
			std::cout << gxy(boardSize + 10, 11) << "Enter your move:      ";
			std::cout << gxy(boardSize + 10, 11) << sc(WHITE, BLACK) << "Enter your move: ";
			getline(std::cin, move);

			if (move == "resign") {
				return false;
			}

			if (checkMove(move)) {
				break;
			}
			
			std::cout << gxy(boardSize + 10, 13) << sc(LRED, BLACK) << "Illegal Move";
		}
		std::cout << gxy(boardSize + 10, 13) << "            ";

		placePiece(move);
		return true;
	}

	void render(SHORT x, SHORT y) const {
		sc(YELLOW, BLACK);

		for (auto i = 0; i < boardSize; i++) {
			std::cout << gxy(x + 1 + i, y                ) << static_cast<char>(i + 97);
			std::cout << gxy(x + 1 + i, y + boardSize + 1) << static_cast<char>(i + 97);

			std::cout << gxy(x                , y + 1 + i) << static_cast<char>(i + 65);
			std::cout << gxy(x + boardSize + 1, y + 1 + i) << static_cast<char>(i + 65);

			for (auto j = 0; j < boardSize; j++) {
				if (board[i][j] == '.') {
					sc(YELLOW, BLACK);
				}
				else if (board[i][j] == 'o') {
					sc(BWHITE, BLACK);
				}
				else {
					sc(GREY, BLACK);
				}

				std::cout << gxy(x + 1 + i, y + 1 + j) << board[i][j];
			}
		}
	}
};

void play() {
	system("cls");
	std::cout << gxy(1, 1) << sc(LYELLOW, BLACK) << "GoConsole ";
	std::cout << sc(YELLOW, BLACK) << "v0.1 ";
	std::cout << sc(LPURPLE, BLACK) << "Play";

	GoBoard gb(19);

	while (true) {
		gb.render(3, 3);
		if (!gb.nextTurn()) {
			break;
		}
	}
}

int main() {
	while (true) {
		switch (printMenu()) {
		case '1':
			play();
			break;

		case '2':
			system("cls");
			return 0;
		}
	}
}
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <assert.h>
#include <vector>

struct settings {
	int timebank;
	int time_per_move;
	int your_botid;
};

struct board {
	int field[9][9];
	int macroboard[3][3];

	board* copy()
	{
		board* newBoard = new board();
		std::memcpy(&(newBoard->field)[0][0], &field[0][0], sizeof(int) * 9 * 9);
		std::memcpy(&(newBoard->macroboard)[0][0], &macroboard[0][0], sizeof(int) * 3 * 3);
		return newBoard;
	}

	int getValue(int player)
	{
		for (int y = 0; y < 3; ++y)
		{
			if (macroboard[0][y] == player && macroboard[1][y] == player && macroboard[2][y] == player)
			{
				return INT_MAX;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if (macroboard[x][0] == player && macroboard[x][1] == player && macroboard[x][2] == player)
			{
				return INT_MAX;
			}
		}
		if (macroboard[0][0] == player && macroboard[1][1] == player && macroboard[2][2] == player)
			return INT_MAX;
		if (macroboard[2][0] == player && macroboard[1][1] == player && macroboard[0][2] == player)
		{
			return INT_MAX;
		}
		int score = 0;
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				if (macroboard[gridX][gridY] == player)
				{
					score += 20;
				}
				else if (macroboard[gridX][gridY] <= 0)
				{
					for (int y = 0; y < 3; ++y)//Vertical two-in-a-row
					{
						int currPlayer = field[3 * gridX + 1][3 * gridY + y];
						if (currPlayer > 0)
						{
							if ((field[3 * gridX][3 * gridY + y] == player && field[3 * gridX + 2][3 * gridY + y] == 0) || // X|X|_
								(field[3 * gridX][3 * gridY + y] == 0 && field[3 * gridX + 2][3 * gridY + y] == player))   // _|X|X
							{
								score++;
							}
						}
						else if (currPlayer == 0){
							if (field[3 * gridX][3 * gridY + y] == field[3 * gridX + 2][3 * gridY + y] && field[3 * gridX][3 * gridY + y] == player) // X|_|X
							{
								score++;
							}
						}
					}
					for (int x = 0; x < 3; ++x)//Vertical two-in-a-row
					{
						int currPlayer = field[3 * gridX + x][3 * gridY + 1];
						if (currPlayer > 0)
						{
							if ((field[3 * gridX + x][3 * gridY] == player && field[3 * gridX + x][3 * gridY + 2] == 0) || // X|X|_
								(field[3 * gridX + x][3 * gridY] == 0 && field[3 * gridX + x][3 * gridY + 2] == player))   // _|X|X
							{
								score++;
							}
						}
						else if (currPlayer == 0) {
							if (field[3 * gridX + x][3 * gridY] == field[3 * gridX + x][3 * gridY + 2] && field[3 * gridX + x][3 * gridY] == player) // X|_|X
							{
								score++;
							}
						}
					}
					if (field[3 * gridX + 1][3 * gridY + 1] == player) //Center is claimed
					{
						if ((field[3 * gridX][3 * gridY] == player && field[3 * gridX + 2][3 * gridY + 2] == 0) || //Top left to bottom right
							(field[3 * gridX][3 * gridY] == 0 && field[3 * gridX + 2][3 * gridY + 2] == player))
						{
							score++;
						}
						if ((field[3 * gridX + 2][3 * gridY] == player && field[3 * gridX][3 * gridY + 2] == 0) || //Top right to bottom left
							(field[3 * gridX + 2][3 * gridY] == 0 && field[3 * gridX][3 * gridY + 2] == player))
						{
							score++;
						}
					}
					if (field[3 * gridX + 1][3 * gridY + 1] == 0)
					{
						if ((field[3 * gridX][3 * gridY] == player && field[3 * gridX + 2][3 * gridY + 2] == player) || //Top left to bottom right
							(field[3 * gridX + 2][3 * gridY] == player && field[3 * gridX][3 * gridY + 2] == player))   //Top right to bottom left
						{
							score++;
						}
					}
				}
			}
		}
		for (int y = 0; y < 3; ++y)
		{
			if ((macroboard[0][y] == player && macroboard[1][y] == player && macroboard[2][y] <= 0) || //X|X|_
				(macroboard[0][y] == player && macroboard[1][y] <= 0 && macroboard[2][y] == player) || //X|_|X
				(macroboard[0][y] <= 0 && macroboard[1][y] == player && macroboard[2][y] == player))   //_|X|X
			{
				score += 10;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if ((macroboard[x][0] == player && macroboard[x][1] == player && macroboard[x][2] <= 0) || //X|X|_
				(macroboard[x][0] == player && macroboard[x][1] <= 0 && macroboard[x][2] == player) || //X|_|X
				(macroboard[x][0] <= 0 && macroboard[x][1] == player && macroboard[x][2] == player))   //_|X|X
			{
				score += 10;
			}
		}
		if (macroboard[1][1] == player)
		{
			if ((macroboard[0][0] == player && macroboard[2][2] <= 0) ||
				(macroboard[0][0] <= 0 && macroboard[2][2] == player))
			{
				score += 10;
			}
			if ((macroboard[2][0] == player && macroboard[0][2] <= 0) ||
				(macroboard[2][0] <= 0 && macroboard[0][2] == player))
			{
				score += 10;
			}
		}
		else if (macroboard[1][1] <= 0)
		{
			if (macroboard[0][0] == player && macroboard[2][2] == player)
			{
				score += 10;
			}
			if (macroboard[2][0] == player && macroboard[0][2] == player)
			{
				score += 10;
			}
		}
		return score;
	}

	void play_move(int player, int x, int y)
	{
		assert(field[x][y] == 0);

		int gridX = (int)(x / 3);
		int gridY = (int)(y / 3);
		assert(macroboard[gridX][gridY] == -1);

		field[x][y] = player;
		
		int boxX = x % 3;
		int boxY = y % 3;

		if (field[gridX][y] == field[gridX + 1][y] && field[gridX + 1][y] == field[gridX + 2][y])
			macroboard[gridX][gridY] = player;
		else if (field[x][gridY] == field[x][gridY+1] && field[x][gridY+1] == field[x][gridY+2])
			macroboard[gridX][gridY] = player;
		else if (field[3 * gridX][3 * gridY] == player && field[3 * gridX + 1][3 * gridY + 1] == player && field[3 * gridX + 2][3 * gridY + 2] == player)
		{
			macroboard[gridX][gridY] = player;
		}
		else if (field[3 * gridX + 2][3 * gridY] == player && field[3 * gridX + 1][3 * gridY + 1] == player && field[3 * gridX][3 * gridY + 2] == player)
		{
			macroboard[gridX][gridY] = player;
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (macroboard[j][i] == -1)
				{
					macroboard[j][i] = 0;
				}
			}
		}
		if (macroboard[boxX][boxY] == 0)
		{
			macroboard[boxX][boxY] = -1;
		}
		else {
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					if (macroboard[j][i] == 0)
					{
						macroboard[j][i] = -1;
					}
				}
			}
		}
	}
};

bool evaluateInput(std::string input, settings* s, board* b, int* move)
{
	if (input.compare(0, 9, "settings ") == 0)
	{
		input.erase(0, 9);
		if (input.compare(0, 9, "timebank ") == 0)
		{
			input.erase(0, 9);
			s->timebank = std::stoi(input);
		}
		else if (input.compare(0, 14, "time_per_move ") == 0)
		{
			input.erase(0, 14);
			s->time_per_move = std::stoi(input);
		}
		else if (input.compare(0, 13, "player_names ") == 0)
		{
			input.erase(0, 13);
		}
		else if (input.compare(0, 14, "time_per_move ") == 0)
		{
			input.erase(0, 14);
		}
		else if (input.compare(0, 9, "your_bot ") == 0)
		{
			input.erase(0, 9);
		}
		else if (input.compare(0, 11, "your_botid ") == 0)
		{
			input.erase(0, 11);
			s->your_botid = std::stoi(input);
		}
		else {
			assert(0);
		}
		return true;
	}
	else if (input.compare(0, 12, "update game ") == 0)
	{
		input.erase(0, 12);
		if (input.compare(0, 6, "round ") == 0)
		{
			input.erase(0, 6);
			
		}
		else if (input.compare(0, 5, "move ") == 0)
		{
			input.erase(0, 5);
			(*move) = std::stoi(input);
		}
		else if (input.compare(0, 6, "field ") == 0)
		{
			input.erase(0, 6);
			std::stringstream ss(input);
			for (int y = 0; y < 9; ++y)
			{
				for (int x = 0; x < 9; ++x)
				{
					ss >> b->field[x][y];
					if (ss.peek() == ',')
						ss.ignore();
				}
			}
		}
		else if (input.compare(0, 11, "macroboard ") == 0)
		{
			input.erase(0, 11);
			std::stringstream ss(input);
			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					ss >> b->macroboard[x][y];
					if (ss.peek() == ',')
						ss.ignore();
				}
			}
		}
		else {
			assert(0);
		}
		return true;
	}
	else if (input.compare(0, 12, "action move ") == 0)
	{
		input.erase(0, 12);
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

int minimax(board* b, int currPlayer, bool maximizing, int depth, bool getIndex)
{
	if (depth <= 0)
		return b->getValue(currPlayer) - b->getValue(!(currPlayer - 1) + 1);
	int best = maximizing ? INT_MIN : INT_MAX;
	std::vector<int> bestMovesX;
	std::vector<int> bestMovesY;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			if (b->macroboard[gridX][gridY] == -1)
			{
				for (int y = 0; y < 3; ++y)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (b->field[3 * gridX + x][3 * gridY + y] == 0)
						{
							board* newBoard = b->copy();
							newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
							int v = minimax(newBoard, !(currPlayer - 1) + 1, !maximizing, depth - 1, false);
							delete newBoard;
							if (maximizing)
							{
								if (getIndex && v > best)
								{
									bestMovesX.clear();
									bestMovesY.clear();
								}
								best = std::max(best, v);
							}
							else {
								if (getIndex && v < best)
								{
									bestMovesX.clear();
									bestMovesY.clear();
								}
								best = std::min(best, v);
							}
							if (getIndex && best == v)
							{
								bestMovesX.push_back(3 * gridX + x);
								bestMovesY.push_back(3 * gridY + y);
							}
						}
					}
				}
			}
		}
	}
	if (getIndex)
	{
		int i = rand() % bestMovesX.size();
		return 9 * bestMovesY[i] + bestMovesX[i];
	}
	return best;
}

int main()
{
	settings* gameSettings = new settings();
	board* gameBoard = new board();
	int move = 0;
	std::string line;
	while (1)
	{
		do
		{
			std::getline(std::cin, line);
		} while (evaluateInput(line, gameSettings, gameBoard, &move));
		if (move == 1)
		{
			std::cout << "place_move " << rand()%9 << " " << rand()%9 << std::endl;
		}
		else {
			int index = minimax(gameBoard, gameSettings->your_botid, true, 6, true);
			int x = index % 9;
			int y = (int)((index - x) / 9);
			std::cout << "place_move " << x << " " << y << std::endl;
		}
	}
}
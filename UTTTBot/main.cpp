#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <assert.h>
#include <cstring>
#include <ctime>
#include <vector>

#define INT_MAX 2147483647
#define INT_MIN -2147483647

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

	int winner()
	{
		for (int y = 0; y < 3; ++y)
		{
			int player = macroboard[0][y];
			if (player > 0)
			{
				if (macroboard[1][y] == player && macroboard[2][y] == player)
				{
					return player;
				}
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			int player = macroboard[x][0];
			if (player > 0)
			{
				if (macroboard[x][1] == player && macroboard[x][2] == player)
				{
					return player;
				}
			}
		}
		int player = macroboard[1][1];
		if (player > 0)
		{
			if (macroboard[0][0] == player && macroboard[2][2] == player)
			{
				return player;
			}
			if (macroboard[2][0] == player && macroboard[0][2] == player)
			{
				return player;
			}
		}
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				if (macroboard[x][y] != -2 && macroboard[x][y] != 1 && macroboard[x][y] != 2)
					return 0;
			}
		}
		return -1;
	}

	int getValue(int player)
	{
		int opponent = !(player - 1) + 1;
		for (int y = 0; y < 3; ++y)
		{
/*DONE*/	if (macroboard[0][y] == player && macroboard[1][y] == player && macroboard[2][y] == player)
			{
				return INT_MAX;
			}
/*DONE*/	if (macroboard[0][y] == opponent && macroboard[1][y] == opponent && macroboard[2][y] == opponent)
			{
				return INT_MIN;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
/*DONE*/	if (macroboard[x][0] == player && macroboard[x][1] == player && macroboard[x][2] == player)
			{
				return INT_MAX;
			}
/*DONE*/	if (macroboard[x][0] == opponent && macroboard[x][1] == opponent && macroboard[x][2] == opponent)
			{
				return INT_MIN;
			}
		}
/*DONE*/if (macroboard[0][0] == player && macroboard[1][1] == player && macroboard[2][2] == player)
			return INT_MAX;
/*DONE*/if (macroboard[2][0] == player && macroboard[1][1] == player && macroboard[0][2] == player)
			return INT_MAX;
/*DONE*/if (macroboard[0][0] == opponent && macroboard[1][1] == opponent && macroboard[2][2] == opponent)
			return INT_MIN;
/*DONE*/if (macroboard[2][0] == opponent && macroboard[1][1] == opponent && macroboard[0][2] == opponent)
			return INT_MIN;
		int score = 0;
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
/*DONE*/		if (macroboard[gridX][gridY] == player)
				{
					score += 20;
				}
/*DONE*/		else if (macroboard[gridX][gridY] == opponent)
				{
					score -= 20;
				}
				else if (macroboard[gridX][gridY] == 0 || macroboard[gridX][gridY] == -1)
				{
					for (int y = 0; y < 3; ++y)//Horizontal two-in-a-row
					{
						int currPlayer = field[3 * gridX + 1][3 * gridY + y];
						if (currPlayer > 0)
						{
							if ((field[3 * gridX][3 * gridY + y] == currPlayer && field[3 * gridX + 2][3 * gridY + y] == 0) || // X|X|_
								(field[3 * gridX][3 * gridY + y] == 0 && field[3 * gridX + 2][3 * gridY + y] == currPlayer))   // _|X|X
							{
								if (currPlayer == player)
									score += 4;
								if (currPlayer == opponent)
									score -= 4;
							}
						}
						else if (currPlayer == 0){
							if (field[3 * gridX][3 * gridY + y] == player && field[3 * gridX + 2][3 * gridY + y] == player) // X|_|X
							{
								score+= 4;
							}
							else if (field[3 * gridX][3 * gridY + y] == opponent && field[3 * gridX + 2][3 * gridY + y] == opponent) // X|_|X
							{
								score-= 4;
							}
						}
					}
					for (int x = 0; x < 3; ++x)//Vertical two-in-a-row
					{
						int currPlayer = field[3 * gridX + x][3 * gridY + 1];
						if (currPlayer > 0)
						{
							if ((field[3 * gridX + x][3 * gridY] == currPlayer && field[3 * gridX + x][3 * gridY + 2] == 0) || // X|X|_
								(field[3 * gridX + x][3 * gridY] == 0 && field[3 * gridX + x][3 * gridY + 2] == currPlayer))   // _|X|X
							{
								if (currPlayer == player)
									score += 4;
								if (currPlayer == opponent)
									score -= 4;
							}
						}
						else if (currPlayer == 0) {
							if (field[3 * gridX + x][3 * gridY] == player && field[3 * gridX + x][3 * gridY + 2] == player) // X|_|X
							{
								score += 4;
							}
							else if (field[3 * gridX + x][3 * gridY] == opponent && field[3 * gridX + x][3 * gridY + 2] == opponent) // X|_|X
							{
								score -= 4;
							}
						}
					}
					if (field[3 * gridX + 1][3 * gridY + 1] == player) //Center is claimed
					{
						if ((field[3 * gridX][3 * gridY] == player && field[3 * gridX + 2][3 * gridY + 2] == 0) || //Top left to bottom right
							(field[3 * gridX][3 * gridY] == 0 && field[3 * gridX + 2][3 * gridY + 2] == player))
						{
							score+= 4;
						}
						if ((field[3 * gridX + 2][3 * gridY] == player && field[3 * gridX][3 * gridY + 2] == 0) || //Top right to bottom left
							(field[3 * gridX + 2][3 * gridY] == 0 && field[3 * gridX][3 * gridY + 2] == player))
						{
							score+= 4;
						}
					}
					if (field[3 * gridX + 1][3 * gridY + 1] == opponent) //Center is claimed
					{
						if ((field[3 * gridX][3 * gridY] == opponent && field[3 * gridX + 2][3 * gridY + 2] == 0) || //Top left to bottom right
							(field[3 * gridX][3 * gridY] == 0 && field[3 * gridX + 2][3 * gridY + 2] == opponent))
						{
							score-= 4;
						}
						if ((field[3 * gridX + 2][3 * gridY] == opponent && field[3 * gridX][3 * gridY + 2] == 0) || //Top right to bottom left
							(field[3 * gridX + 2][3 * gridY] == 0 && field[3 * gridX][3 * gridY + 2] == opponent))
						{
							score-= 4;
						}
					}
					if (field[3 * gridX + 1][3 * gridY + 1] == 0)
					{
						if ((field[3 * gridX][3 * gridY] == player && field[3 * gridX + 2][3 * gridY + 2] == player) || //Top left to bottom right
							(field[3 * gridX + 2][3 * gridY] == player && field[3 * gridX][3 * gridY + 2] == player))   //Top right to bottom left
						{
							score+= 4;
						}
						if ((field[3 * gridX][3 * gridY] == opponent && field[3 * gridX + 2][3 * gridY + 2] == opponent) || //Top left to bottom right
							(field[3 * gridX + 2][3 * gridY] == opponent && field[3 * gridX][3 * gridY + 2] == opponent))   //Top right to bottom left
						{
							score-= 4;
						}
					}
				}
			}
		}
		for (int y = 0; y < 3; ++y)
		{
/*DONE*/	if ((macroboard[0][y] == player && macroboard[1][y] == player && (macroboard[2][y] == 0 || macroboard[2][y] == -1)) || //X|X|_
				(macroboard[0][y] == player && (macroboard[1][y] == 0 || macroboard[1][y] == -1) && macroboard[2][y] == player) || //X|_|X
				((macroboard[0][y] == 0 || macroboard[0][y] == -1) && macroboard[1][y] == player && macroboard[2][y] == player))   //_|X|X
			{
				score += 20;
			}
/*DONE*/	else if ((macroboard[0][y] == opponent && macroboard[1][y] == opponent && (macroboard[2][y] == 0 || macroboard[2][y] == -1)) || //X|X|_
				(macroboard[0][y] == opponent && (macroboard[1][y] == 0 || macroboard[1][y] == -1) && macroboard[2][y] == opponent) || //X|_|X
				((macroboard[0][y] == 0 || macroboard[0][y] == -1) && macroboard[1][y] == opponent && macroboard[2][y] == opponent))   //_|X|X
			{
				score -= 20;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
/*DONE*/	if ((macroboard[x][0] == player && macroboard[x][1] == player && (macroboard[x][2] == 0 || macroboard[x][2] == -1)) || //X|X|_
				(macroboard[x][0] == player && (macroboard[x][1] == 0 || macroboard[x][1] == -1) && macroboard[x][2] == player) || //X|_|X
				((macroboard[x][0] == 0 || macroboard[x][0] == -1) && macroboard[x][1] == player && macroboard[x][2] == player))   //_|X|X
			{
				score += 20;
			}
/*DONE*/	else if ((macroboard[x][0] == opponent && macroboard[x][1] == opponent && (macroboard[x][2] == 0 || macroboard[x][2] == -1)) || //X|X|_
				(macroboard[x][0] == opponent && (macroboard[x][1] == 0 || macroboard[x][1] == -1) && macroboard[x][2] == opponent) || //X|_|X
				((macroboard[x][0] == 0 || macroboard[x][0] == -1) && macroboard[x][1] == opponent && macroboard[x][2] == opponent))   //_|X|X
			{
				score -= 20;
			}
		}
		int currPlayer = macroboard[1][1];
		if (currPlayer > 0)
		{
/*DONE*/	if ((macroboard[0][0] == currPlayer && (macroboard[2][2] == 0 || macroboard[2][2] == -1)) ||
				((macroboard[0][0] == 0 || macroboard[0][0] == -1) && macroboard[2][2] == currPlayer))
			{
				if (currPlayer == player)
					score += 20;
				else if (currPlayer == opponent)
					score -= 20;
			}
/*DONE*/	if ((macroboard[2][0] == currPlayer && (macroboard[0][2] == 0 || macroboard[0][2] == -1)) ||
				((macroboard[2][0] == 0 || macroboard[2][0] == -1) && macroboard[0][2] == currPlayer))
			{
				if (currPlayer == player)
					score += 20;
				else if (currPlayer == opponent)
					score -= 20;
			}
		}
		else if (currPlayer == 0 || currPlayer == -1)
		{
/*DONE*/	if ((macroboard[0][0] == player && macroboard[2][2] == player) ||
				(macroboard[2][0] == player && macroboard[0][2] == player))
			{
				score += 20;
			}
/*DONE*/	if ((macroboard[0][0] == opponent && macroboard[2][2] == opponent) ||
				(macroboard[2][0] == opponent && macroboard[0][2] == opponent))
			{
				score -= 20;
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

		if (field[3*gridX][y] == field[3 * gridX + 1][y] && field[3 * gridX + 1][y] == field[3 * gridX + 2][y])
			macroboard[gridX][gridY] = player;
		else if (field[x][3 * gridY] == field[x][3 * gridY+1] && field[x][3 * gridY+1] == field[x][3 * gridY+2])
			macroboard[gridX][gridY] = player;
		else if (field[3 * gridX][3 * gridY] == player && field[3 * gridX + 1][3 * gridY + 1] == player && field[3 * gridX + 2][3 * gridY + 2] == player)
		{
			macroboard[gridX][gridY] = player;
		}
		else if (field[3 * gridX + 2][3 * gridY] == player && field[3 * gridX + 1][3 * gridY + 1] == player && field[3 * gridX][3 * gridY + 2] == player)
		{
			macroboard[gridX][gridY] = player;
		}
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				if (macroboard[gridX][gridY] == -1)
				{
					macroboard[gridX][gridY] = 0;
				}
				if (macroboard[gridX][gridY] == 0)
				{
					macroboard[gridX][gridY] = -2;
					bool cont = true;
					for (int y = 0; y < 3 && cont; ++y)
					{
						for (int x = 0; x < 3 && cont; ++x)
						{
							if (field[3 * gridX + x][3 * gridY + y] == 0)
							{
								macroboard[gridX][gridY] = 0;
								cont = false;
								break;
							}
						}
					}
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

bool evaluateInput(std::string input, settings* s, board* b, int* move, int* timeLeft)
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
		(*timeLeft) = std::stoi(input);
		return false;
	}
	else
	{
		return true;
	}
	return true;
}
/*
int minimax(board* b, int currPlayer, bool maximizing, int max, int min, int depth, bool getIndex)
{
	bool print = false;
	if (depth <= 0 || b->winner() != 0)
		return b->getValue(currPlayer);
	int best = maximizing ? INT_MIN : INT_MAX;
	int bestMoveX = -1;
	int bestMoveY = -1;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				for (int x = 0; x < 3; ++x)
				{
					if (getIndex && print)
						std::cout << "[";
					if (b->field[3 * gridX + x][3 * gridY + y] == 0)
					{
						if (b->macroboard[gridX][gridY] == -1)
						{
							board* newBoard = b->copy();
							newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
							int v = minimax(newBoard, !(currPlayer - 1) + 1, !maximizing, max, min, depth - 1, false);
							if (getIndex && print)
							{
								if (v > 99)
								{
									std::cout << "MAX";
								}
								else if (v < -99)
								{
									std::cout << "MIN";
								}
								else {
									std::cout << v;
								}
							}
							delete newBoard;
							if (maximizing)
							{
								if (getIndex && v > best)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
								best = std::max(best, v);
								max = std::max(max, best);
								if (best == INT_MIN && getIndex)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
							}
							else {
								if (getIndex && v < best)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
								best = std::min(best, v);
								min = std::min(min, best);
								if (best == INT_MAX && getIndex)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
							}
							/*
							if (min < max)
							{
								if (getIndex)
								{
									return 9 * bestMoveY + bestMoveX;
								}
								return best;
							}
							*/
/*
						}
					}
					if (getIndex && print)
						std::cout << "]";
				}
				if (getIndex && print)
					std::cout << " ";
			}
			if (getIndex && print)
			std::cout << std::endl;
		}
		if (getIndex && print)
			std::cout << std::endl;
	}
	if (getIndex)
	{
		return 9 * bestMoveY + bestMoveX;
	}
	return best;
}
*/
int alphaBeta(board* b, int currPlayer, int scorePlayer, bool maximizing, int alpha, int beta, int depth, bool getIndex)
{
	bool print = true;
	if (print && getIndex)
	{
		std::cerr << "Depth: " << depth << std::endl;
	}
	if (depth <= 0 || b->winner() != 0)
		return b->getValue(scorePlayer);
	int best = maximizing ? INT_MIN : INT_MAX;
	int bestMoveX = -1;
	int bestMoveY = -1;
	int gridCount = 0;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			if (b->macroboard[gridX][gridY] == -1)
			{
				gridCount++;
			}
		}
	}
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				for (int x = 0; x < 3; ++x)
				{
					if (getIndex && print)
						std::cerr << "[";
					if (b->field[3 * gridX + x][3 * gridY + y] == 0)
					{
						if (b->macroboard[gridX][gridY] == -1)
						{
							board* newBoard = b->copy();
							newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
							int tempDepth = depth;
							if (newBoard->macroboard[gridX][gridY] == currPlayer && gridCount <= 3)
								tempDepth++;
							int v = alphaBeta(newBoard, !(currPlayer - 1) + 1, scorePlayer, !maximizing, alpha, beta, tempDepth - 1, false);
							if (getIndex && print)
							{
								if (v > 99)
								{
									std::cerr << "MAX";
								}
								else if (v < -99)
								{
									std::cerr << "MIN";
								}
								else{
									if (v >= 0)
									{
										std::cerr << " ";
									}
									if (abs(v) < 10)
									{
										std::cerr << " ";
									}
									std::cerr << v;
								}
							}
							delete newBoard;
							if (maximizing)
							{
								if (getIndex && v > best)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
								best = std::max(best, v);
								alpha = std::max(alpha, best);
								if (best == INT_MIN && getIndex)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
							}
							else {
								if (getIndex && v < best)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
								best = std::min(best, v);
								beta = std::min(beta, best);
								if (best == INT_MAX && getIndex)
								{
									bestMoveX = 3 * gridX + x;
									bestMoveY = 3 * gridY + y;
								}
							}
							
							if (beta <= alpha)
							{
								if (getIndex)
								{
									if (print)
										std::cerr << std::endl;
									if (best == INT_MIN)
									{
										std::cerr << "no_moves" << std::endl;
									}
									return 9 * bestMoveY + bestMoveX;
								}
								return beta;
							}
							
						}
						else if (getIndex && print)
						{
							std::cerr << "   ";
						}
					}
					else if (getIndex && print)
					{
						std::cerr << "   ";
					}
					if (getIndex && print)
						std::cerr << "]";
				}
				if (getIndex && print)
					std::cerr << " ";
			}
			if (getIndex && print)
				std::cerr << std::endl;
		}
		if (getIndex && print)
			std::cerr << std::endl;
	}
	if (getIndex)
	{
		if (best == INT_MIN)
		{
			std::cout << "no_moves" << std::endl;
		}
		return 9 * bestMoveY + bestMoveX;
	}
	return best;
}


int main()
{
	settings* gameSettings = new settings();
	board* gameBoard = new board();
	int move = 1;
	int timeLeft = 10000;
	int lastTime = timeLeft;
	int depth = 5;
	std::string line;
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			gameBoard->field[x][y] = 0;
		}
	}
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			gameBoard->macroboard[x][y] = -1;
		}
	}

	while (gameBoard->winner() == 0)
	{
		
		do
		{
			std::getline(std::cin, line);
		} while (evaluateInput(line, gameSettings, gameBoard, &move, &timeLeft));
		
		std::cerr << "Move: " << move << std::endl;

		if (move == 1)
		{
			std::cout << "place_move 4 4" << std::endl;
		}
		else {
			depth = (int)(move / 3) + 2;
			depth = std::min(depth, 8);
			if (timeLeft < 8000)
			{
				depth--;
			}
			int index = alphaBeta(gameBoard, gameSettings->your_botid, gameSettings->your_botid, true, INT_MIN, INT_MAX, depth, true);
			int x = index % 9;
			int y = (int)((index - x) / 9);
			std::cout << "place_move " << x << " " << y << std::endl;
			for (int gridY = 0; gridY < 3; ++gridY)
			{
				for (int y = 0; y < 3; ++y)
				{
					for (int gridX = 0; gridX < 3; ++gridX)
					{
						for (int x = 0; x < 3; ++x)
						{
							switch (gameBoard->field[3 * gridX + x][3 * gridY + y])
							{
							case 1:
								std::cerr << "X";
								break;
							case 2:
								std::cerr << "O";
								break;
							default:
								std::cerr << " ";
								break;
							}
						}
						if (gridX != 2)
							std::cerr << "|";
					}
					std::cerr << std::endl;
				}
				if (gridY != 2)
					std::cerr << "---+---+---" << std::endl;
			}
			std::cerr << std::endl;
			std::cerr << "Score: " << gameBoard->getValue(gameSettings->your_botid) << std::endl << "---------" << std::endl;
		}
	}
	
	/*
	while (gameBoard->winner() == 0)
	{
		std::cout << "Move: " << move << std::endl;
		int index;
		depth = (int)(move / 5) + 1;
		if (move == 1)
		{
			index = rand() % 81;
		}
		else {
			index = alphaBeta(gameBoard, gameSettings->your_botid, gameSettings->your_botid, true, INT_MIN, INT_MAX, depth, true);
		}
		int x = index % 9;
		int y = (int)((index - x) / 9);
		
		std::cout << "Alpha-Beta: place_move " << x << " " << y << std::endl;

		gameBoard->play_move(gameSettings->your_botid, x, y);
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int gridX = 0; gridX < 3; ++gridX)
				{
					for (int x = 0; x < 3; ++x)
					{
						switch (gameBoard->field[3 * gridX + x][3 * gridY + y])
						{
						case 1:
							std::cout << "X";
							break;
						case 2:
							std::cout << "O";
							break;
						default:
							std::cout << " ";
							break;
						}
					}
					if (gridX != 2)
						std::cout << (char)179;
				}
				std::cout << std::endl;
			}
			if (gridY != 2)
				std::cout << (char)196 << (char)196 << (char)196 << (char)197 << (char)196 << (char)196 << (char)196 << (char)197 << (char)196 << (char)196 << (char)196 << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Score: " << gameBoard->getValue(gameSettings->your_botid) << std::endl << "---------" << std::endl;
		gameSettings->your_botid = !(gameSettings->your_botid - 1) + 1;
		move++;
	}
	std::cout << gameBoard->winner() << " Wins!";
	*/
	while (1);
}


/*
for (int gridY = 0; gridY < 3; ++gridY)
{
for (int y = 0; y < 3; ++y)
{
for (int gridX = 0; gridX < 3; ++gridX)
{
for (int x = 0; x < 3; ++x)
{
switch (gameBoard->field[3 * gridX + x][3 * gridY + y])
{
case 1:
std::cout << "X";
break;
case 2:
std::cout << "O";
break;
default:
std::cout << " ";
break;
}
}
if (gridX != 2)
std::cout << (char)179;
}
std::cout << std::endl;
}
if (gridY != 2)
std::cout << (char)196 << (char)196 << (char)196 << (char)197 << (char)196 << (char)196 << (char)196 << (char)197 << (char)196 << (char)196 << (char)196 << std::endl;
}
std::cout << std::endl;
*/
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <assert.h>
#include <cstring>
#include <ctime>
#include <vector>
#include <list>
#include <iomanip>
#include <functional>

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
		std::memcpy(newBoard->field, field, sizeof(int) * 9 * 9);
		std::memcpy(newBoard->macroboard, macroboard, sizeof(int) * 3 * 3);
		return newBoard;
	}

	std::list<board*> getChildren(int player)
	{
		std::list<board*> children;
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				if (macroboard[gridX][gridY] == -1)
				{
					for (int y = 0; y < 3; ++y)
					{
						for (int x = 0; x < 3; ++x)
						{
							if (field[3 * gridX + x][3 * gridY + y] == 0)
							{
								board* newBoard = this->copy();
								newBoard->play_move(player, 3 * gridX + x, 3 * gridY + y);
								children.push_back(newBoard);
							}
						}
					}
				}
			}
		}
		return children;
	}

	int winner()
	{
		int player = macroboard[1][1];
		if (player > 0)
		{
			if (player == macroboard[0][0] && player == macroboard[2][2])
				return player;
			if (player == macroboard[2][0] && player == macroboard[0][2])
				return player;
			if (player == macroboard[0][1] && player == macroboard[2][1])
				return player;
			if (player == macroboard[1][0] && player == macroboard[1][2])
				return player;
		}
		player = macroboard[0][0];
		if (player > 0)
		{
			if (player == macroboard[1][0] && player == macroboard[2][0])
				return player;
			if (player == macroboard[0][1] && player == macroboard[0][2])
				return player;
		}
		player = macroboard[2][2];
		if (player > 0)
		{
			if (player == macroboard[2][1] && player == macroboard[2][0])
				return player;
			if (player == macroboard[1][2] && player == macroboard[0][2])
				return player;
		}
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				if (macroboard[x][y] == -1 || macroboard[x][y] == 0)
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
			if (macroboard[0][y] == player && macroboard[1][y] == player && macroboard[2][y] == player)
			{
				return INT_MAX;
			}
			if (macroboard[0][y] == opponent && macroboard[1][y] == opponent && macroboard[2][y] == opponent)
			{
				return INT_MIN;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if (macroboard[x][0] == player && macroboard[x][1] == player && macroboard[x][2] == player)
			{
				return INT_MAX;
			}
			if (macroboard[x][0] == opponent && macroboard[x][1] == opponent && macroboard[x][2] == opponent)
			{
				return INT_MIN;
			}
		}
		if (macroboard[0][0] == player && macroboard[1][1] == player && macroboard[2][2] == player)
			return INT_MAX;
		if (macroboard[2][0] == player && macroboard[1][1] == player && macroboard[0][2] == player)
			return INT_MAX;
		if (macroboard[0][0] == opponent && macroboard[1][1] == opponent && macroboard[2][2] == opponent)
			return INT_MIN;
		if (macroboard[2][0] == opponent && macroboard[1][1] == opponent && macroboard[0][2] == opponent)
			return INT_MIN;
		int score = 0;
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				if (macroboard[gridX][gridY] == 0 || macroboard[gridX][gridY] == -1)
				{
					for (int y = 0; y < 3; ++y) //Horizontal
					{
						int playerCount = 0;
						int opponentCount = 0;
						for (int x = 0; x < 3; ++x)
						{
							if (field[3 * gridX + x][3 * gridY + y] == player)
							{
								playerCount++;
							}
							else if (field[3 * gridX + x][3 * gridY + y] == opponent)
							{
								opponentCount++;
							}
						}
						if (opponentCount == 0)
						{
							score += pow(playerCount, 2);
						}
						else if (playerCount == 0)
						{
							score -= pow(opponentCount, 2);
						}
					}
					for (int x = 0; x < 3; ++x) //Vertical
					{
						int playerCount = 0;
						int opponentCount = 0;
						for (int y = 0; y < 3; ++y)
						{
							if (field[3 * gridX + x][3 * gridY + y] == player)
							{
								playerCount++;
							}
							else if (field[3 * gridX + x][3 * gridY + y] == opponent)
							{
								opponentCount++;
							}
						}
						if (opponentCount == 0)
						{
							score += pow(playerCount, 2);
						}
						else if (playerCount == 0)
						{
							score -= pow(opponentCount, 2);
						}
					}
					int playerCount = 0;
					int opponentCount = 0;
					//Top-Left to Bottom-Right Diagonal
					for (int i = 0; i < 3; ++i)
					{
						if (field[3 * gridX + i][3 * gridY + i] == player)
						{
							playerCount++;
						}
						else if (field[3 * gridX + i][3 * gridY + i] == opponent)
						{
							opponentCount++;
						}
					}
					if (opponentCount == 0)
					{
						score += pow(playerCount, 2);
					}
					else if (playerCount == 0)
					{
						score -= pow(opponentCount, 2);
					}
					playerCount = 0;
					opponentCount = 0;
					//Top-Right to Bottom-Left Diagonal
					for (int i = 0; i < 3; ++i)
					{
						if (field[3 * gridX + 2 - i][3 * gridY + i] == player)
						{
							playerCount++;
						}
						else if (field[3 * gridX + 2 - i][3 * gridY + i] == opponent)
						{
							opponentCount++;
						}
					}
					if (opponentCount == 0)
					{
						score += pow(playerCount, 2);
					}
					else if (playerCount == 0)
					{
						score -= pow(opponentCount, 2);
					}
				}
			}
		}
		for (int gridY = 0; gridY < 3; ++gridY) //Horizontal
		{
			int playerCount = 0;
			int opponentCount = 0;
			int tieCount = 0;
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				if (macroboard[gridX][gridY] == player)
				{
					playerCount++;
				}
				else if (macroboard[gridX][gridY] == opponent)
				{
					opponentCount++;
				}
				else if (macroboard[gridX][gridY] == -2)
				{
					tieCount++;
				}
			}
			if (tieCount == 0)
			{
				if (opponentCount == 0)
				{
					score += 25 * pow(playerCount, 2);
				}
				else if (playerCount == 0)
				{
					score -= 25 * pow(opponentCount, 2);
				}
			}
		}
		for (int gridX = 0; gridX < 3; ++gridX) //Vertical
		{
			int playerCount = 0;
			int opponentCount = 0;
			int tieCount = 0;
			for (int gridY = 0; gridY < 3; ++gridY)
			{
				if (macroboard[gridX][gridY] == player)
				{
					playerCount++;
				}
				else if (macroboard[gridX][gridY] == opponent)
				{
					opponentCount++;
				}
				else if (macroboard[gridX][gridY] == -2)
				{
					tieCount++;
				}
			}
			if (tieCount == 0)
			{
				if (opponentCount == 0)
				{
					score += 25 * pow(playerCount, 2);
				}
				else if (playerCount == 0)
				{
					score -= 25 * pow(opponentCount, 2);
				}
			}
		}
		int playerCount = 0;
		int opponentCount = 0;
		int tieCount = 0;
		//Top-Left to Bottom-Right Diagonal
		for (int i = 0; i < 3; ++i)
		{
			if (macroboard[i][i] == player)
			{
				playerCount++;
			}
			else if (macroboard[i][i] == opponent)
			{
				opponentCount++;
			}
			else if (macroboard[i][i] == -2)
			{
				tieCount++;
			}
		}
		if (tieCount == 0)
		{
			if (opponentCount == 0)
			{
				score += 25 * pow(playerCount, 2);
			}
			else if (playerCount == 0)
			{
				score -= 25 * pow(opponentCount, 2);
			}
		}

		playerCount = 0;
		opponentCount = 0;
		tieCount = 0;
		//Top-Right to Bottom-Left Diagonal
		for (int i = 0; i < 3; ++i)
		{
			if (macroboard[2 - i][i] == player)
			{
				playerCount++;
			}
			else if (macroboard[2 - i][i] == opponent)
			{
				opponentCount++;
			}
			else if (macroboard[2 - i][i] == -2)
			{
				tieCount++;
			}
		}
		if (tieCount == 0)
		{
			if (opponentCount == 0)
			{
				score += 25 * pow(playerCount, 2);
			}
			else if (playerCount == 0)
			{
				score -= 25 * pow(opponentCount, 2);
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

		if (field[3 * gridX][y] == field[3 * gridX + 1][y] && field[3 * gridX + 1][y] == field[3 * gridX + 2][y])
			macroboard[gridX][gridY] = player;
		else if (field[x][3 * gridY] == field[x][3 * gridY + 1] && field[x][3 * gridY + 1] == field[x][3 * gridY + 2])
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

struct move {
	int x;
	int y;
	bool bestMove = false;
	int player;
	int score;
	std::list<move*> nextMoves;
	move* getNextMove(int x, int y)
	{
		std::list<move*>::iterator i = nextMoves.begin();
		while (i != nextMoves.end())
		{
			if ((*i)->x == x && (*i)->y == y)
				return (*i);
			i++;
		}
		return NULL;
	}
	int getBestIndex()
	{
		std::list<move*>::iterator i = nextMoves.begin();
		while (i != nextMoves.end())
		{
			if ((*i)->bestMove)
			{
				return 9 * (*i)->y + (*i)->x;
			}
			i++;
		}
		return -1;
	}
	~move()
	{
		while (!nextMoves.empty())
		{
			delete nextMoves.front();
			nextMoves.pop_front();
		}
	}
};

bool evaluateInput(std::string input, settings* s, board* b, int* move, int* timeLeft, int* round)
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
			(*round) = std::stoi(input);
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

int alphaBeta(board* b, int firstMove, char* bestMove, char bestMoves[81], int currDepth, int currPlayer, int scorePlayer, bool maximizing, int alpha, int beta, int* count, int* totalCount, bool getIndex, int startTime)
{
	bool print = true;
	if ((*count) <= 1 || b->winner() != 0)
	{
		(*count) = 0;
		(*totalCount)++;
		return b->getValue(scorePlayer);
	}
	int best = maximizing ? INT_MIN : INT_MAX;
	int bestMoveX = -1;
	int bestMoveY = -1;
	int gridCount = 0;
	int playCount = 0;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			if (b->macroboard[gridX][gridY] == -1)
			{
				gridCount++;
				for (int y = 0; y < 3; ++y)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (b->field[3 * gridX + x][3 * gridY + y] == 0)
						{
							playCount++;
						}
					}
				}
			}
		}
	}
	if ((int)firstMove >= 0)
	{
		int y = (int)firstMove / 9;
		int x = (int)firstMove % 9;
		int gridX = x / 3;
		int gridY = y / 3;
		x = x % 3;
		y = y % 3;

		board* newBoard = b->copy();
		newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
		int nextCount = (int)round(*count / playCount);
		if (nextCount == 0)
		{
			nextCount = 1;
		}
		if (newBoard->macroboard[gridX][gridY] == currPlayer && gridCount <= 2)
		{
			nextCount *= 2;
		}
		(*count) -= nextCount;
		char newBestMove;
		char* newBestMoves = NULL;
		if (currDepth == 0)
		{
			newBestMoves = (char*)malloc(sizeof(char) * 81);
			for (int i = 0; i < 81; ++i)
			{
				newBestMoves[i] = -1;
			}
		}
		int v = alphaBeta(newBoard, -1, &newBestMove, newBestMoves, currDepth + 1, !(currPlayer - 1) + 1, scorePlayer, !maximizing, alpha, beta, &nextCount, totalCount, false, startTime);
		(*count) += nextCount;
		playCount--;

		if (currDepth == 1)
		{
			bestMoves[9 * (3 * gridY + y) + 3 * gridX + x] = newBestMove;
		}

		if (getIndex && print)
		{
			std::cerr << "First guess score: " << v << std::endl;
		}
		delete newBoard;
		if (maximizing)
		{
			if (currDepth == 2 && v > best)
			{
				*bestMove = 9 * (3 * gridY + y) + 3 * gridX + x;
			}
			if (currDepth == 0 && v > best && bestMoves != NULL)
			{
				std::memcpy(bestMoves, newBestMoves, sizeof(char[81]));
			}
			if (getIndex && v > best)
			{
				bestMoveX = 3 * gridX + x;
				bestMoveY = 3 * gridY + y;
			}
			if (v == INT_MAX && getIndex)
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
			if (currDepth == 2 && v < best)
			{
				*bestMove = 9 * (3 * gridY + y) + 3 * gridX + x;
			}
			if (currDepth == 0 && v < best && bestMoves != NULL)
			{
				std::memcpy(bestMoves, newBestMoves, sizeof(bestMoves));
			}
			if (getIndex && v < best)
			{
				bestMoveX = 3 * gridX + x;
				bestMoveY = 3 * gridY + y;
			}
			if (v == INT_MIN && getIndex)
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
				std::cerr << std::endl;
				return 9 * bestMoveY + bestMoveX;
			}
			return beta;
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
					if (b->macroboard[gridX][gridY] == -1)
					{
						if (b->field[3 * gridX + x][3 * gridY + y] == 0)
						{
							if (9 * (3 * gridY + y) + 3 * gridX + x != firstMove)
							{
								board* newBoard = b->copy();
								newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
								int nextCount = (int)round(*count / playCount);
								if (nextCount == 0)
								{
									nextCount = 1;
								}
								if (newBoard->macroboard[gridX][gridY] == currPlayer && gridCount <= 2)
								{
									nextCount *= 2;
								}
								(*count) -= nextCount;
								char newBestMove;
								char* newBestMoves = NULL;
								if (currDepth == 0)
								{
									newBestMoves = (char*)malloc(sizeof(char) * 81);
									for (int i = 0; i < 81; ++i)
									{
										newBestMoves[i] = -1;
									}
								}
								int v = alphaBeta(newBoard, -1, &newBestMove, newBestMoves, currDepth + 1, !(currPlayer - 1) + 1, scorePlayer, !maximizing, alpha, beta, &nextCount, totalCount, false, startTime);
								(*count) += nextCount;
								playCount--;

								if (currDepth == 1)
								{
									bestMoves[9 * (3 * gridY + y) + 3 * gridX + x] = newBestMove;
								}

								if (getIndex && print)
								{
									if (v > 9999)
									{
										std::cerr << " MAX";
									}
									else if (v < -999)
									{
										std::cerr << " MIN";
									}
									else {
										if (v >= 0 && v < 10000)
										{
											std::cerr << " ";
										}
										if (abs(v) < 100)
										{
											std::cerr << " ";
										}
										if (abs(v) < 10)
										{
											std::cerr << " ";
										}
										if (v < -999)
											std::cerr << "-999";
										else
											std::cerr << v;
										std::cerr << "]" << std::setfill('0') << std::setw(4) << (int)((clock() - startTime) / double(CLOCKS_PER_SEC) * 1000) << " ";
									}
								}
								delete newBoard;
								if (maximizing)
								{
									if (currDepth == 2 && v > best)
									{
										*bestMove = 9 * (3 * gridY + y) + 3 * gridX + x;
									}
									if (currDepth == 0 && v > best && bestMoves != NULL)
									{
										std::memcpy(bestMoves, newBestMoves, sizeof(char[81]));
									}
									if (getIndex && v > best)
									{
										bestMoveX = 3 * gridX + x;
										bestMoveY = 3 * gridY + y;
									}
									if (v == INT_MAX && getIndex)
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
									if (currDepth == 2 && v < best)
									{
										*bestMove = 9 * (3 * gridY + y) + 3 * gridX + x;
									}
									if (currDepth == 0 && v < best && bestMoves != NULL)
									{
										std::memcpy(bestMoves, newBestMoves, sizeof(bestMoves));
									}
									if (getIndex && v < best)
									{
										bestMoveX = 3 * gridX + x;
										bestMoveY = 3 * gridY + y;
									}
									if (v == INT_MIN && getIndex)
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
										std::cerr << std::endl;
										return 9 * bestMoveY + bestMoveX;
									}
									return beta;
								}
							}
							else {
								if (getIndex && print)
								{
									std::cerr << "STRT]     ";
								}
							}
						}
						else if (getIndex && print)
						{
							if (b->field[3 * gridX + x][3 * gridY + y] == 1)
								std::cerr << "~~X~]     ";
							else if (b->field[3 * gridX + x][3 * gridY + y] == 2)
								std::cerr << "~~O~]     ";
							else
								std::cerr << "~~~~]     ";
						}
					}
					else if (getIndex && print)
					{
						if (b->field[3 * gridX + x][3 * gridY + y] == 1)
							std::cerr << "  X ]     ";
						else if (b->field[3 * gridX + x][3 * gridY + y] == 2)
							std::cerr << "  O ]     ";
						else
							std::cerr << "    ]     ";
					}
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
		return 9 * bestMoveY + bestMoveX;
	}
	return best;
}

int alphaBetaWithMemory(board* b, move* moves, int memoryDepth, int currDepth, int currPlayer, int scorePlayer, int firstGuess, bool maximizing, int alpha, int beta, int* count, int* totalCount)
{
	if (b->winner() != 0)
	{
		(*count) = 0;
		(*totalCount)++;
		return b->getValue(scorePlayer);
	}
	int playCount = 0;
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
							playCount++;
						}
					}
				}
			}
		}
	}

	if (playCount > *count && currPlayer != scorePlayer)
	{
		(*count)--;
		if (*count < 0)
			*count = 0;
		(*totalCount)++;
		return b->getValue(scorePlayer);
	}

	int best = maximizing ? INT_MIN : INT_MAX;

	if (firstGuess != -1)
	{
		int y = (int)firstGuess / 9;
		int x = (int)firstGuess % 9;
		int gridX = x / 3;
		int gridY = y / 3;
		x = x % 3;
		y = y % 3;
		if (b->macroboard[gridX][gridY] == -1 && b->field[3 * gridX + x][3 * gridY + y] == 0)
		{
			board* newBoard = b->copy();
			newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
			int nextCount = (int)round(*count / playCount);
			if (nextCount <= 0)
			{
				nextCount = 1;
			}
			(*count) -= nextCount;
			move* newMove;
			if (currDepth <= memoryDepth)
				newMove = new move();
			else
				newMove = NULL;
			int v = alphaBetaWithMemory(newBoard, newMove, memoryDepth, currDepth + 1, !(currPlayer - 1) + 1, scorePlayer, -1, !maximizing, alpha, beta, &nextCount, totalCount);
			(*count) += nextCount;
			(*count) *= 2;
			playCount--;
			if (currDepth <= 2)
			{
				newMove->player = !(currPlayer - 1) + 1;
				newMove->score = v;
				newMove->x = 3 * gridX + x;
				newMove->y = 3 * gridY + y;
				moves->nextMoves.push_back(newMove);
				if (moves->nextMoves.size() == 1)
				{
					(*moves->nextMoves.begin())->bestMove = true;
				}
			}
			delete newBoard;
			if (maximizing)
			{
				if (v > best && currDepth <= memoryDepth)
				{
					std::list<move*>::iterator i = moves->nextMoves.begin();
					while (i != moves->nextMoves.end())
					{
						(*i)->bestMove = false;
						i++;
					}
					newMove->bestMove = true;
				}
				best = std::max(best, v);
				alpha = std::max(alpha, best);
			}
			else {
				if (v < best && currDepth <= memoryDepth)
				{
					std::list<move*>::iterator i = moves->nextMoves.begin();
					while (i != moves->nextMoves.end())
					{
						(*i)->bestMove = false;
						i++;
					}
					newMove->bestMove = true;
				}
				best = std::min(best, v);
				beta = std::min(beta, best);
			}

			if (beta <= alpha)
			{
				return beta;
			}
		}
		else {
			firstGuess = -1;
		}
	}
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
						if (b->field[3 * gridX + x][3 * gridY + y] == 0 && 9 * (3 * gridY + y) + 3 * gridX + x != firstGuess)
						{
							board* newBoard = b->copy();
							newBoard->play_move(currPlayer, 3 * gridX + x, 3 * gridY + y);
							int nextCount = (int)round(*count / playCount);
							if (nextCount <= 0)
							{
								nextCount = 1;
							}
							(*count) -= nextCount;
							move* newMove;
							if (currDepth <= memoryDepth)
								newMove = new move();
							else
								newMove = NULL;
							int v = alphaBetaWithMemory(newBoard, newMove, memoryDepth, currDepth + 1, !(currPlayer - 1) + 1, scorePlayer, -1, !maximizing, alpha, beta, &nextCount, totalCount);
							(*count) += nextCount;
							playCount--;
							if (currDepth <= 2)
							{
								newMove->player = !(currPlayer - 1) + 1;
								newMove->score = v;
								newMove->x = 3 * gridX + x;
								newMove->y = 3 * gridY + y;
								moves->nextMoves.push_back(newMove);
								if (moves->nextMoves.size() == 1)
								{
									(*moves->nextMoves.begin())->bestMove = true;
								}
							}
							delete newBoard;
							if (maximizing)
							{
								if (v > best && currDepth <= memoryDepth)
								{
									std::list<move*>::iterator i = moves->nextMoves.begin();
									while (i != moves->nextMoves.end())
									{
										(*i)->bestMove = false;
										i++;
									}
									newMove->bestMove = true;
								}
								best = std::max(best, v);
								alpha = std::max(alpha, best);
							}
							else {
								if (v < best && currDepth <= memoryDepth)
								{
									std::list<move*>::iterator i = moves->nextMoves.begin();
									while (i != moves->nextMoves.end())
									{
										(*i)->bestMove = false;
										i++;
									}
									newMove->bestMove = true;
								}
								best = std::min(best, v);
								beta = std::min(beta, best);
							}

							if (beta <= alpha)
							{
								return beta;
							}
						}
					}
				}
			}
		}
	}
	return best;
}

int main()
{

	settings* gameSettings = new settings();
	board* gameBoard = new board();
	int currMove = 1;
	//int lastTime = timeLeft;
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

	int timePool;
	int expectedMoveCount = 10;
	int countPerMs = 0;
	int round;
	int indices[81];
	for (int i = 0; i < 81; ++i)
		indices[i] = -1;
	board* prevBoard = new board();
	bool predictMove = false;
	move* newMoves = new move();
	move* bestMove = NULL;
	while (gameBoard->winner() == 0)
	{
		do
		{
			std::getline(std::cin, line);
		} while (evaluateInput(line, gameSettings, gameBoard, &currMove, &timePool, &round));

		std::cerr << "Move: " << currMove << std::endl;

		if (currMove == 1)
		{
			std::cout << "place_move 4 4" << std::endl;
			std::cout.flush();
			indices[0] = 4 * 9 + 4;
		}
		else if (indices[(currMove - 1) / 2] != -1)
		{
			int index = indices[(currMove - 1) / 2];
			int x = index % 9;
			int y = (int)((index - x) / 9);
			std::cerr << "Move already found: " << x << "," << y << std::endl;
			std::cout << "place_move " << x << " " << y << std::endl;
			std::cout.flush();
		}
		else {
			int start_time = clock();
			int time = timePool / expectedMoveCount;
			if (timePool < 4000)
				time /= 2;
			std::cerr << "Time pool: " << timePool << " ms" << std::endl;
			std::cerr << "Time allocated: " << time << " ms" << std::endl;
			if (countPerMs == 0)
			{
				int currTime = clock();
				int testTotalCount = 0;
				while (clock() / double(CLOCKS_PER_SEC) * 1000 < currTime / double(CLOCKS_PER_SEC) * 1000 + 10)
				{
					int testCount = 100;
					move* testMoves = new move();
					alphaBetaWithMemory(gameBoard, testMoves, 2, 0, gameSettings->your_botid, gameSettings->your_botid, -1, true, INT_MIN, INT_MAX, &testCount, &testTotalCount);
				}
				countPerMs = testTotalCount / 10;
			}
			if (bestMove != NULL)
			{
				for (int y = 0; y < 9; ++y)
				{
					for (int x = 0; x < 9; ++x)
					{
						if (gameBoard->field[x][y] != prevBoard->field[x][y])
						{
							if (bestMove != NULL)
								bestMove = bestMove->getNextMove(x, y);
						}
					}
				}
			}
			int bestMoveIndex = -1;
			if (bestMove != NULL)
			{
				bestMoveIndex = bestMove->getBestIndex();
				int bestMoveScore = bestMove->getNextMove(bestMoveIndex % 9, bestMoveIndex / 9)->score;
				std::cerr << "Best guess: " << bestMoveIndex % 9 << "," << bestMoveIndex / 9 << " with score " << bestMoveScore << std::endl;
			}
			else {
				std::cerr << "No best guess." << std::endl;
			}
			delete newMoves;
			newMoves = new move();
			std::cerr << countPerMs << " nodes analyzed per ms" << std::endl;
			int count = countPerMs * time;
			int newCount = count;
			std::cerr << count << " nodes to be analyzed in " << time << " ms" << std::endl;
			int originalCount = count;
			int totalCount = 0;
			int startT = clock();
			//int index = alphaBeta(gameBoard, bestMove, NULL, bestMoves, 0, gameSettings->your_botid, gameSettings->your_botid, true, INT_MIN, INT_MAX, &count, &totalCount, true, clock());
			int newTotalCount = 0;
			int score = alphaBetaWithMemory(gameBoard, newMoves, 2, 0, gameSettings->your_botid, gameSettings->your_botid, bestMoveIndex, true, INT_MIN, INT_MAX, &count, &totalCount);
			int stopT = clock();
			int index = newMoves->getBestIndex();
			int x = index % 9;
			int y = (int)((index - x) / 9);

			std::cout << "place_move " << x << " " << y << std::endl;
			std::cout.flush();
			int stop_time = clock();
			indices[(currMove - 1) / 2] = index;
			bestMove = newMoves->getNextMove(x, y);
			std::cerr << "Best move: " << x << "," << y << " with score: " << score << std::endl;
			gameBoard->play_move(gameSettings->your_botid, x, y);
			prevBoard = gameBoard->copy();

			int timeInMs = (stopT - startT) / double(CLOCKS_PER_SEC) * 1000;
			if (timeInMs > 0)
				countPerMs = totalCount / timeInMs;
			else
				countPerMs = 0;

			if (expectedMoveCount > 2)
				expectedMoveCount--;
			std::cerr << totalCount << "(" << (double)totalCount / originalCount * 100 << "%) nodes actually analyzed in " << (stop_time - start_time) / double(CLOCKS_PER_SEC) * 1000 << " ms" << std::endl;
		}
	}
	while (1);
}
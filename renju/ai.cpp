//#include"renjuView.h"
#include"stdafx.h"
#include <cstdlib>
#include <ctime>
#include<string>
#define BLACK 1
#define WHITE 2
#define NOCHESS 0
#define board_size 15
using namespace std;
int PosValue[15][15] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
unsigned char current_board[15][15];
unsigned char chess_board[15][15];
unsigned char test_board[21][21];
enum types { not_analysed,ooooo,poooop, oooop, ooopo, oopoo, ppooopp, ooopp,popoop, oppoo, opopo, pppooppp, ooppp, ppopopp, poppop ,analysed};
char* screen[] = { 0,0,0,"AAAA?","AAA?A","AA?AA","??AAA??","AAA??","?A?AA?","A??AA","A?A?A","???AA???","AA???","??A?A??","?A??A?" };
unsigned char typecount[16][3] = { { 0,0 } };//第一是black，第二是white
unsigned char banalysed[15][15][4];
unsigned char one_line_result[15];
int ai_colour, id, rival_colour;
const int weights[] = { 0,0,0,2500,3000,2600,3000,500,800,600,550,650,150,250,200 };
int analyse_a_line(unsigned char line[], int length, int pos)
{
	int leftbound, rightbound = leftbound = pos;
	if (length < 5)
	{
		memset(one_line_result, analysed, length);
		return 0;
	}
	memset(one_line_result, 0, sizeof(one_line_result));
	while (leftbound>=1&&line[leftbound-1]==line[pos])
	{
		leftbound--;
	}
	while (rightbound<length-1&&line[rightbound+1]==line[pos])
	{
		rightbound++;
	}
	int charlength = rightbound - leftbound + 1;
	for (int i = leftbound; i <= rightbound; i++)
	{
		one_line_result[i] = analysed;
	}
	if (charlength >= 5)
	{
		one_line_result[pos] = ooooo;
		return 5;
	}
	else if (charlength==4)
	{
		if (rightbound + 1 < length&&line[rightbound + 1] == NOCHESS)
		{
			if (leftbound - 1 >=0&&line[leftbound - 1] == NOCHESS)
			{
				one_line_result[pos] = poooop;
				return 4;
			}
			else
			{
				one_line_result[pos] = oooop;
			}
		}
		else
		{
			if (leftbound - 1 >= 0 && line[leftbound - 1] == NOCHESS)
			{
				one_line_result[pos] = oooop;
			}
		}
	}
	else if (charlength == 3)
	{
		if (rightbound + 2 < length&&leftbound - 2 >= 0)
		{
			int sum = 0;
			for (int i = leftbound - 2; i < rightbound + 2; i++)
			{
				sum += line[i];
			}
			if (sum == charlength * line[pos])
			{
				one_line_result[pos] = ppooopp;
				return 0;
			}
		}
		if (rightbound + 2 < length&&line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == line[pos])
		{
			one_line_result[pos] = ooopo;
		}
		else	if (rightbound + 2 >= length&&leftbound - 2 >= 0 && line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == line[pos])
		{
			one_line_result[pos] = ooopo;
		}
		else if (rightbound + 2 < length&&line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == NOCHESS)
		{
			one_line_result[pos] = ooopp;
		}
		else if (rightbound + 2 >= length&&leftbound - 2 >= 0 && line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == NOCHESS)
		{
			one_line_result[pos] = ooopp;
		}
	}
	else if (charlength == 2)
	{
		if (rightbound + 3 < length&&line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == line[pos] && line[rightbound + 3] == line[pos])
		{
			one_line_result[pos] = oopoo;
			one_line_result[rightbound + 2] = one_line_result[rightbound + 3] = analysed;
			return 0;
		}
		else if (leftbound - 3 >= 0 && line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == line[pos] && line[leftbound - 3] == line[pos])
		{
			one_line_result[pos] = oopoo;
			one_line_result[leftbound - 2] = one_line_result[leftbound - 3] = analysed;
			return 0;
		}
		if (rightbound + 3 < length&&leftbound - 1 >= 0)
		{
			if (line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == line[pos] && line[rightbound + 3] == NOCHESS&&line[leftbound - 1] == NOCHESS)
			{
				one_line_result[pos] = popoop;
				return 0;
			}
		}
		 if (leftbound - 3 >= 0 && rightbound + 1 < length)
		{
			if (line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == line[pos] && line[leftbound - 3] == NOCHESS&&line[rightbound + 1] == NOCHESS)
			{
				one_line_result[pos] = popoop;
				return 0;
			}
		}
		 if (rightbound + 3 < length&&leftbound - 3 >= 0)
		{
			if (line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == NOCHESS && line[rightbound + 3] == NOCHESS)
			{
				if (line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == NOCHESS && line[leftbound - 3] == NOCHESS)
				{
					one_line_result[pos] = pppooppp;
					return 0;
				}
			}
		}
		 if (rightbound + 3 < length&&line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == NOCHESS && line[rightbound + 3] == line[pos])
		{
			one_line_result[pos] = oppoo;
			return 0;
		}
		else if (leftbound - 3 >= 0 && line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == NOCHESS && line[leftbound - 3] == line[pos])
		{
			one_line_result[pos] = oppoo;
			return 0;
		}
		else if (rightbound + 3 < length&&line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == NOCHESS && line[rightbound + 3] == NOCHESS)
		{
			one_line_result[pos] = ooppp;
			return 0;
		}
		else if (leftbound - 3 >= 0 && line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == NOCHESS && line[leftbound - 3] == NOCHESS)
		{
			one_line_result[pos] = ooppp;
			return 0;
		}
	}
	else if (charlength == 1)
	{
		if (rightbound + 4<length)
		{
			if (line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == line[pos] && line[rightbound + 3] == NOCHESS&&line[rightbound + 4] == line[pos])
			{
				one_line_result[pos] = opopo;
				one_line_result[rightbound + 2] = one_line_result[rightbound + 4] = analysed;
				return 0;
			}
			if (leftbound - 2 >= 0)
			{
				if (line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == line[pos] && line[rightbound + 3] == NOCHESS&&line[rightbound + 4] == NOCHESS&&line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == NOCHESS)
				{
					one_line_result[pos] = ppopopp;
					one_line_result[rightbound + 2] = analysed;
					return 0;
				}
			}
			if (leftbound - 1 >= 0)
			{
				if (line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == NOCHESS && line[rightbound + 3] == line[pos] && line[rightbound + 4] == NOCHESS&&line[leftbound - 1] == NOCHESS)
				{
					one_line_result[pos] = poppop;
					one_line_result[rightbound + 3] = analysed;
					return 0;
				}
			}
		}
	}
	return 0;
}
int forecast_a_line(unsigned char line[], int length, int pos)
{
	int leftbound, rightbound = leftbound = pos;
	if (length < 5)
	{
		memset(one_line_result, analysed, length);
		return false;
	}
	memset(one_line_result, 0, sizeof(one_line_result));
	while (leftbound >= 1 && line[leftbound - 1] == line[pos])
	{
		leftbound--;
	}
	while (rightbound<length - 1 && line[rightbound + 1] == line[pos])
	{
		rightbound++;
	}
	int charlength = rightbound - leftbound + 1;
	for (int i = leftbound; i <= rightbound; i++)
	{
		one_line_result[i] = analysed;
	}
	if (charlength >= 5)
	{
		return 5;
	}
	else if (charlength == 4)
	{
		if (rightbound + 1 < length&&line[rightbound + 1] == NOCHESS)
		{
			if (leftbound - 1 >= 0 && line[leftbound - 1] == NOCHESS)
			{
				return 4;
			}
			else
			{
				return 3;
			}
		}
		if (leftbound - 1 >= 0 && line[leftbound - 1] == NOCHESS)
		{
			return 3;
		}
	}
	else if (charlength==3)
	{
		if (rightbound+2<length)
		{
			if (line[rightbound+1]==NOCHESS&&line[rightbound+2]==line[pos])
			{
				return 6;
			}
		}
		if (leftbound - 2>=0)
		{
			if (line[leftbound - 1] == NOCHESS&&line[leftbound - 2] == line[pos])
			{
				return 6;
			}
		}
	}
	else if (charlength == 2)
	{
		if (rightbound + 3<length)
		{
			if (line[rightbound + 1] == NOCHESS&&line[rightbound + 2] == line[pos] && line[rightbound + 3] == line[pos])
			{
				return 6;
			}
		}
	}
	return 0;
}
int analyse_horizon(unsigned char current[][15], int row, int col)
{
	int result=analyse_a_line(current[row], 15, col);
	if (result)
		return result;
	for (int i = 0; i < 15; i++)
	{
		if (one_line_result[i]!=not_analysed)
		{
			banalysed[row][i][0] = one_line_result[i];
		}
	}
	return 0;
}
int analyse_verti(unsigned char current[][15], int row, int col)
{
	unsigned char column[15];
	for (int i = 0; i < 15; i++)
	{
		column[i] = current[i][col];
	}
	int result=analyse_a_line(column, 15, row);
	if (result)
		return result;
	for (int i = 0; i < 15; i++)
	{
		if (one_line_result[i] != not_analysed)
		{
			banalysed[row][i][1] = one_line_result[i];
		}
	}
	return 0;
}
int analyse_downright(unsigned char current[][15], int row, int col)
{
	unsigned char slash[15];
	int x, y;
	if (row<col)
	{
		x = 0;
		y = col - row;
	}
	else
	{
		y = 0;
		x = row - col;
	}
	int i;
	for (i = 0; x+i<15&&y+i<15; i++)
	{
		slash[i] = current[x + i][y + i];
	}
	int result=analyse_a_line(slash, i, col - y);
	if (result)
		return result;
	for (int j = 0; j < i; j++)
	{
		if (one_line_result[j] != not_analysed)
		{
			banalysed[x+j][y+j][2] = one_line_result[j];
		}
	}
	return 0;
}
int analyse_downleft(unsigned char current[][15], int row, int col)
{
	unsigned char slash[15];
	int x, y;
	if (row + col < 15)
	{
		y = 0;
		x = row + col;
	}
	else
	{
		x = 14;
		y = row + col - 14;
	}
	int i;
	for (i = 0; x - i>=0 && y + i<15; i++)
	{
		slash[i] = current[x - i][y + i];
	}
	int result=analyse_a_line(slash, i, col - y);
	if (result)
		return result;
	for (int j = 0; j < i; j++)
	{
		if (one_line_result[j] != not_analysed)
		{
			banalysed[x - j][y + j][3] = one_line_result[j];
		}
	}
	return 0;
}
int forecast_horizon(unsigned char current[][15], int row, int col)
{
	int result = forecast_a_line(current[row], 15, col);
	if (result)
		return result;
	for (int i = 0; i < 15; i++)
	{
		if (one_line_result[i] != not_analysed)
		{
			banalysed[row][i][0] = one_line_result[i];
		}
	}
	return 0;
}
int forecast_verti(unsigned char current[][15], int row, int col)
{
	unsigned char column[15];
	for (int i = 0; i < 15; i++)
	{
		column[i] = current[i][col];
	}
	int result = forecast_a_line(column, 15, row);
	if (result)
		return result;
	for (int i = 0; i < 15; i++)
	{
		if (one_line_result[i] != not_analysed)
		{
			banalysed[row][i][1] = one_line_result[i];
		}
	}
	return 0;
}
int forecast_downright(unsigned char current[][15], int row, int col)
{
	unsigned char slash[15];
	int x, y;
	if (row<col)
	{
		x = 0;
		y = col - row;
	}
	else
	{
		y = 0;
		x = row - col;
	}
	int i;
	for (i = 0; x + i<15 && y + i<15; i++)
	{
		slash[i] = current[x + i][y + i];
	}
	int result = forecast_a_line(slash, i, col - y);
	if (result)
		return result;
	for (int j = 0; j < i; j++)
	{
		if (one_line_result[j] != not_analysed)
		{
			banalysed[x + j][y + j][2] = one_line_result[j];
		}
	}
	return 0;
}
int forecast_downleft(unsigned char current[][15], int row, int col)
{
	unsigned char slash[15];
	int x, y;
	if (row + col < 15)
	{
		y = 0;
		x = row + col;
	}
	else
	{
		x = 14;
		y = row + col - 14;
	}
	int i;
	for (i = 0; x - i >= 0 && y + i<15; i++)
	{
		slash[i] = current[x - i][y + i];
	}
	int result = forecast_a_line(slash, i, col - y);
	if (result)
		return result;
	for (int j = 0; j < i; j++)
	{
		if (one_line_result[j] != not_analysed)
		{
			banalysed[x - j][y + j][3] = one_line_result[j];
		}
	}
	return 0;
}
int return_value(unsigned char c, int result)
{
	if (ai_colour==WHITE)
	{
		if (result == 4)
		{
			if (c == BLACK)
				return -300000;
			else
			{
				return 300000;
			}
		}
		else
		{
			if (c == BLACK)
				return -900000;
			else
			{
				return 900000;
			}
		}
	}
	else
	{
		if (result == 4)
		{
			if (c == BLACK)
				return 300000;
			else
			{
				return -300000;
			}
		}
		else
		{
			if (c == BLACK)
				return 900000;
			else
			{
				return -900000;
			}
		}
	}
}
int forecast(unsigned char current[][15],int color)
{
	memset(banalysed, 0, sizeof(banalysed));
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (current[i][j] == color)
			{
				if (banalysed[i][j][0] == not_analysed)
				{
					int f = forecast_horizon(current, i, j);
					if (f)
						return f;
				}
				if (banalysed[i][j][1] == not_analysed)
				{
					int f = forecast_verti(current, i, j);
					if (f)
						return f;
				}
				if (banalysed[i][j][2] == not_analysed)
				{
					int f = forecast_downright(current, i, j);
					if (f)
						return f;
				}
				if (banalysed[i][j][3] == not_analysed)
				{
					int f = forecast_downleft(current, i, j);
					if (f)
						return f;
				}
			}
		}
	}
	return false;
}
int evaluate(unsigned char current[][15])
{
	memset(banalysed, 0, sizeof(banalysed));	
	memset(typecount, 0, sizeof(typecount));
	int result = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (current[i][j] != NOCHESS)
			{
				if (banalysed[i][j][0] == not_analysed)
				{
					result=analyse_horizon(current, i, j);
					if (result)
					{
						return return_value(current[i][j], result);
					}
				}
				if (banalysed[i][j][1] == not_analysed)
				{
					result=analyse_verti(current, i, j);
					if (result)
					{
						return return_value(current[i][j], result);
					}
				}
				if (banalysed[i][j][2] == not_analysed)
				{
					result=analyse_downright(current, i, j);
					if (result)
					{
						return return_value(current[i][j], result);
					}
				}
				if (banalysed[i][j][3] == not_analysed)
				{
					result=analyse_downleft(current, i, j);
					if (result)
					{
						return return_value(current[i][j], result);
					}
				}
			}
		}
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (current[i][j] != NOCHESS)
			{
				int chess_type = current[i][j];
				for (int k = 0; k < 4; k++)
				{
					typecount[banalysed[i][j][k]][chess_type]++;
				}
			}
		}
	}
	if (ai_colour==WHITE)
	{
		if (typecount[ooooo][BLACK])
			return -900000;
		if (typecount[ooooo][WHITE])
			return 900000;
		if (typecount[poooop][BLACK])
			return -300000;
		if (typecount[poooop][WHITE])
			return 300000;
	}
	else
	{
		if (typecount[ooooo][BLACK])
			return 900000;
		if (typecount[ooooo][WHITE])
			return -900000;
		if (typecount[poooop][BLACK])
			return 300000;
		if (typecount[poooop][WHITE])
			return -300000;
	}
	int bvalue = 0, wvalue = 0;
	for (int i = 3; i < 15; i++)
	{
		/*if (typecount[i][BLACK] != 0)
			cout << "black " << screen[i]<<int(typecount[i][BLACK]) << endl;
		if (typecount[i][WHITE] != 0)
			cout << "white " << screen[i] << int(typecount[i][WHITE]) << endl;*/
			bvalue += typecount[i][BLACK] * weights[i];
			wvalue += typecount[i][WHITE] * weights[i];
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (current[i][j]!=NOCHESS)
			{
				if (current[i][j]==BLACK)
				{
					bvalue += PosValue[i][j];
				}
				else
				{
					wvalue += PosValue[i][j];
				}
			}
		}
	}
	if (ai_colour==WHITE)
	{
		return wvalue - bvalue;
	}
	else
	{
		return bvalue - wvalue;
	}
}
int X, Y;
int search_depth, max_depth, move_count;
struct chess_move
{
	int x, y, score;
}best_move,move_list[10][225],buffer[225];
void merge(chess_move src[], chess_move dst[], int l, int m, int r)
{
	int i = l;
	int j = m + 1;
	int k = l;
	while (i <= m && j <= r)
		if (src[i].score <= src[j].score)
			dst[k++] = src[i++];
		else
			dst[k++] = src[j++];
	if (i>m)
		for (int q = j; q <= r; q++)
			dst[k++] = src[q];
	else
		for (int q = i; q <= m; q++)
			dst[k++] = src[q];
}
void reverse_merge(chess_move src[], chess_move dst[], int l, int m, int r)
{
	int i = l;
	int j = m + 1;
	int k = l;
	while (i <= m && j <= r)
		if (src[i].score >= src[j].score)
			dst[k++] = src[i++];
		else
			dst[k++] = src[j++];
	if (i>m)
		for (int q = j; q <= r; q++)
			dst[k++] = src[q];
	else
		for (int q = i; q <= m; q++)
			dst[k++] = src[q];
}
void mini_merge_sort(chess_move src[], chess_move dst[], int in, int n, bool di)
{
	int i = 0;
	while (i <= n - 2 * in)
	{
		if (di)
			merge(src, dst, i, i + in - 1, i + 2 * in - 1);
		else
			reverse_merge(src, dst, i, i + in - 1, i + 2 * in - 1);
		i = i + 2 * in;
	}
	if (i + in<n)
	{
		if (di)
			merge(src, dst, i, i + in - 1, n - 1);
		else
			reverse_merge(src, dst, i, i + in - 1, n - 1);
	}
	else
		for (int j = i; j <= n - 1; j++)
			dst[j] = src[j];
}
void merge_sort(chess_move src[], int n, bool di)
{
	int in = 1;
	while (in<n)
	{
		mini_merge_sort(src, buffer,in, n, di);
		in += in;
		mini_merge_sort( buffer, src,in, n, di);
		in += in;
	}
}
void make_a_move(chess_move m, int minmax)
{
	if (minmax == 0)
	{
		current_board[m.x][m.y] = ai_colour;
		test_board[m.x + 3][m.y + 3] = ai_colour;
	}
	else
	{
		current_board[m.x][m.y] = rival_colour;
		test_board[m.x + 3][m.y + 3] = rival_colour;
	}
	//for (int i = 0; i < 15; i++)
	//{
	//	for (int j = 0; j < 15; j++)
	//	{
	//		if (current_board[i][j] == WHITE)
	//			printf("O ");
	//		else  if (current_board[i][j] == BLACK)
	//			printf("X ");
	//		else
	//			printf("+ ");
	//	}
	//	printf("\n");
	//}
	//printf("\n");
}
void unmake_a_move(chess_move m)
{
	current_board[m.x][m.y] = NOCHESS;
	test_board[m.x + 3][m.y + 3] = NOCHESS;
}
void addmove(int x, int y, int depth)
{
	move_list[depth][move_count].x = x;
	move_list[depth][move_count].y = y;
	int minmax = (max_depth - depth) % 2;
	if (depth>=2)
	{
		make_a_move(move_list[depth][move_count], minmax);
		move_list[depth][move_count].score = evaluate(current_board);
		unmake_a_move(move_list[depth][move_count]);
	}
	else
	{
		move_list[depth][move_count].score = PosValue[x][y];
	}
	move_count++;
}
bool empty(int x, int y,int minmax)
{
	int xup = x - 1+3, xdown = x + 1+3, yleft = y -1+3, yright = y + 1+3;
	for (int i = xup; i <= xdown; i++)
	{
		for (int j = yleft; j <= yright; j++)
		{
			if (i == x&&j == y)
				continue;
			if (test_board[i][j] != NOCHESS)
			{
				return false;
			}
		}
	}
	xup--, xdown++, yleft--, yright++;
	if (test_board[xup][yleft]==minmax+1&&test_board[xup-1][yleft-1] == minmax+1)
	{
		return false;
	}
	if (test_board[xup][y] == minmax+1&&test_board[xup - 1][y] == minmax+1)
	{
		return false;
	}
	if (test_board[xup][yright] == minmax+1&&test_board[xup - 1][yright + 1] == minmax+1)
	{
		return false;
	}
	if (test_board[x][yleft] == minmax+1&&test_board[x][yleft-1] == minmax+1)
	{
		return false;
	}
	if (test_board[x][yright] == minmax+1&&test_board[x][yright + 1] == minmax+1)
	{
		return false;
	}
	if (test_board[xdown][yleft] == minmax+1&&test_board[xdown+1][yleft - 1] == minmax+1)
	{
		return false;
	}
	if (test_board[xdown][y] == minmax+1&&test_board[xdown + 1][y] == minmax+1)
	{
		return false;
	}
	if (test_board[xdown][yright] == minmax+1&&test_board[xdown + 1][yright + 1] == minmax+1)
	{
		return false;
	}
	return true;
}
int make_possible_move(int depth,int minmax)
{
	//int minpos = 7;
	//for (int i = 0; i < 15; i++)
	//{
	//	for (int j = 0; j < 15; j++)
	//	{
	//		if (current_board[i][j] != NOCHESS&&PosValue[i][j] < minpos)
	//			minpos = PosValue[i][j];
	//	}
	//}
	//int upleft = minpos - 2;
	//if (upleft<0)
	//{
	//	upleft = 0;
	//}
	move_count = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (current_board[i][j]==NOCHESS&&!empty(i,j,minmax))
			{
				addmove(i, j, depth);
			}
		}
	}
	if (minmax == 0)
		merge_sort(move_list[depth], move_count, 0);
	else
	{
		merge_sort(move_list[depth], move_count, 1);
	}
	return move_count;
}
int negascout(int depth, int alpha, int beta)
{
	//static bool first = true;
	int minmax = (max_depth - depth) % 2;
	int score;
	if (depth <= 0)
	{
		score = evaluate(current_board);
		return score;
	}
	int v;
	int counts = make_possible_move(depth,minmax);
	if (counts>20&&depth>=2)
	{
		counts = 20;
	}
	if (minmax==0)
	{
		for (int i = 0; i < counts; i++)
		{
			make_a_move(move_list[depth][i], minmax);
			int ai = forecast(current_board, ai_colour);
			int rival = forecast(current_board, rival_colour);
			if (ai!=5&&rival>=3)
			{
				unmake_a_move(move_list[depth][i]);
				continue;
			}
			if ((ai == 4 || ai==5)&& rival == 0)
			{
				v = 300000;
				unmake_a_move(move_list[depth][i]);
				if (v>alpha)
				{
					if (v > beta)
					{
						return v;
					}
					else
					{
						if (depth == max_depth)
							best_move = move_list[depth][i];
						alpha = v;
						return v;
					}
				}
				continue;
			}
			if (i!=0)
			{
				v = negascout(depth - 1, beta - 1, beta);
				if (v >= beta)
				{
					unmake_a_move(move_list[depth][i]);
					return v;
				}
			}
			v = negascout(depth - 1, alpha, beta);	
			unmake_a_move(move_list[depth][i]);
			if (v>alpha)
			{
				if (v > beta)
				{
					return v;
				}
				else
				{
					if (depth == max_depth&&v!=-300000)
						best_move = move_list[depth][i];
					alpha = v;
				}				
			}
		}
		return alpha;
	}
	else
	{
		for (int i = 0; i < counts; i++)
		{
			make_a_move(move_list[depth][i], minmax);
			int ai = forecast(current_board, ai_colour);
			int rival = forecast(current_board, rival_colour);
			if (ai >=3 && rival ==0)
			{
				unmake_a_move(move_list[depth][i]);
				continue;
			}
			if ((rival == 4 || rival == 5) && ai == 0)
			{
				v = -300000;
				unmake_a_move(move_list[depth][i]);
				if (v<beta)
				{
					if (v < alpha)
					{
						return v;
					}
					else
					{
						beta = v;
						return v;
					}
				}
				continue;
			}
			if (i!=0)
			{
				v = negascout(depth - 1, alpha, alpha + 1);
				if (v <= alpha)
				{
					unmake_a_move(move_list[depth][i]);
					return v;
				}
			}
			v = negascout(depth - 1, alpha, beta);
			unmake_a_move(move_list[depth][i]);
			if (v<beta)
			{
				if (v < alpha)
				{
					return v;
				}
				else
				{
					beta = v;
				}
			}
		}
		return beta;
	}
}
void search_best_move(unsigned char current[][15])
{
	memcpy(current_board, current, sizeof(current_board));
	memset(test_board, 0, sizeof(test_board));
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			test_board[i + 3][j + 3] = current_board[i][j];
		}
	}
	max_depth = search_depth;
	negascout(max_depth, -1000000, 1000000);
	X = best_move.x; Y = best_move.y;
}
pair<int,int> ai(unsigned char matrix[][15],bool first,int num)
{
	memcpy(chess_board, matrix, 225);
	if (first)
	{
		id = 1;
		ai_colour = BLACK;
		rival_colour = WHITE;
	}
	else
	{
		id = 0;
		ai_colour = WHITE;
		rival_colour = BLACK;
	}
	//cout << evaluate(chess_board);
	//cin.get();
	if (num==0)
	{
		X = Y = 15 / 2;
	}
	else
	{
		if (num == 1)
		{
			X = 6; Y = 7;
		}
		else
		{
			search_depth = 5;
			search_best_move(chess_board);
		}
	}
	if (X==0&&Y==0)
	{
		bool found = false;
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (chess_board[i][j]==NOCHESS)
				{
					X = i; Y = j;
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
	}
	return pair<int, int>(X, Y);
}

//int main(int argc, char *argv[])
//{
//	int SL = 0;
//	for (int i = 0; i < 15; i++)
//		for (int j = 0; j < 15; j++)
//			chess_board[i][j] = NOCHESS; //棋盘初始化 
//	int XS;
//	printf("请选择先手棋手：1表示AI先手 0表示棋手先手\n");
//	cin >> XS; 
//	if (XS)
//	{
//		ai_colour = BLACK;
//		rival_colour = WHITE;
//	}
//	else
//	{
//		ai_colour = WHITE;
//		rival_colour = BLACK;
//	}
//	while (!SL)
//	{
//		int rival_x=-1, rival_y=-1; //用于保存对手上一步落子点 
//		if (!XS)
//		{
//			printf("\n请输入落子坐标：\n");
//			cin >> rival_x >> rival_y; //读入对手上一步落子点  
//		}
//		if (XS == 1 && rival_x == -1 && rival_y == -1) //如果己方执黑且是第一步，则占据棋盘中心位置 
//		{
//			chess_board[15 / 2][15 / 2] = ai_colour; //更新棋盘信息
//			X = Y = 15 / 2;
//			XS = 0;
//		}
//		else
//		{
//			chess_board[rival_x][rival_y] = rival_colour;
//			//更新棋盘信息 
//			for (int i = 0; i < 15; i++)
//			{
//				for (int j = 0; j < 15; j++)
//				{
//					if (chess_board[i][j] == WHITE)
//						printf("O "); 
//					else  if (chess_board[i][j] == BLACK)
//						printf("X "); 
//					else 
//						printf("+ ");
//				}
//				printf("\n");
//			}
//			search_depth = 1;
//			search_best_move(chess_board);
//			chess_board[X][Y] = ai_colour;
//		}
//		printf("\nAI落子坐标为：");
//		cout << X << ' ' << Y << endl; //输出 
//		cout << flush; //刷新缓冲区
//		for (int i = 0; i < 15; i++)
//		{
//			for (int j = 0; j < 15; j++)
//			{
//				if (chess_board[i][j] == WHITE)
//					printf("O ");
//				else  if (chess_board[i][j] == BLACK)
//					printf("X ");
//				else
//					printf("+ ");
//			}
//			printf("\n");
//		}
//	}
//}
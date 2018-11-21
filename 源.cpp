#include <stdio.h>
#include <windows.h>
//定义全局变量:
char per, com;
char aha[8][8] = { '\0' };
char hei[8][8] = { '\0' };
char site_discs[8][8];//存储各个点的棋子情况，可以下子的位置标记为'Y'，黑子处标记为'*'，白子处标记为'O'，其余标记为'\0';
double site_score[8][8];//存储各个点的位置分数;
double k_internal[8][8];//记录下此处时k子内部子的变化量;（最终量-初始量）
double p_internal[8][8];//记录下此处时p子内部子的变化量;
double k_eat_num[8][8];//记录下此处时k子吃子数目;
double p_eat_num[8][8];//记录k子吃子后p子可以吃子数量的最大值;
double p_mobility[8][8];//k子吃子后p子的行动力既选择数目;
double score[8][8];//通过对以上参数的加权得到最终分数记录该位置的合适度;
//初始化结构体中变量的函数；
void init(void)
{
	int i, j;
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			site_discs[i][j] = aha[i][j];
			site_score[i][j] = 0.2;
			p_internal[i][j] = 0;
			k_internal[i][j] = 0;
			k_eat_num[i][j] = 0;
			p_eat_num[i][j] = 0;
			p_mobility[i][j] = 0;
			score[i][j] = 0;
		}
	}
	//对site_score赋值;
	site_score[0][0] = site_score[7][7] = site_score[0][7] = site_score[7][0] = 20;
	site_score[1][0] = site_score[0][1] = site_score[6][0] = site_score[0][6] = -2;
	site_score[1][7] = site_score[7][1] = site_score[6][7] = site_score[7][6] = -2;
	site_score[2][0] = site_score[0][2] = site_score[5][0] = site_score[0][5] = 2;
	site_score[2][7] = site_score[7][2] = site_score[5][7] = site_score[7][5] = 2;
	site_score[3][0] = site_score[0][3] = site_score[4][0] = site_score[0][4] = 1;
	site_score[3][7] = site_score[7][3] = site_score[4][7] = site_score[7][4] = 1;
	site_score[1][1] = site_score[1][6] = site_score[6][1] = site_score[6][6] = -9;
	site_score[2][2] = site_score[2][5] = site_score[5][2] = site_score[5][5] = 0.6;
	site_score[2][3] = site_score[2][4] = site_score[3][2] = site_score[4][2] = 0.4;
	site_score[5][3] = site_score[5][4] = site_score[3][5] = site_score[4][5] = 0.4;
	/*对site_score赋值如下;（以下数据*2）;
	{  10,  -1,   1, 0.5, 0.5,  1,   -1,  10},
	{  -1,-4.5, 0.1, 0.1, 0.1, 0.1,-4.5,  -1},
	{   1, 0.1, 0.3, 0.2, 0.2, 0.3, 0.1,   1},
	{ 0.5, 0.1, 0.2, 0.1, 0.1, 0.2, 0.1, 0.5},
	{ 0.5, 0.1, 0.2, 0.1, 0.1, 0.2, 0.1, 0.5},
	{   1, 0.1, 0.3, 0.2, 0.2, 0.3, 0.1,   1},
	{  -1,-4.5, 0.1, 0.1, 0.1, 0.1,-4.5,  -1},
	{  10,  -1,   1, 0.5, 0.5,   1,  -1,  10}
	*/
}
//画棋盘函数；
void draw(char aha[][8])
{
	printf("                       【黑白棋】\n");
	int i, j, black = 0, white = 0;
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			if (aha[i][j] == 'O')
				white++;
			else if (aha[i][j] == '*')
				black++;
		}
	}
	printf("              0   1   2   3   4   5   6   7  \n");
	printf("           ┌─┬─┬─┬─┬─┬─┬─┬─┐\n");
	printf("          a│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│a\n", aha[0][0], aha[0][1], aha[0][2], aha[0][3], aha[0][4], aha[0][5], aha[0][6], aha[0][7]);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          b│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│b\n", aha[1][0], aha[1][1], aha[1][2], aha[1][3], aha[1][4], aha[1][5], aha[1][6], aha[1][7]);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│c\n", aha[2][0], aha[2][1], aha[2][2], aha[2][3], aha[2][4], aha[2][5], aha[2][6], aha[2][7]);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          d│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│d    * :%-2d\n", aha[3][0], aha[3][1], aha[3][2], aha[3][3], aha[3][4], aha[3][5], aha[3][6], aha[3][7], black);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          e│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│e    O :%-2d\n", aha[4][0], aha[4][1], aha[4][2], aha[4][3], aha[4][4], aha[4][5], aha[4][6], aha[4][7], white);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          f│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│f\n", aha[5][0], aha[5][1], aha[5][2], aha[5][3], aha[5][4], aha[5][5], aha[5][6], aha[5][7]);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          g│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│g\n", aha[6][0], aha[6][1], aha[6][2], aha[6][3], aha[6][4], aha[6][5], aha[6][6], aha[6][7]);
	printf("           ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("          h│ %c│ %c│ %c│ %c│ %c│ %c│ %c│ %c│h\n", aha[7][0], aha[7][1], aha[7][2], aha[7][3], aha[7][4], aha[7][5], aha[7][6], aha[7][7]);
	printf("           └─┴─┴─┴─┴─┴─┴─┴─┘\n");
	printf("              0   1   2   3   4   5   6   7  \n");
}
//结束整理分数判定胜负函数；
void end()//结束判定胜负函数；
{
	//计算黑白棋子数；
	int i, j;
	int com_ = 0, per_ = 0;
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			if (aha[i][j] == com)
				com_++;
			else if (aha[i][j] == per)
				per_++;
		}
	}
	//判断胜负；
	if (com_ < per_)
		printf("\t\tyou win!\n\n");
	else if (com_ > per_)
		printf("\t\tyou lose!\n\n");
	else
		printf("\t\tsame score!\n\n");
}
//对search中数据进行标准化处理的函数;(将数据转化成平均数约为1)
void standard(double p[][8])
{
	double aver0 = 0, sum0 = 0, num = 0;
	int i, j;
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			if (site_discs[i][j] == 'Y')
			{
				sum0 += p[i][j];
				num++;
			}
		}
	}
	aver0 = sum0 / num;
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			if (site_discs[i][j] == 'Y')
			{
				if (aver0 != 0)
					p[i][j] /= aver0;
				else
					p[i][j] = 0;
			}
		}
	}
}
//检测棋盘上总共棋子数目的函数；
int number(void)
{
	int num = 0;
	int i, j;
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (aha[i][j] == 'O' || aha[i][j] == '*')
				num++;
	return num;
}
//检测该射线上棋子k是否可以吃子的函数;
int detect_line(char k, int x, int i, int y, int j, char A[][8])
{
	int i0, j0;
	int a = 0;//标记用，作为返回值；
	for (i0 = i, j0 = j; a == 0 && x + i <= 7 && y + j <= 7 && x + i >= 0 && y + j >= 0; i += i0, j += j0)
	{
		if (A[x + i][y + j] == '\0' || A[x + i][y + j] == '-')break;
		if (A[x + i][y + j] == k)a = 1;//可以吃子；
	}
	return a;//可以吃子时返回1，不可以时返回0；
}
//检测棋子m下棋处是否符合规则的函数,合乎规则返回1;
int detect_ok(char m, int x, int y, char A[][8])
{
	int i, j;
	int a = 0;//标记，作为返回值；
	char n;
	if (m == 'O')n = '*';
	else n = 'O';
	if (A[x][y] == n || A[x][y] == m)//该点已有棋子时；
		return a;
	for (i = -1; a == 0 && i <= 1; i++)
	{
		if ((x + i) > 7 || (x + i) < 0)continue;
		for (j = -1; a == 0 && j <= 1; j++)
		{
			if ((y + j) > 7 || (y + j) < 0 || A[x + i][y + j] != n)continue;
			a = detect_line(m, x, i, y, j, A);//调用函数进行检测；
		}
	}
	return a;
}
//检测此处的棋子m是否是内部子,是时返回1;
int detect_internal(char m, int x, int y, char A[][8])
{
	char n;
	int i, j;
	if (m == 'O') n = '*';
	else n = 'O';
	if (A[x][y] != m) return 0;
	if (x < 2 || x > 5 || y < 2 || y > 5) return 0;
	//判断该棋子是否是被对方棋子所围困的棋子;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;
			if (A[x + i][y + j] != n) return 0;
		}
	}
	return 1;
}
//检测棋子m下此处时可以吃掉棋子n的数目的分数；
int detect_eat_num(char m, int x, int y, char A[][8], int x0, int y0)
{
	char n;//x0,y0指上一步中对方棋子的下子位置;
	if (m == '*')n = 'O';
	else n = '*';
	int num = 0;
	int i, j;
	for (i = -1; i <= 1; i++)
	{
		if ((x + i) > 7 || (x + i) < 0)continue;
		for (j = -1; j <= 1; j++)
		{
			if ((y + j) > 7 || (y + j) < 0 || A[x + i][y + j] != n)continue;
			if (detect_line(m, x, i, y, j, A))//判断m是否可以吃子；
			{
				for (int i0 = i, j0 = j; x + i0 <= 7 && y + j0 <= 7 && x + i0 >= 0 && y + j0 >= 0 && A[x + i0][y + j0] == n; i0 += i, j0 += j)
				{
					num++;//每有一个n棋子num就加1；
					if (x + i0 == 0 || x + i0 == 7 || y + j0 == 0 || y + j0 == 7)
					{
						num += 2;//当吃掉的棋子是边缘处的棋子时num再加2;
						if (x + i0 == x0 && y + j0 == y0)
							num += 9;//当上一步中对方棋子下子处为边缘且该棋子下子处可以吃掉该子时增大落子合适度;
					}
				}
			}
		}
	}
	return num;
}
//棋子m吃子函数；
void eat(char m, int x, int y, char A[][8])
{
	int i, j;
	char n;
	if (m == 'O')n = '*';
	else n = 'O';
	for (i = -1; i <= 1; i++)
	{
		if ((x + i) > 7 || (x + i) < 0)continue;
		for (j = -1; j <= 1; j++)
		{
			if ((y + j)>7 || (y + j) < 0 || A[x + i][y + j] != n)continue;
			if (detect_line(m, x, i, y, j, A))//调用函数检测该连续线段上是否还有m；
			{
				int i0 = i, j0 = j;
				do
				{
					A[x + i0][y + j0] = m;
					i0 += i;
					j0 += j;
				} while (A[x + i0][y + j0] == n);//吃子环节；
			}
		}
	}
}
//标记m子可下点处的函数；
int sign_ok(char m, char aha[][8])
{
	int i, j;
	int a = 0;//标记是否存在可下子点；
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (detect_ok(m, i, j, aha))
			{
				a = 1;
				aha[i][j] = '-';
			}
	return a;
}
//由棋盘A得到k子吃子后的棋盘数组B；
void suppose_aftereat(char k, int x, int y, char A[][8], char B[][8])
{
	int i, j;
	for (i = 0; i <= 7; i++)//整体复制；
		for (j = 0; j <= 7; j++)
			B[i][j] = A[i][j];
	B[x][y] = k;
	eat(k, x, y, B);
}
//棋子k分析操作函数，可下子时返回1，不可下时返回0；
int analyse_turn(char k, int x0, int y0)//(x0,y0)指上一步中对方棋子的下子位置；
{
	//定义阶段:
	int i, j, m, n, r;
	int x, y;
	char p;
	double k_int_num = 0, p_int_num = 0;//记录k子还未下子时内部子的数量;
	int a = 0;//标记,当有可下子点时为1,没有可下子点时为0;
	char huhu[8][8];//备用数组;
	int num = number();//定义num为棋盘上棋子的数量;
	if (k == 'O') p = '*';
	else p = 'O';
	//检测棋盘是否已下满,下满时结束程序:
	if (num == 64)
	{
		printf("游戏结束！\n");
		end();
		exit(0);//结束程序；
	}
	//将k子下子之前的黑白内部子数量存储到_int_num中:
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
		{
			k_int_num += detect_internal(k, i, j, aha);
			p_int_num += detect_internal(p, i, j, aha);
		}
	//遍历棋盘上的空白点，并对可落子点的各个因素的参数上进行计算：
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (detect_ok(k, i, j, aha))
			{
				a = 1;
				site_discs[i][j] = 'Y';//将k棋可下子处做标记；
				k_eat_num[i][j] = detect_eat_num(k, i, j, aha, x0, y0);//计算k_eat_num[i][j];
				if (x0 == 0 || x0 == 7 || y0 == 0 || y0 == 7)
				{
					if (i == x0 || j == y0)
						score[i][j] += 5;//当k子假设落子点与上一步中p落子点在棋盘的同一边时增大score[i][j];
				}
				suppose_aftereat(k, i, j, aha, hei);//将aha中假设已下子并吃子后的棋盘记录在hei中;
				//对于k子可以下的每一个位置遍历下一步中p子可以下子的位置的参数进行计算:
				for (m = 0; m <= 7; m++)
					for (n = 0; n <= 7; n++)
					{
						if (detect_ok(p, m, n, hei))
						{
							p_mobility[i][j]++;//循环叠加计算p_mobility[i][j];
							if (site_score[m][n] == 20)
								score[i][j] -= 20;//当位置[m][n]为棋盘角时降低score[i][j];
							suppose_aftereat(p, m, n, hei, huhu);//将k子下子后p子再下子后的棋盘再记录到huhu中;
							//检测p子下此处后k子有无行动力：
							for (r = 0, x = 0; x <= 7; x++)
								for (y = 0; y <= 7; y++)
									r += detect_ok(p, x, y, huhu);
							if (r == 0 )
								score[i][j] -= 20;//若p子下子后k子无行动力则增大p子行动力以降低落子合适度;
							//计算k子下子后p子可下点处吃子数量的分数的最大值:
							if (p_eat_num[i][j] < detect_eat_num(p, m, n, hei, i, j))
								p_eat_num[i][j] = detect_eat_num(p, m, n, hei, i, j);
						}
						else
						{
							//将k子下子后黑白内部子数量存储到_int_num][i][j]中:
							k_internal[i][j] += detect_internal(k, m, n, hei);
							p_internal[i][j] += detect_internal(p, m, n, hei);
						}
					}
				if (p_mobility[i][j] <= 1)
					score[i][j] += 10;//当k子下子后p子行动力极少甚至没有时增大score[i][j];
				k_internal[i][j] -= k_int_num;//计算k_internal[i][j];
				p_internal[i][j] -= p_int_num;//计算p_internal[i][j];
			}
	//通过计算得到每个可下子点最后的评估总分:
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (site_discs[i][j] == 'Y')
			{
				//对得到的数据进行数据处理使其各个因素中的数据的平均值为1;
				standard(k_internal);
				standard(p_internal);
				standard(k_eat_num);
				standard(p_eat_num);
				standard(p_mobility);
				if (num < 16)//k子吃子数量，p子吃子数量，位置，p子行动力，内部子。
				{
					score[i][j] += 0.2 * site_score[i][j] - 0.1 * p_internal[i][j] + 0.1 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.2 * p_eat_num[i][j] - 0.1 * p_mobility[i][j];
				}
				else if (num < 32)//k子吃子数量，p子吃子数量，内部子，位置，p子行动力。
				{
					score[i][j] += 0.15 * site_score[i][j] - 0.15 * p_internal[i][j] + 0.15 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.2 * p_eat_num[i][j] - 0.15 * p_mobility[i][j];
				}
				else if (num < 48)//k子吃子数量，p子吃子数量，内部子，位置，p子行动力。
				{
					score[i][j] += 0.1 * site_score[i][j] - 0.15 * p_internal[i][j] + 0.15 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.2 * p_eat_num[i][j] - 0.1 * p_mobility[i][j];
				}
				else//k子吃子数量，p子吃子数量，p子行动力，位置，内部子。
				{
					score[i][j] += 0.05 * site_score[i][j] - 0.1 * p_internal[i][j] + 0.1 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.3 * p_eat_num[i][j] - 0.1 * p_mobility[i][j];
				}
			}
	if (a == 1)//有可下点时
	{
		x = -1;
		y = -1;
		for (i = 0; i <= 7; i++)
			for (j = 0; j <= 7; j++)
				if (site_discs[i][j] == 'Y')
				{
					if (x == -1 && y == -1)
					{
						x = i;
						y = j;
					}
					if (score[x][y] < score[i][j])
					{
						x = i;
						y = j;
					}
				}
		aha[x][y] = k;
		Sleep(1000);
		system("cls");
		draw(aha);//清屏并画出下子后棋盘；
		eat(k, x, y, aha);//得到吃子后的棋盘函数；
		return 1;
	}
	else
	{
		printf("%c 子已无可下点，%c 子继续下。\n", k, p);
		return 0;
	}
}
//主函数；
int main()
{
	int i, j;
	int y, x;//分别表示横纵坐标，即二维和一维坐标点；
	int a = 0, b = 0;//a标记人是否可以下子，b标记人下子后机是否可以下子;
	char c;
	aha[3][3] = aha[4][4] = 'O';
	aha[3][4] = aha[4][3] = '*';
	draw(aha);//画棋盘;
	//选择棋子颜色及先后阶段：
	printf("\t黑棋B/白棋W？请按键选择B/W:");
	do
	{
		c = getchar();
	} while (c != 'B' && c != 'b' && c != 'W' && c != 'w');
	if (c == 'B' || c == 'b')
	{
		per = '*';
		com = 'O';
	}
	else
	{
		per = 'O';
		com = '*';
	}
	printf("\t您已选择 %c 棋.\n", per);
	printf("\t先下X/后下H？请按键选择X/H:");
	do
	{
		c = getchar();
	} while (c != 'X' && c != 'x' && c != 'H' && c != 'h');
	getchar();
	system("cls");
	draw(aha);
	Sleep(500);
	printf("\t\t按enter键开始游戏\n");
	getchar();
	if (c == 'H' || c == 'h')
	{
		if (per == '*') b = analyse_turn(com, 3, 4);
		else b = analyse_turn(com, 3, 3);
	}
	Sleep(500);
	a = sign_ok(per, aha);//将对于per子而言可以下子的地方做标记；
	b = 0;
	system("cls");
	draw(aha);//画出标记后的棋盘；
	//人机对战循环：
	do
	{
		if (a == 1)
		{
			//人下子输入环节；
			do{
				printf("\t\t%c 下子！\n\t\tit is your turn!", per);
				printf("\n‘-’为可下子位置,请输入您的下子位置（数字+字母）：");
				do
				{
					y = -1;
					c = getchar();
					if (c <= '7' && c >= '0')
						y = c - 48;
				} while (y == -1);
				do
				{
					x = -1;
					c = getchar();
					if (c <= 'h'&&c >= 'a')
						x = c - 97;
				} while (x == -1);
			} while (x == -1 || y == -1 || aha[x][y] != '-');
			//取消之前在棋盘上做的标记点；
			for (i = 0; i <= 7; i++)
			{
				for (j = 0; j <= 7; j++)
					if (aha[i][j] == '-')aha[i][j] = '\0';
			}
			aha[x][y] = per;
			//清屏并打印取消标记后per子下子后的棋盘；
			Sleep(250);
			system("cls");
			draw(aha);
			//清屏并打印per子吃子后棋盘；
			eat(per, x, y, aha);
			Sleep(250);
			system("cls");
			draw(aha);
		}
		else
			printf("\t\t%c 子无处可下，%c 子继续下子!\n", per, com);
		//电脑下子吃子环节并清屏打印环节；
		init();//初始化评估数组；
		b = analyse_turn(com, x, y);
		Sleep(500);
		system("cls");
		a = sign_ok(per, aha);//将人可下处做标记的环节；
		draw(aha);//画出标记后的棋盘；
	} while (number() != 64 && (a + 1) * (b + 1) != 1);//棋盘下满时循环终止游戏结束；
	printf("\t游戏结束！\n");
	end();
	return 0;
}

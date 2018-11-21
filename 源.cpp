#include <stdio.h>
#include <windows.h>
//����ȫ�ֱ���:
char per, com;
char aha[8][8] = { '\0' };
char hei[8][8] = { '\0' };
char site_discs[8][8];//�洢�����������������������ӵ�λ�ñ��Ϊ'Y'�����Ӵ����Ϊ'*'�����Ӵ����Ϊ'O'��������Ϊ'\0';
double site_score[8][8];//�洢�������λ�÷���;
double k_internal[8][8];//��¼�´˴�ʱk���ڲ��ӵı仯��;��������-��ʼ����
double p_internal[8][8];//��¼�´˴�ʱp���ڲ��ӵı仯��;
double k_eat_num[8][8];//��¼�´˴�ʱk�ӳ�����Ŀ;
double p_eat_num[8][8];//��¼k�ӳ��Ӻ�p�ӿ��Գ������������ֵ;
double p_mobility[8][8];//k�ӳ��Ӻ�p�ӵ��ж�����ѡ����Ŀ;
double score[8][8];//ͨ�������ϲ����ļ�Ȩ�õ����շ�����¼��λ�õĺ��ʶ�;
//��ʼ���ṹ���б����ĺ�����
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
	//��site_score��ֵ;
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
	/*��site_score��ֵ����;����������*2��;
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
//�����̺�����
void draw(char aha[][8])
{
	printf("                       ���ڰ��塿\n");
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
	printf("           �����Щ��Щ��Щ��Щ��Щ��Щ��Щ���\n");
	printf("          a�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��a\n", aha[0][0], aha[0][1], aha[0][2], aha[0][3], aha[0][4], aha[0][5], aha[0][6], aha[0][7]);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          b�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��b\n", aha[1][0], aha[1][1], aha[1][2], aha[1][3], aha[1][4], aha[1][5], aha[1][6], aha[1][7]);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          c�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��c\n", aha[2][0], aha[2][1], aha[2][2], aha[2][3], aha[2][4], aha[2][5], aha[2][6], aha[2][7]);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          d�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��d    * :%-2d\n", aha[3][0], aha[3][1], aha[3][2], aha[3][3], aha[3][4], aha[3][5], aha[3][6], aha[3][7], black);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          e�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��e    O :%-2d\n", aha[4][0], aha[4][1], aha[4][2], aha[4][3], aha[4][4], aha[4][5], aha[4][6], aha[4][7], white);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          f�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��f\n", aha[5][0], aha[5][1], aha[5][2], aha[5][3], aha[5][4], aha[5][5], aha[5][6], aha[5][7]);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          g�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��g\n", aha[6][0], aha[6][1], aha[6][2], aha[6][3], aha[6][4], aha[6][5], aha[6][6], aha[6][7]);
	printf("           �����੤�੤�੤�੤�੤�੤�੤��\n");
	printf("          h�� %c�� %c�� %c�� %c�� %c�� %c�� %c�� %c��h\n", aha[7][0], aha[7][1], aha[7][2], aha[7][3], aha[7][4], aha[7][5], aha[7][6], aha[7][7]);
	printf("           �����ة��ة��ة��ة��ة��ة��ة���\n");
	printf("              0   1   2   3   4   5   6   7  \n");
}
//������������ж�ʤ��������
void end()//�����ж�ʤ��������
{
	//����ڰ���������
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
	//�ж�ʤ����
	if (com_ < per_)
		printf("\t\tyou win!\n\n");
	else if (com_ > per_)
		printf("\t\tyou lose!\n\n");
	else
		printf("\t\tsame score!\n\n");
}
//��search�����ݽ��б�׼������ĺ���;(������ת����ƽ����ԼΪ1)
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
//����������ܹ�������Ŀ�ĺ�����
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
//��������������k�Ƿ���Գ��ӵĺ���;
int detect_line(char k, int x, int i, int y, int j, char A[][8])
{
	int i0, j0;
	int a = 0;//����ã���Ϊ����ֵ��
	for (i0 = i, j0 = j; a == 0 && x + i <= 7 && y + j <= 7 && x + i >= 0 && y + j >= 0; i += i0, j += j0)
	{
		if (A[x + i][y + j] == '\0' || A[x + i][y + j] == '-')break;
		if (A[x + i][y + j] == k)a = 1;//���Գ��ӣ�
	}
	return a;//���Գ���ʱ����1��������ʱ����0��
}
//�������m���崦�Ƿ���Ϲ���ĺ���,�Ϻ����򷵻�1;
int detect_ok(char m, int x, int y, char A[][8])
{
	int i, j;
	int a = 0;//��ǣ���Ϊ����ֵ��
	char n;
	if (m == 'O')n = '*';
	else n = 'O';
	if (A[x][y] == n || A[x][y] == m)//�õ���������ʱ��
		return a;
	for (i = -1; a == 0 && i <= 1; i++)
	{
		if ((x + i) > 7 || (x + i) < 0)continue;
		for (j = -1; a == 0 && j <= 1; j++)
		{
			if ((y + j) > 7 || (y + j) < 0 || A[x + i][y + j] != n)continue;
			a = detect_line(m, x, i, y, j, A);//���ú������м�⣻
		}
	}
	return a;
}
//���˴�������m�Ƿ����ڲ���,��ʱ����1;
int detect_internal(char m, int x, int y, char A[][8])
{
	char n;
	int i, j;
	if (m == 'O') n = '*';
	else n = 'O';
	if (A[x][y] != m) return 0;
	if (x < 2 || x > 5 || y < 2 || y > 5) return 0;
	//�жϸ������Ƿ��Ǳ��Է�������Χ��������;
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
//�������m�´˴�ʱ���ԳԵ�����n����Ŀ�ķ�����
int detect_eat_num(char m, int x, int y, char A[][8], int x0, int y0)
{
	char n;//x0,y0ָ��һ���жԷ����ӵ�����λ��;
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
			if (detect_line(m, x, i, y, j, A))//�ж�m�Ƿ���Գ��ӣ�
			{
				for (int i0 = i, j0 = j; x + i0 <= 7 && y + j0 <= 7 && x + i0 >= 0 && y + j0 >= 0 && A[x + i0][y + j0] == n; i0 += i, j0 += j)
				{
					num++;//ÿ��һ��n����num�ͼ�1��
					if (x + i0 == 0 || x + i0 == 7 || y + j0 == 0 || y + j0 == 7)
					{
						num += 2;//���Ե��������Ǳ�Ե��������ʱnum�ټ�2;
						if (x + i0 == x0 && y + j0 == y0)
							num += 9;//����һ���жԷ��������Ӵ�Ϊ��Ե�Ҹ��������Ӵ����ԳԵ�����ʱ�������Ӻ��ʶ�;
					}
				}
			}
		}
	}
	return num;
}
//����m���Ӻ�����
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
			if (detect_line(m, x, i, y, j, A))//���ú������������߶����Ƿ���m��
			{
				int i0 = i, j0 = j;
				do
				{
					A[x + i0][y + j0] = m;
					i0 += i;
					j0 += j;
				} while (A[x + i0][y + j0] == n);//���ӻ��ڣ�
			}
		}
	}
}
//���m�ӿ��µ㴦�ĺ�����
int sign_ok(char m, char aha[][8])
{
	int i, j;
	int a = 0;//����Ƿ���ڿ����ӵ㣻
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (detect_ok(m, i, j, aha))
			{
				a = 1;
				aha[i][j] = '-';
			}
	return a;
}
//������A�õ�k�ӳ��Ӻ����������B��
void suppose_aftereat(char k, int x, int y, char A[][8], char B[][8])
{
	int i, j;
	for (i = 0; i <= 7; i++)//���帴�ƣ�
		for (j = 0; j <= 7; j++)
			B[i][j] = A[i][j];
	B[x][y] = k;
	eat(k, x, y, B);
}
//����k��������������������ʱ����1��������ʱ����0��
int analyse_turn(char k, int x0, int y0)//(x0,y0)ָ��һ���жԷ����ӵ�����λ�ã�
{
	//����׶�:
	int i, j, m, n, r;
	int x, y;
	char p;
	double k_int_num = 0, p_int_num = 0;//��¼k�ӻ�δ����ʱ�ڲ��ӵ�����;
	int a = 0;//���,���п����ӵ�ʱΪ1,û�п����ӵ�ʱΪ0;
	char huhu[8][8];//��������;
	int num = number();//����numΪ���������ӵ�����;
	if (k == 'O') p = '*';
	else p = 'O';
	//��������Ƿ�������,����ʱ��������:
	if (num == 64)
	{
		printf("��Ϸ������\n");
		end();
		exit(0);//��������
	}
	//��k������֮ǰ�ĺڰ��ڲ��������洢��_int_num��:
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
		{
			k_int_num += detect_internal(k, i, j, aha);
			p_int_num += detect_internal(p, i, j, aha);
		}
	//���������ϵĿհ׵㣬���Կ����ӵ�ĸ������صĲ����Ͻ��м��㣺
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (detect_ok(k, i, j, aha))
			{
				a = 1;
				site_discs[i][j] = 'Y';//��k������Ӵ�����ǣ�
				k_eat_num[i][j] = detect_eat_num(k, i, j, aha, x0, y0);//����k_eat_num[i][j];
				if (x0 == 0 || x0 == 7 || y0 == 0 || y0 == 7)
				{
					if (i == x0 || j == y0)
						score[i][j] += 5;//��k�Ӽ������ӵ�����һ����p���ӵ������̵�ͬһ��ʱ����score[i][j];
				}
				suppose_aftereat(k, i, j, aha, hei);//��aha�м��������Ӳ����Ӻ�����̼�¼��hei��;
				//����k�ӿ����µ�ÿһ��λ�ñ�����һ����p�ӿ������ӵ�λ�õĲ������м���:
				for (m = 0; m <= 7; m++)
					for (n = 0; n <= 7; n++)
					{
						if (detect_ok(p, m, n, hei))
						{
							p_mobility[i][j]++;//ѭ�����Ӽ���p_mobility[i][j];
							if (site_score[m][n] == 20)
								score[i][j] -= 20;//��λ��[m][n]Ϊ���̽�ʱ����score[i][j];
							suppose_aftereat(p, m, n, hei, huhu);//��k�����Ӻ�p�������Ӻ�������ټ�¼��huhu��;
							//���p���´˴���k�������ж�����
							for (r = 0, x = 0; x <= 7; x++)
								for (y = 0; y <= 7; y++)
									r += detect_ok(p, x, y, huhu);
							if (r == 0 )
								score[i][j] -= 20;//��p�����Ӻ�k�����ж���������p���ж����Խ������Ӻ��ʶ�;
							//����k�����Ӻ�p�ӿ��µ㴦���������ķ��������ֵ:
							if (p_eat_num[i][j] < detect_eat_num(p, m, n, hei, i, j))
								p_eat_num[i][j] = detect_eat_num(p, m, n, hei, i, j);
						}
						else
						{
							//��k�����Ӻ�ڰ��ڲ��������洢��_int_num][i][j]��:
							k_internal[i][j] += detect_internal(k, m, n, hei);
							p_internal[i][j] += detect_internal(p, m, n, hei);
						}
					}
				if (p_mobility[i][j] <= 1)
					score[i][j] += 10;//��k�����Ӻ�p���ж�����������û��ʱ����score[i][j];
				k_internal[i][j] -= k_int_num;//����k_internal[i][j];
				p_internal[i][j] -= p_int_num;//����p_internal[i][j];
			}
	//ͨ������õ�ÿ�������ӵ����������ܷ�:
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (site_discs[i][j] == 'Y')
			{
				//�Եõ������ݽ������ݴ���ʹ����������е����ݵ�ƽ��ֵΪ1;
				standard(k_internal);
				standard(p_internal);
				standard(k_eat_num);
				standard(p_eat_num);
				standard(p_mobility);
				if (num < 16)//k�ӳ���������p�ӳ���������λ�ã�p���ж������ڲ��ӡ�
				{
					score[i][j] += 0.2 * site_score[i][j] - 0.1 * p_internal[i][j] + 0.1 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.2 * p_eat_num[i][j] - 0.1 * p_mobility[i][j];
				}
				else if (num < 32)//k�ӳ���������p�ӳ����������ڲ��ӣ�λ�ã�p���ж�����
				{
					score[i][j] += 0.15 * site_score[i][j] - 0.15 * p_internal[i][j] + 0.15 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.2 * p_eat_num[i][j] - 0.15 * p_mobility[i][j];
				}
				else if (num < 48)//k�ӳ���������p�ӳ����������ڲ��ӣ�λ�ã�p���ж�����
				{
					score[i][j] += 0.1 * site_score[i][j] - 0.15 * p_internal[i][j] + 0.15 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.2 * p_eat_num[i][j] - 0.1 * p_mobility[i][j];
				}
				else//k�ӳ���������p�ӳ���������p���ж�����λ�ã��ڲ��ӡ�
				{
					score[i][j] += 0.05 * site_score[i][j] - 0.1 * p_internal[i][j] + 0.1 * k_internal[i][j];
					score[i][j] += 0.3 * k_eat_num[i][j] - 0.3 * p_eat_num[i][j] - 0.1 * p_mobility[i][j];
				}
			}
	if (a == 1)//�п��µ�ʱ
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
		draw(aha);//�������������Ӻ����̣�
		eat(k, x, y, aha);//�õ����Ӻ�����̺�����
		return 1;
	}
	else
	{
		printf("%c �����޿��µ㣬%c �Ӽ����¡�\n", k, p);
		return 0;
	}
}
//��������
int main()
{
	int i, j;
	int y, x;//�ֱ��ʾ�������꣬����ά��һά����㣻
	int a = 0, b = 0;//a������Ƿ�������ӣ�b��������Ӻ���Ƿ��������;
	char c;
	aha[3][3] = aha[4][4] = 'O';
	aha[3][4] = aha[4][3] = '*';
	draw(aha);//������;
	//ѡ��������ɫ���Ⱥ�׶Σ�
	printf("\t����B/����W���밴��ѡ��B/W:");
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
	printf("\t����ѡ�� %c ��.\n", per);
	printf("\t����X/����H���밴��ѡ��X/H:");
	do
	{
		c = getchar();
	} while (c != 'X' && c != 'x' && c != 'H' && c != 'h');
	getchar();
	system("cls");
	draw(aha);
	Sleep(500);
	printf("\t\t��enter����ʼ��Ϸ\n");
	getchar();
	if (c == 'H' || c == 'h')
	{
		if (per == '*') b = analyse_turn(com, 3, 4);
		else b = analyse_turn(com, 3, 3);
	}
	Sleep(500);
	a = sign_ok(per, aha);//������per�Ӷ��Կ������ӵĵط�����ǣ�
	b = 0;
	system("cls");
	draw(aha);//������Ǻ�����̣�
	//�˻���սѭ����
	do
	{
		if (a == 1)
		{
			//���������뻷�ڣ�
			do{
				printf("\t\t%c ���ӣ�\n\t\tit is your turn!", per);
				printf("\n��-��Ϊ������λ��,��������������λ�ã�����+��ĸ����");
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
			//ȡ��֮ǰ�����������ı�ǵ㣻
			for (i = 0; i <= 7; i++)
			{
				for (j = 0; j <= 7; j++)
					if (aha[i][j] == '-')aha[i][j] = '\0';
			}
			aha[x][y] = per;
			//��������ӡȡ����Ǻ�per�����Ӻ�����̣�
			Sleep(250);
			system("cls");
			draw(aha);
			//��������ӡper�ӳ��Ӻ����̣�
			eat(per, x, y, aha);
			Sleep(250);
			system("cls");
			draw(aha);
		}
		else
			printf("\t\t%c ���޴����£�%c �Ӽ�������!\n", per, com);
		//�������ӳ��ӻ��ڲ�������ӡ���ڣ�
		init();//��ʼ���������飻
		b = analyse_turn(com, x, y);
		Sleep(500);
		system("cls");
		a = sign_ok(per, aha);//���˿��´�����ǵĻ��ڣ�
		draw(aha);//������Ǻ�����̣�
	} while (number() != 64 && (a + 1) * (b + 1) != 1);//��������ʱѭ����ֹ��Ϸ������
	printf("\t��Ϸ������\n");
	end();
	return 0;
}

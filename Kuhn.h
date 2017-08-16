#ifndef KUHNH
#define KUHNH
#define MAXN 10000
#define inf 1000000000 
int KM(int m, int n, int** mat, int *match1, int *match2);
/******************************************************
二分图最佳匹配 （kuhn munkras 算法 O(m*m*n)).
邻接矩阵形式 。  返回最佳匹配值，传入二分图大小m,n
邻接矩阵 mat ，表示权，match1,match2返回一个最佳匹配,为匹配顶点的match值为-1，
一定注意m<=n，否则循环无法终止，最小权匹配可将全职取相反数。
初始化：  for(i=0;i<MAXN;i++)
for(j=0;j<MAXN;j++) mat[i][j]=-inf;
对于存在的边：mat[i][j]=val;//注意不能负值
********************************************************/

#include<string.h>
#define _clr(x) memset(x,-1,sizeof(int)*MAXN)
int KM(int m, int n, int** mat, int *match1, int *match2)
{
	int s[MAXN], t[MAXN], l1[MAXN], l2[MAXN];
	int p, q, i, j, k, ret = 0;
	for (i = 0; i<m; i++)
	{
		l1[i] = -inf;
		for (j = 0; j<n; j++)
			l1[i] = mat[i][j]>l1[i] ? mat[i][j] : l1[i];
		if (l1[i] == -inf)  return -1;
	}
	for (i = 0; i<n; i++)
		l2[i] = 0;
	_clr(match1);
	_clr(match2);
	for (i = 0; i<m; i++)
	{
		_clr(t);
		p = 0; q = 0;
		for (s[0] = i; p <= q&&match1[i]<0; p++)
		{
			for (k = s[p], j = 0; j<n&&match1[i]<0; j++)
			{
				if (l1[k] + l2[j] == mat[k][j] && t[j]<0)
				{
					s[++q] = match2[j];
					t[j] = k;
					if (s[q]<0)
					{
						for (p = j; p >= 0; j = p)
						{
							match2[j] = k = t[j];
							p = match1[k];
							match1[k] = j;
						}
					}
				}
			}
		}
		if (match1[i]<0)
		{
			i--;
			p = inf;
			for (k = 0; k <= q; k++)
			{
				for (j = 0; j<n; j++)
				{
					if (t[j]<0 && l1[s[k]] + l2[j] - mat[s[k]][j]<p)
						p = l1[s[k]] + l2[j] - mat[s[k]][j];
				}
			}
			for (j = 0; j<n; j++)
				l2[j] += t[j]<0 ? 0 : p;
			for (k = 0; k <= q; k++)
				l1[s[k]] -= p;
		}
	}
	for (i = 0; i<m; i++)
		ret += mat[i][match1[i]];
	return ret;
}


#endif
/*
The KMP algorithm
*/
void compute_prefix(char *P, int *pi, int m)
{
	pi[1] = 0;
	int k = 0;
	for (int q = 2; q <= m; q++)
	{
		while (k > 0 && P[k + 1] != P[q]) k = pi[k];
		if (P[k + 1] == P[q]) k++;
		pi[q] = k;
	}
}

int KMP_matcher(char *P, int *pi, char*T, int m, int n)
{
	int sum = 0;
	int q = 0;
	for (int i = 1; i <= n; i++)
	{
		while (q > 0 && P[q + 1] != T[i]) q = pi[q];
		if (P[q + 1] == T[i]) q++;
		if (q == m)
		{
			sum++;
			q = pi[q];
		}
	}
	return sum;
}
/*
The KMP algorithm
*/
int *compute_prefix(char *P, int m)
{
	int *pi = new int[m + 1];
	pi[1] = 0;
	int k = 0;
	for (int q = 2; q <= m; q++)
	{
		while (k > 0 && P[k + 1] != P[q]) k = pi[k];
		if (P[k + 1] == P[q]) k++;
		pi[q] = k;
	}
	return pi;
}
int KMP_matcher(char *P, char*T, int m, int n)
{
	int *pi = compute_prefix(P, m);
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
	delete[]pi;
	return sum;
}
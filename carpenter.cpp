#include <cstdio>
#include <algorithm>

const int maxn = 600;

int boardLength, boardWidth;
bool isGoodBoard[maxn + 1][maxn + 1];
int result[maxn + 1][maxn + 1];

int main()
{
	scanf("%d%d", &boardLength, &boardWidth);
	
	while (true) {
		int a, b;
		scanf("%d", &a);
		
		if (a == -1)
			break;
		
		scanf("%d", &b);
		isGoodBoard[a][b] = true;
	}
	
	if (isGoodBoard[1][1])
		result[1][1] = 0;
	else
		result[1][1] = 1;
		
	for (int i = 1; i <= boardLength; i++)
		for (int j = 1; j <= boardWidth; j++) {
			if (i == 1 && j == 1)
				continue;
			
			if (isGoodBoard[i][j])
				result[i][j] = 0;
			else {
				result[i][j] = maxn*maxn + 1;
				
				for (int k = 1; k <= i/2; k++)
					result[i][j] = std::min(result[i][j], result[k][j] + result[i - k][j]);
				
				for (int k = 1; k <= j/2; k++)
					result[i][j] = std::min(result[i][j], result[i][k] + result[i][j - k]);
			}
		}
	
	printf("%d\n", result[boardLength][boardWidth]);
	return 0;
}

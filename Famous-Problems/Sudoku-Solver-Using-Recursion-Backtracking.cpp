const int N = 9;
vector<string> board(N);

bool canPlace(int r, int c, int val)
{
	for(int i = 0 ; i < N ; i++)
	{
		if(board[r][i] == val)
			return false;
		if(board[i][c] == val)
			return false;
	}

	int rootn = sqrt(N);
	int row = rootn * floor(r / rootn);
	int col = rootn * floor(c / rootn);
	
	for(int i = row ; i < row + rootn ; i++)
		for(int j = col ; j < col + rootn ; j++)
			if(board[i][j] == val)
				return false;

	return true;
}

bool solveSudoku(int r = 0, int c = 0)
{
	if(r == N)
		return true;

	if(c == N)
		return solveSudoku(r + 1, 0);

	if(board[r][c] != '.')
		return solveSudoku(r, c + 1);
	
	for(char val = '1' ; val <= N + '0' ; val++)
	{
		if(canPlace(r, c, val))
		{
			board[r][c] = val;
			if(solveSudoku(r, c + 1))
				return true;
			board[r][c] = '.';
		}
	}
	return false;
}
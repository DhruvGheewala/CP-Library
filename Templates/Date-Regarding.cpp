int dayofweek(int dd, int mm, int yy)
{
	static string days[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
	static int day[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	yy -= (mm < 3);  // for leap year
	return (yy + yy >> 2 - yy / 100 + yy / 400 + day[mm - 1] + dd) % 7;
}

bool isLeap(int yy) {return (yy % 400 == 0 ? true : ((yy % 4 == 0 && yy % 100 != 0) ? true : false));}
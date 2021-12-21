
#ifndef __CUSTOM_BITS_STDCPP_INCLUDED__
	
#define __CUSTOM_BITS_STDCPP_INCLUDED__

#define DEBUG
#define DHRUV_GHEEWALA
// #define __INCLUDE_LINE_NUMBER__
// #define __USE_TOURIST_DEBUG__
// #define __HOLD_SCREEN_ATLAST__

#include <bits/stdc++.h>
#include "debug.h"

void __holdScreen()
{
	#ifdef __HOLD_SCREEN_ATLAST__
	#undef DHRUV_GHEEWALA
	#undef endl
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
	cout << "Press any key to continue...";
	cout.flush();
	
	string s;
	getline(cin, s);

	#endif
}

#ifndef DHRUV_GHEEWALA
auto __initialize__ = []() {
	freopen("debug.txt", "w", stderr);
	return 0;
}();
#endif

#define exit(x)  	__holdScreen();\
					system("Color 0A");\
                    cline << "Run time: " << fixed << setprecision(3) << (double)clock() / CLOCKS_PER_SEC << "s" << endl;\
                    return x;\

#endif

#ifdef TEST


#include <ostream>
#include <iostream>
#include "fixpoint.h"


int main(){
	sf16 fp = -10.9f;
	std::cout << (float)fp << std::endl;
	sf16 fp1 = fp * fp;
	std::cout << (float)fp1 << std::endl;
	fp1 = fp1 / fp;
	std::cout << (float)fp1 << std::endl;

	sf16 fp2 = 3.3f;
	fp1 /= fp2;
	std::cout << (float)fp1 << std::endl;

	sf16 fp3 = 4.f;
	fp1 = fp1 + fp3;

	std::cout << (float)fp1 << std::endl;
	std::cout << (int)fp1 << std::endl;

//	sf16 fp1 = -10.f;
//	sf16 fp2 = 5.f;
//
//	std::cout << (float)(fp1 + fp2) << std::endl;
//	std::cout << (float)(fp2 + fp1) << std::endl;
//
//	sf16 fp3 = 10.f;
//	sf16 fp4 = -5.f;
//
//	std::cout << (float)(fp3 + fp4) << std::endl;
//	std::cout << (float)(fp4 + fp3) << std::endl;

	return 0;
}
#endif

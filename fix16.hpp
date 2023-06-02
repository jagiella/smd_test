/*
 * fix16.h
 *
 *  Created on: 02.06.2023
 *      Author: nickj
 */

#ifndef FIX16_HPP_
#define FIX16_HPP_

//#include <ostream>
//#include <iostream>

#define FIX16_INT_BITS              10
#define FIX16_FRAC_BITS             (16 - FIX16_INT_BITS)

/*
typedef signed short s16;
typedef signed int s32;
s16 divs(s32 op1, s16 op2) {
	std::cout << "divs( " << op1 << ", " << op2 << ")" << std::endl;
	return op1 / op2;
	//op1 /= op2;
	//return op1;
}

s32 muls(s16 op1, s16 op2) {
	std::cout << "muls( " << op1 << ", " << op2 << ")" << std::endl;
	return op1 * op2;
}
*/

extern s32 muls(s16 op1, s16 op2);
extern s16 divs(s32 op1, s16 op2);

struct FixPoint {
	s16 v;

	FixPoint() {
		v = 0;
	}
	FixPoint(const signed short i) {
		v = i << FIX16_FRAC_BITS;
	}
	FixPoint(const float f) {
		v = f * (1 << FIX16_FRAC_BITS);
	}
	FixPoint(const FixPoint &fp) {
		v = fp.v;
	}

	FixPoint& operator=(const FixPoint &fp1) {
		this->v = fp1.v;
		return *this;
	}
	FixPoint& operator=(const s16 &val) {
		this->v = val << FIX16_FRAC_BITS;
		return *this;
	}
	FixPoint& operator+=(const FixPoint &fp1) {
		//std::cout << "add( " << this->v << " + " << fp1.v << ")" << std::endl;
		this->v += fp1.v;
		return *this;
	}
	FixPoint& operator-=(const FixPoint &fp1) {
		//std::cout << "sub( " << this->v << " - " << fp1.v << ")" << std::endl;
		this->v -= fp1.v;
		return *this;
	}
	FixPoint& operator*=(const FixPoint &fp1) {
		s32 val1 = this->v;
		s16 val2 = fp1.v;
		this->v = muls(val1, val2) >> FIX16_FRAC_BITS;
		return *this;
	}
	FixPoint& operator/=(const FixPoint &fp1) {
		s32 val1 = this->v;
		s16 val2 = fp1.v;
		this->v = divs(val1 << FIX16_FRAC_BITS, val2);
		return *this;
	}

	FixPoint operator+(const FixPoint &fp1) const {
		FixPoint fp2 = *this;
		fp2 += fp1;
		return fp2;
	}
	FixPoint operator-(const FixPoint &fp1) const {
		FixPoint fp2 = *this;
		fp2 -= fp1;
		return fp2;
	}
	FixPoint operator*(const FixPoint &fp1) const {
		FixPoint fp2 = *this;
		fp2 *= fp1;
		return fp2;
	}
	FixPoint operator/(const FixPoint &fp1) const {
		FixPoint fp2 = *this;
		fp2 /= fp1;
		return fp2;
	}

	operator float() const {
		return ((float) v) / ((float) (1 << FIX16_FRAC_BITS));
	}
	/*operator int() const {
		return v >> FIX16_FRAC_BITS;
	}*/
	operator signed short() const {
			return v >> FIX16_FRAC_BITS;
		}
};

//typedef FixPoint fix16;
/*
int main() {
	fix16 fp = -10.9f;
	std::cout << fp << std::endl;
	fix16 fp1 = fp * fp;
	std::cout << fp1 << std::endl;
	fp1 = fp1 / fp;
	std::cout << fp1 << std::endl;

	fix16 fp2 = 3.3f;
	fp1 /= fp2;
	std::cout << fp1 << std::endl;

	fix16 fp3 = 4.f;
	fp1 = fp1 + fp3;
	std::cout << fp1 << std::endl;

	fix16 fp4 = 40.f;
	fp1 = fp1 - fp4;
	std::cout << fp1 << std::endl;

	std::cout << (float) fp1 << std::endl;
	std::cout << (int) fp1 << std::endl;

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
}*/

#endif /* FIX16_HPP_ */

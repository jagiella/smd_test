/*
 * pixpoint.h
 *
 *  Created on: 24.06.2022
 *      Author: nickj
 */

#ifndef FIXPOINT_H_
#define FIXPOINT_H_

#define FIXED_POINT_FRACTIONAL_BITS 6
#define FIXED_POINT_SIGN_MASK 0x8000
#define FIXED_POINT_NUM_MASK ~FIXED_POINT_SIGN_MASK

struct FixPoint {
	unsigned short v;

	// constructors
	FixPoint(const int i = 0) {
		v = i << FIXED_POINT_FRACTIONAL_BITS;
	}
	FixPoint(const float f) {
		v = f * (1 << FIXED_POINT_FRACTIONAL_BITS);
	}

	// asignments
	FixPoint& operator=(const float f) {
		v = f * (1 << FIXED_POINT_FRACTIONAL_BITS);
		return *this;
	}
	FixPoint& operator=(const int i) {
		v = i << FIXED_POINT_FRACTIONAL_BITS;
		return *this;
	}
	FixPoint& operator/=(const FixPoint fp) {
		v = (((unsigned int) v) << FIXED_POINT_FRACTIONAL_BITS) / fp.v;
		return *this;
	}
	FixPoint& operator/=(const float f) {
		*this /= (FixPoint) f;
		return *this;
	}
	FixPoint& operator/=(const int i) {
		v /= i;
		return *this;
	}

	// arithmetics
	FixPoint operator+(const FixPoint &fp1) {
		FixPoint fp;
		fp.v = v + fp1.v;
		return fp;
	}
	FixPoint operator-(const FixPoint &fp1) {
		FixPoint fp;
		fp.v = v - fp1.v;
		return fp;
	}
	FixPoint operator*(const FixPoint &fp1) {
		FixPoint fp;
		fp.v = (((unsigned int) v) * fp1.v) >> FIXED_POINT_FRACTIONAL_BITS;
		return fp;
	}
	FixPoint operator/(const FixPoint &fp1) {
		FixPoint fp;
		fp.v = (((unsigned int) v) << FIXED_POINT_FRACTIONAL_BITS) / fp1.v;
		return fp;
	}

	// conversions
	operator float() const {
		return ((float) v) / ((float) (1 << FIXED_POINT_FRACTIONAL_BITS));
	}
	operator int() const {
		return v >> FIXED_POINT_FRACTIONAL_BITS;
	}
};

struct SignedFixPoint {
	unsigned short v;

	// constructors
	SignedFixPoint(const int i = 0) {
		if (i < 0) {
			v = (((unsigned short) -i) << FIXED_POINT_FRACTIONAL_BITS)
					| FIXED_POINT_SIGN_MASK;
		} else {
			v = (((unsigned short) i) << FIXED_POINT_FRACTIONAL_BITS);
		}
	}
	SignedFixPoint(const float f) {
		if (f >= 0)
			v = f * (1 << FIXED_POINT_FRACTIONAL_BITS);
		else
			v = FIXED_POINT_SIGN_MASK
					| ((~FIXED_POINT_SIGN_MASK)
							& ((unsigned short) (-f
									* (1 << FIXED_POINT_FRACTIONAL_BITS))));
	}

	// asignments
//	SignedFixPoint& operator=(const float f) {
//		v = f * (1 << FIXED_POINT_FRACTIONAL_BITS);
//		return *this;
//	}
//	SignedFixPoint& operator=(const int i) {
//		v = i << FIXED_POINT_FRACTIONAL_BITS;
//		return *this;
//	}
//	SignedFixPoint& operator/=(const SignedFixPoint fp) {
//		v = (((unsigned int) v) << FIXED_POINT_FRACTIONAL_BITS) / fp.v;
//		return *this;
//	}
//	SignedFixPoint& operator/=(const float f) {
//		*this /= (SignedFixPoint) f;
//		return *this;
//	}
	SignedFixPoint& operator/=(const int i) {
		// split i

		// divide
//		if(i>=0)
//			v = ((v & FIXED_POINT_NUM_MASK) / i) | (v & FIXED_POINT_NUM_MASK);
//		else{
//			unsigned short pi = -i;
//			v = ((v & FIXED_POINT_NUM_MASK) / pi) | (v & FIXED_POINT_NUM_MASK);
//		}
		SignedFixPoint fp = i;
		*this = *this / fp;

		return *this;
	}

	// arithmetics
	SignedFixPoint operator+(const SignedFixPoint &fp1) {
		SignedFixPoint fp;
		if (v >> 15 == fp1.v >> 15)
			fp.v = v + (fp1.v & FIXED_POINT_NUM_MASK);
		else{
			if((v & FIXED_POINT_NUM_MASK) > (fp1.v & FIXED_POINT_NUM_MASK)){
				fp.v = v - (fp1.v & FIXED_POINT_NUM_MASK);
			}
			else
				fp.v =  fp1.v - (v & FIXED_POINT_NUM_MASK);
//				fp.v =  (fp1.v & FIXED_POINT_NUM_MASK) - (v & FIXED_POINT_NUM_MASK) + (fp1.v & FIXED_POINT_SIGN_MASK);

		}
		return fp;
	}
//	SignedFixPoint operator-(const SignedFixPoint &fp1) {
//		SignedFixPoint fp;
//		fp.v = v - fp1.v;
//		return fp;
//	}
	SignedFixPoint operator*(const SignedFixPoint &fp1) {
		SignedFixPoint fp;
		fp.v =
				(((unsigned int) (v & FIXED_POINT_NUM_MASK))
						* (fp1.v & FIXED_POINT_NUM_MASK))
						>> FIXED_POINT_FRACTIONAL_BITS;
		// add sign
		fp.v |= ((v ^ fp1.v) & FIXED_POINT_SIGN_MASK);
		return fp;
	}
	SignedFixPoint operator/(const SignedFixPoint &fp1) {
		SignedFixPoint fp;
		// divide without sign
		unsigned int tmp = (v & FIXED_POINT_NUM_MASK)
				<< FIXED_POINT_FRACTIONAL_BITS;
		fp.v = tmp / (fp1.v & FIXED_POINT_NUM_MASK);
		// add sign
		fp.v |= ((v ^ fp1.v) & FIXED_POINT_SIGN_MASK);
		return fp;
	}

	// conversions
	operator float() const {
		if (FIXED_POINT_SIGN_MASK & v)
			return -((float) (FIXED_POINT_NUM_MASK & v))
					/ ((float) (1 << FIXED_POINT_FRACTIONAL_BITS));
		else
			return ((float) (FIXED_POINT_NUM_MASK & v))
					/ ((float) (1 << FIXED_POINT_FRACTIONAL_BITS));
	}
	operator int() const {
		if (v & FIXED_POINT_SIGN_MASK)
			return -(int) ((v & FIXED_POINT_NUM_MASK)
					>> FIXED_POINT_FRACTIONAL_BITS);
		else
			return (int) ((v & FIXED_POINT_NUM_MASK)
					>> FIXED_POINT_FRACTIONAL_BITS);
	}
};

typedef FixPoint ufix16;
typedef FixPoint uf16;
typedef SignedFixPoint sfix16;
typedef SignedFixPoint sf16;

//float operator=(FixPoint &fp){
//	float f = ((float)fp.v) / ((float)(1<<FIXPOINT_OFFSET));
//}

#endif /* FIXPOINT_H_ */

#ifndef FIELD
	#include "field.cpp" 
#endif

//TODO: Implement ECDSA after diffie hellman

#include<gmpxx.h>
#include<iostream>
#include<assert.h>

#ifndef EC_CPP
	#include "ec.cpp"
#endif

typedef EcElement Point;

void show(const EcElement &e){
	std::cout<<"("<<e.getEcElementX()<<", "<<e.getEcElementY()<<")";
}

int main(){
	Field p11(11);
	FieldElement f1(6, p11), f2(7, 11);
	FieldElement f3 = f1 + f2;
	std::cout<<f1<<" + "<<f2
	 			<<" = "<<f3<<"\n";

	EllipticCurve curve(1,6, p11);
	EcElement p0 = curve.getEcPoint(2, 4);
	EcElement p1(mpz_class(3), mpz_class(5), curve);
	EcElement p2(p0 + p1);
	show(p0); std::cout<<" + ";
	show(p1); std::cout<<" = ";
	show(p2);
	std::cout<<"  [y^2 = x^3+x+6 (mod 11)]\n";
	assert(p2 == curve.getEcPoint(7, 2));
	

	//Following test examples taken from https://youtu.be/vnpZXJL6QCQ?t=4713 
	EllipticCurve ec(2, 2, 17);  //order of this group = 19
	curve = ec;
	Point generator = curve.getEcPoint(5, 1);  //generator of this group 
	Point point_at_infinity = curve.getPointAtInfinity();

	assert( generator               ==	(19*1323 + 1)				*	generator				);
	assert( curve.getEcPoint(6, 3)	==	15*generator				+	6*curve.getEcPoint(5, 1));
	assert( curve.getEcPoint(6, 3)	==	curve.getEcPoint(5, 1)		+	curve.getEcPoint(5, 1)	);
	assert( curve.getEcPoint(10, 6)	==	curve.getEcPoint(6, 3)		+	curve.getEcPoint(5, 1)	);
	assert( curve.getEcPoint(3, 1)	==	curve.getEcPoint(10, 6)		+	curve.getEcPoint(5, 1)	);
	assert( curve.getEcPoint(3, 1)	==	curve.getEcPoint(6, 3) 		+	curve.getEcPoint(6, 3)	);
	assert( curve.getEcPoint(9, 16)	==	curve.getEcPoint(6, 3)		+	curve.getEcPoint(10, 6)	);
	assert( curve.getEcPoint(7, 11)	==	curve.getEcPoint(10, 6)		+	curve.getEcPoint(0, 6)	);
	assert( point_at_infinity		==	curve.getEcPoint(5, 1)		+	curve.getEcPoint(5, 16)	);
	assert( point_at_infinity		==	curve.getEcPoint(0, 6)		+	curve.getEcPoint(0, 11)	);
	assert( curve.getEcPoint(5, 1)	== 	curve.getEcPoint(5, 1)		+	point_at_infinity		);
	// /****************************************************************************************/

	EllipticCurve P256curve = getP256Curve();
	std::cout<<(P256curve.getCurveA())<<"\n"<<P256curve.getCurveB()<<"\n";

	std::cout<<"Successfully passed all tests!\n";
	return 0;
}
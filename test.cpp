#ifndef FIELD
	#include "field.cpp" 
#endif

#ifndef ASSERT
	#define ASSERT
	#include<assert.h>
#endif

#ifndef EC_CPP
	#include "ec.cpp"
#endif

#include<iostream>

void show(const EcElement &e){
	std::cout<<"("<<e.getEcElementX()<<", "<<e.getEcElementY()<<")";
}

int main(){
	Field p11(11);
	
	FieldElement f1(6, p11), f2(7, 11);
	FieldElement f3 = f1 + f2;
	std::cout<<f1.serialise()<<" + "<<f2.serialise()
				<<" = "<<f3.serialise()<<"\n";


	EC ec(1, 6, p11);
	EllipticCurve curve(ec);
	
	EcElement p0 = curve.getEcPoint(2, 4);
	EcElement p1(3, 5, ec);
	EcElement p2 = p0 + p1;
	//assert(p2.isEqual(curve.getEcPoint(7, 2)));
	show(p0); std::cout<<" + ";
	show(p1); std::cout<<" = ";
	show(p2);
	std::cout<<"  [y^2 = x^3+x+6 (mod 11)]\n";
	
	//Following test examples taken from https://youtu.be/vnpZXJL6QCQ?t=4713
	EC ec2(2, 2, 17);
	curve = ec2;
	assert( curve.getEcPoint(6, 3).isEqual(curve.getEcPoint(5, 1)+curve.getEcPoint(5, 1)) );
	assert( curve.getEcPoint(10, 6).isEqual(curve.getEcPoint(6, 3)+curve.getEcPoint(5, 1)) );
	assert( curve.getEcPoint(3, 1).isEqual(curve.getEcPoint(10, 6)+curve.getEcPoint(5, 1)) );
	assert( curve.getEcPoint(3, 1).isEqual(curve.getEcPoint(6, 3)+curve.getEcPoint(6, 3)) );
	assert( curve.getEcPoint(9, 16).isEqual(curve.getEcPoint(6, 3)+curve.getEcPoint(10, 6)) );
	assert( curve.getEcPoint(7, 11).isEqual(curve.getEcPoint(10, 6)+curve.getEcPoint(0, 6)) );
	assert( curve.getEcPoint(-1, -1).isEqual(curve.getEcPoint(5, 1)+curve.getEcPoint(5, 16)) );
	assert( curve.getEcPoint(-1, -1).isEqual(curve.getEcPoint(0, 6)+curve.getEcPoint(0, 11)) );
	assert( curve.getEcPoint(5, 1).isEqual(curve.getEcPoint(5, 1)+curve.getEcPoint(-1, -1)) );
	/****************************************************************************************/

	return 0;
}

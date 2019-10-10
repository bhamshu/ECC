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
	EcElement p2 = p0*p1;
	show(p0); std::cout<<" * ";
	show(p1); std::cout<<" = ";
	show(p2);
	std::cout<<"  [y^2 = x^3+x+6 (mod 11)]\n";
	return 0;
}

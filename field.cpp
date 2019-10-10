// TODO: Use GMP and use 160 bit primes for guaranteeing 128-bit security. Btw !!CAUTION!! in this current version everything is designed to work in 32 bits, so it's not really secure for any real scenario.
// https://tools.ietf.org/html/rfc6090

#define FIELD_CPP

#ifndef ASSERT
	#define ASSERT
	#include<assert.h>
#endif

#ifndef UTILITIES_CPP
	#include "utilities.cpp"
#endif

//#define TESTING

#include<string>


#ifdef TESTING
#include<iostream>
#endif

class Field{
	protected:
		int p; //charecteristic prime
	public:
		Field(int p){
			this->p = p;
			//assert(isPrime(p));
		}
		int getCharacteristic() const{
			return p;
		}
};


class FieldElement : public Field{
	private:
		int x; //element
	public:	
		FieldElement(int x, int p) : Field(p){
			this->x = x; 
			assert(x<p);
			//assert(isPrime(p));
		}
		FieldElement(int x, Field f) : Field(f){
			this->x = x; 
			assert(x<p);
			//assert(isPrime(p));
		}

		int getValue() const{
			return x;
		}
		FieldElement operator+(FieldElement const &f2){
			assert((this->p) == f2.getCharacteristic());
			FieldElement ret((this->x + f2.getValue())%(this->p), this->p);
			return ret;
 		}
 		FieldElement operator*(FieldElement const &f2){
			assert((this->p) == f2.getCharacteristic());
			FieldElement ret((this->x * f2.getValue())%(this->p), this->p);
			return ret;
 		}
 		FieldElement operator-(FieldElement const &f2){
			assert((this->p) == f2.getCharacteristic());
			FieldElement ret( (this->x - f2.getValue() + this->p)%(this->p), 
								this->p);
			return ret;
 		}
 		FieldElement operator/(FieldElement const &f2){
			assert(1==0); //TODO
		}
 		std::string serialise(){
 			return "("+std::to_string(x) + " % " + std::to_string(p)+ ")";
 		}
};


#ifdef TESTING
int main(){
	FieldElement f1(3, 7),
					f2(4, 7);
	FieldElement f3 = f1 - f2;
	std:: cout<<f3.serialise();
	return 0;
}
#endif


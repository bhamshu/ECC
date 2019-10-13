// TODO: Use GMP and use 160 bit primes for guaranteeing 128-bit security. Btw !!CAUTION!! in this current version everything is designed to work in 32 bits, so it's not really secure for any real scenario.
// https://tools.ietf.org/html/rfc6090

//#define DEBUG

#define FIELD_CPP

#include<gmpxx.h>
#include<assert.h>
#include<ostream>


//#ifdef DEBUG
#include<iostream>
//#endif

class FieldElement;
FieldElement inv(FieldElement x);	

class Field{
	protected:
		mpz_class p; //charecteristic prime
	public:
		Field(){
		}
		template<class T>
		Field(T p){
			this->p = p;
			//assert(isPrime(p));
		}
		mpz_class getCharacteristic() const{
			return p;
		}
		bool operator==(Field f2){
			return f2.getCharacteristic()==p;
		}
		FieldElement getElement(int x);	
};

class FieldElement : public Field{
	using Field::p;
	protected:
		mpz_class x; //element
	public:	
		FieldElement(){
		}
		template<class T>
		FieldElement(T x, T p) : Field(p){
			if(p==-1){
				assert(x==-1);
			}
			else{
				assert(p>0);
				//assert(isPrime(p));
				this->x = (x%p + p)%p; 
			}
		}
		template<class T>
		FieldElement(T x, Field f) : Field(f){
			if(p==-1){
				assert(x==-1);
			}
			else{
				assert(p>0);
				//assert(isPrime(p));
				this->x = (x%p + p)%p; 
			}
		}

		mpz_class getValue() const{
			return x;
		}

		//removed const & from these operators because it prevents multiple operations.
		FieldElement operator+(FieldElement f2){
			assert( p == f2.getCharacteristic() );
			FieldElement ret( (x + f2.getValue())%p, p);
			return ret;
 		}
 		FieldElement operator*(FieldElement f2){
			assert( p == f2.getCharacteristic() );
			FieldElement ret( (x * f2.getValue())%p, p);
			return ret;
 		}
 		FieldElement operator-(FieldElement f2){
			assert( p == f2.getCharacteristic() );
			FieldElement ret( ((x- f2.getValue())%p + p)%p, p);
			return ret;
 		}
 		FieldElement operator/(FieldElement f2){
			assert( p == f2.getCharacteristic() );
			FieldElement ret = (*this) * inv(f2);
			return ret;
		}
 		// std::string serialise(){
 		// 	return "("+std::to_string(x) + " % " + std::to_string(p)+ ")";
 		// }
};

std::ostream & operator<<(std::ostream &out, FieldElement fe){ 
    out << "(" << fe.getValue() << " % " << fe.getCharacteristic() << ")"; 
    return out; 
} 

template<class T>//T can be int or mpz_class
FieldElement operator*(T x, FieldElement fe){
	mpz_class p = fe.getCharacteristic();
	FieldElement ret( (x%p * fe.getValue())%p, p);
	return ret;
}

bool operator==(FieldElement fe1, FieldElement fe2){
//	std::cout<<fe1<<" "<<fe2;
	assert((fe2.getCharacteristic() == fe1.getCharacteristic()));
	return fe1.getValue()==fe2.getValue();
}

bool operator!=(FieldElement fe1, FieldElement fe2){
	assert(fe2.getCharacteristic() == fe1.getCharacteristic());
	return fe1.getValue()!=fe2.getValue();
}

template<class T>
bool operator==(FieldElement fe1, T x){
	return x/*%fe1.getCharacteristic()*/==fe1.getValue();
}

template<class F>
bool operator!=(FieldElement fe1, F x){
	return !(fe1==x);
}

template<class T, class F>  // T can be int or mpz_class. F can be field or int or mpz class
FieldElement getFieldElement(T x, F p){
	FieldElement fe(x, p);
	return fe;
}

FieldElement Field::getElement(int x){	
	FieldElement fe(x, p);
	return fe;
}	

FieldElement operator-(FieldElement fe){
	return getFieldElement(0, fe.getCharacteristic()) - fe;	
}
 
template<class T>
mpz_class operator%(FieldElement fe, T x){
	return fe.getValue()%x;
}

FieldElement powmod(FieldElement b, mpz_class e){
	mpz_class p = b.getCharacteristic();
	if(e==0)
		return getFieldElement(1, p);

	FieldElement t = powmod(b, e/2);
	return t * t * ((e%2==1)?b:getFieldElement(1, p));
	//return (((ll)t * (ll)t)%m * (ll)((e%2)?b:1))%m;
}

FieldElement inv(FieldElement x){	//returns 1/x
	//assert(isPrime(p));
	mpz_class p = x.getCharacteristic();
	FieldElement ret = powmod(x, p-2);
	//assert(x*x==1);
	return ret;
}

#ifdef DEBUG
int main(){
	mpz_class sev("7");
	Field p7(sev), p7_(7);
	assert(p7==p7_);
	FieldElement f1(3, 7),
					f2(6, p7), f6 = p7.getElement(6);
	FieldElement f3 = f1 - f2;
	FieldElement f4 = powmod(f3, 2);
	std::cout<<p7.getElement(3)/p7.getElement(5)<<"\n";
	return 0;
}
#endif

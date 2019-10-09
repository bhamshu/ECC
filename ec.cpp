// parameter set: https://tools.ietf.org/html/rfc6090#appendix-D

#define EC_CPP

typedef long long ll;

#ifndef FIELD_CPP
	#include "field.cpp" //ec
#endif

#ifndef UTILITIES_CPP
	#include "utilities.cpp"
#endif

#ifndef ASSERT
	#define ASSERT
	#include<assert.h>
#endif

class EcElement;

class EC: public Field { //elliptic curve
	protected:
		int a, b; // y^2 = x^3 + a*x + b;
		//(-1, -1) denotes point at infinity
	public:
		EC(int a, int b, int p) : Field(p){
			this->a = a; 
			this->b = b; 
			assert(a<p); 
			assert(b<p);
			assert(this->getDiscriminant()!=0);
		}
		EC(int a, int b, Field f) : Field(f){
			this->a = a; 
			this->b = b; 
			assert(a<p); 
			assert(b<p);
			assert(this->getDiscriminant()!=0);	
		}
		bool isBelong(int x, int y){
			if(x==-1&&y==-1) return true; //point at infinity always belongs
			return ((ll)y*(ll)y)%p == ((((ll)x*(ll)x)%p*(ll)x)%p + ((ll)a*(ll)x)%p + (ll)b)%p;
		}
		int getDiscriminant(){
			return (4*a*a*a + 27*b*b)%p;
		}
		int getCurveA() const{
			return a;
		}
		int getCurveB() const{
			return b;
		}
};

class EcElement : public EC {
	private:
		int x, y;
	public:	
		EcElement(int x, int y, int a, int b, int p) : EC(a, b, p){
			this->x = x;
			this->y = y;
			assert(isBelong(x, y));
		}
		EcElement(int x, int y, EC &curve) : EC(curve){
			this->x = x;
			this->y = y;
			assert(isBelong(x, y));
		}
		int getEcElementX() const{
			return x;
		}
		int getEcElementY() const{
			return y;
		}
		bool isEqual(EcElement const &ece2) const{
			assert( p==ece2.getCharacteristic() 
					&& a==ece2.getCurveA()
					&& b==ece2.getCurveB());

			return ece2.getEcElementX() == x 
					&& ece2.getEcElementY() == y;
		}
		bool isPointAtInfinity() const{
			return (x==-1&&y==-1);
		}
		EcElement getInverse(){		
			if(this->isPointAtInfinity()) return *this;	
			EcElement ret(x, (p-y)%p, a, b, p);
			return ret;
		}
		
		//https://tools.ietf.org/html/rfc6090#appendix-F.1
		EcElement operator*(EcElement const &ece2){
			assert( p==ece2.getCharacteristic() 
					&& a==ece2.getCurveA()
					&& b==ece2.getCurveB());

			int x3, y3;
			int x1 = x, y1 = y;
			int x2 = ece2.getEcElementX(),
				 y2 = ece2.getEcElementY();

			if(this->isPointAtInfinity()){
				x3 = x2;
				y3 = y2;
			}
			else if(ece2.isPointAtInfinity()){
				x3 = x1;
				y3 = y1;
			}
			else if(!this->isEqual(ece2) && x1==x2){
				x3 = -1;
				y3 = -1;
			}
			else if(!this->isEqual(ece2) && x1!=x2){
				int temp = ( ( (ll)(y2-y1) * (ll)inv(x2-x1, p))%p + (ll)p)%p;
				x3 = ( ((ll)temp * (ll) temp - (ll)x1 - (ll)x2)%p + (ll)p)%p;
				y3 = (((((ll)(x1-x3)*(ll)(y2-y1))%p*(ll)inv(x2-x1, p))%p - y1)%p + p)%p;
			}
			else if(this->isEqual(ece2)&&this->isPointAtInfinity()){
				x3 = -1;
				y3 = -1;
			}
			else{ // equal. and not point at infinity
				int temp = (((3LL*((ll)x1*(ll)x1)%p)%p + a)%p * (ll)inv(2*y1, p))%p;
				x3 = ((((ll)temp * (ll)temp)%p - 2LL*(ll)x1)%p+(ll)p)%p;
				y3 = ((((ll)(x1-x3)*(((3LL*(ll)x1)%p*(ll)x1)%p + (ll)a) * (ll)inv(2*y1, p))%p - y)%p + p) %p;
			}
			EcElement ret(x3, y3, a, b, p);
			return ret;
		}
};

class EllipticCurve : public EC {
	public:
		EllipticCurve(EC curve): EC(curve){

		}
		EcElement getEcPoint(int x, int y){
			EcElement ret(x, y, a, b, p);
			return ret;
		}
};

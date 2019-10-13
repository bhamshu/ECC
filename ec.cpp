// parameter set: https://tools.ietf.org/html/rfc6090#appendix-D

#define EC_CPP

#include<gmpxx.h>
#include<assert.h>

typedef long long ll;
typedef mpz_class mzc;

#ifndef FIELD_CPP
	#include "field.cpp" //ec
#endif

#ifndef UTILITIES_CPP
	#include "utilities.cpp"
#endif

class EcElement;

class EllipticCurve: public Field { //elliptic curve
	typedef FieldElement INT_P; //P to denote that it's an integer modulo some prime
	protected:
		using Field::p;
		INT_P a, b; // y^2 = x^3 + a*x + b;
	public:
		EllipticCurve(){}
		EllipticCurve(const INT_P &a_arg, const INT_P &b_arg, const Field& f_arg) : Field(f_arg), a(a_arg), b(b_arg){ 
			assert(a.getValue() < p); 
			assert(b.getValue() < p);
			assert(this->getDiscriminant().getValue()!=mzc(0));	
		}

		template<class T, class F> // T can be int/long/mpz_class.  F can be all these or a field 
		EllipticCurve(const T &a_arg, const T &b_arg, const F& f_arg) 
									: Field(f_arg), 
										a( getFieldElement(a_arg, f_arg) ), 
										b( getFieldElement(b_arg, f_arg) ){ 
			assert(a.getValue() < p); 
			assert(b.getValue() < p);
			assert(this->getDiscriminant().getValue()!=mzc(0));	
		}

		bool isBelong(INT_P x, INT_P y){
			if(x.getValue()==-1&&y.getValue()==-1) return true; //point at infinity always belongs
			return y*y == x*x*x + a*x + b;
		}
		INT_P getDiscriminant(){
			return 4*a*a + 27*b*b;
		}
		INT_P getCurveA() const{
			return a;
		}
		INT_P getCurveB() const{
			return b;
		}
		template<typename T>
		EcElement getEcPoint(T x, T y);
		EcElement getEcPoint(INT_P x, INT_P y);
		EcElement getPointAtInfinity();
};

class EcElement : public EllipticCurve{
	typedef FieldElement INT_P;
	private:
		bool point_at_infinity;
		bool isEqual(EcElement ece2) const{
			assert( p==ece2.getCharacteristic() );
			assert( a==ece2.getCurveA());
			assert( b==ece2.getCurveB());
			if(this->isPointAtInfinity()&&ece2.isPointAtInfinity())
				return true;
			return ece2.getEcElementX() == x 
					&& ece2.getEcElementY() == y;
		}
	protected:
		using Field::p;
		using EllipticCurve::a;
		using EllipticCurve::b;
		INT_P x, y;
	public:		
		using EllipticCurve::isBelong;
		//TODO: Use SFINAE because it might be that some other template also matches but there's another one intended for that signature
		EcElement(){}

		template<class F> // F can be field or int/long/mpz_class
		EcElement( const INT_P& x_arg, const INT_P& y_arg, const INT_P& a, const INT_P& b, const F& p) 
																		: EllipticCurve(a, b, p), x(x_arg), y(y_arg){
			point_at_infinity = false;
			assert(isBelong(x, y));
		}
		template<class T, class U, class V, class W, class F> // T, U, V, W can be int/long/mpz_class.  F can be all these or a field 
		EcElement( T x_arg, U y_arg, V a, W b, F p) : EllipticCurve(a, b, p){
			//assert(p is prime)
			point_at_infinity = false;
			x = getFieldElement(x_arg, p);
			y = getFieldElement(y_arg, p);
			assert(isBelong(x, y));
		}

		EcElement(const INT_P& x_arg, const INT_P& y_arg, const EllipticCurve& curve) 
																		: EllipticCurve(curve), x(x_arg), y(y_arg){
			point_at_infinity = false;
			assert(isBelong(x, y));
		}

		template<class T>
		EcElement(const  T& x_arg, const T& y_arg, const EllipticCurve& curve) 
														: EllipticCurve(curve), 
															x(x_arg, curve.getCharacteristic()), 
															y(y_arg, curve.getCharacteristic()){
			point_at_infinity = false;
			assert(isBelong(x, y));
		}
		
		EcElement(const EllipticCurve& curve):EllipticCurve(curve){
			point_at_infinity = true;
		}

		INT_P getEcElementX() const{
			assert(!this->isPointAtInfinity());
			return x;
		}
		
		INT_P getEcElementY() const{
			assert(!this->isPointAtInfinity());
			return y;
		}
		
		bool isPointAtInfinity() const{
			return point_at_infinity;
		}
		
		EcElement getInverse(){		
			if(this->isPointAtInfinity()) return *this;	
			EcElement ret(x, -y, a, b, p);
			return ret;
		}
		
		EllipticCurve getCurve() const{
			EllipticCurve ec(a, b, p);
			return ec;
		}
		
		EcElement operator+(EcElement ece2);
		
		bool operator==(EcElement ece2){
			return this->isEqual(ece2);
		}
};

template<typename T>
EcElement EllipticCurve::getEcPoint(T x, T y){
	EcElement ret(x, y, a, b, p);
	return ret;
}

EcElement EllipticCurve::getEcPoint(INT_P x, INT_P y){
	assert(x.getCharacteristic()==p);
	assert(y.getCharacteristic()==p);
	EcElement ret(x, y, a, b, p);
	return ret;
}
EcElement EllipticCurve::getPointAtInfinity(){
	EcElement ret(*this);
	return ret;
}



// May refer https://tools.ietf.org/html/rfc6090#appendix-F.1 for testing
// Current implementation is by my understanding
EcElement EcElement::operator+(EcElement ece2){
	assert( p==ece2.getCharacteristic() 
		&& a==ece2.getCurveA()
		&& b==ece2.getCurveB());
	if(this->isPointAtInfinity()||ece2.isPointAtInfinity()){
		if(this->isPointAtInfinity()){
			return ece2;
		}
		else{
			return *this;
		}
	}
	INT_P x3, y3;
	INT_P x1 = x, y1 = y;
	INT_P x2 = ece2.getEcElementX(),
		 y2 = ece2.getEcElementY();

	EcElement ret;
	if(!this->isEqual(ece2)){
		if(x1!=x2){
			INT_P slope			=	(y2 - y1) * inv(x2 - x1);
			INT_P slope_squared	=	slope * slope;	
			x3				=	slope_squared - x1 - x2;
			y3				=	slope * (x1 - x3) - y1;
			ret = EcElement(x3, y3, a, b, p);
		}
		else{ // when x1 = x2. line is parallel to y-axis. 
			ret = this->getPointAtInfinity();
		}
	}
	else{ // equal. 
		if(y1!=0){ //equal so y2!=0 too.
			// the slope here is of the tangent
			INT_P slope			= 	(mzc(3) * x1 * x1 + a) * inv(mzc(2) * y1);
			INT_P slope_squared	=	slope * slope;
			x3					=	slope_squared - x1 - x2;
			y3					=	slope * (x1 - x3) - y1;
			ret = EcElement(x3, y3, a, b, p);
		}
		else{ //equal and y = 0 so tangent at the bell of the curve
			ret = this->getPointAtInfinity();
		}
	}
	return ret;
}

template<class T> //int, long, mpz_class etc
EcElement operator*(T k, EcElement ece){
	if(k==0){
		return ece.getPointAtInfinity(); 
	}
	EcElement ecesqrt = (k/2)*ece;
	EcElement ret = ecesqrt + ecesqrt;
	if(k%2==1){
		ret = ret + ece;
	} 
	return ret;
}

EllipticCurve getP256Curve(){  // defined by J. Solinas and D. Fu in [RFC5903]. I learned from https://tools.ietf.org/html/rfc6090#appendix-D
	// !!Make sure to offer many other non-nsa implementations. Refer http://safecurves.cr.yp.to/  //TODO
	mpz_class pr("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF", 16);
	mpz_class a("-3", 16); a = a+pr;
	mpz_class b("5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B", 16);
	// Group order n = FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551 in base 16
	// generator affine coordinates are 
	//	gx: 6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
	//	gy: 4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5

	Field fpr(pr);

	FieldElement fea(a, fpr), feb(b, fpr);

	EllipticCurve curve(fea, feb, fpr);
	return curve;
}

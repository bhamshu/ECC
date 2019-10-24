// A demonstration of how Diffie Hellman Key exchange would work over elliptic curves

#include<gmpxx.h>
#include "ec.cpp"
#include<iostream>
#include<utility>
#include <assert.h>

#define watch(x) std::cout<<(#x)<<" is "<<x<<"\n\n";

mpz_class getRandom(){
	gmp_randclass r(gmp_randinit_default);
	r.seed(mpz_class(rand())*mpz_class(time(NULL))); // ~60 bit seed
	mp_bitcnt_t numbits = 256;
	return r.get_z_bits(numbits);
}

int main(){
	std::pair<EllipticCurve, EcElement> p = getP256CurveAndGenerator();

	// Let's say we have two entities Ada and Charles who want to communicate. Let Eve be and eavesdropper who can see all communications that happen between them.
	// We assume that Ada and Charles already have a way to make sure that the incoming messages are coming from one another only and not through a man in the middle (this can be achieved by digital signatures through a trusted third party)
	
	mpz_class AdaSecret = getRandom(); //will be used as a secret key of Ada. No one else knows this.
	mpz_class CharlesSecret = getRandom();
	
	EcElement AdaPublic = AdaSecret*p.second; // A_pri*generator. this will be transmitted over the channel (to Charles) and can be seen by Eve.
	EcElement CharlesPublic = CharlesSecret*p.second; //B_pri*secret*generator. this will be transmitted over the channel(to Alice) and can be seen by Eve.

	EcElement AdaCalculatesSharedSecret = AdaSecret*CharlesPublic;
	EcElement CharlesCalculatesSharedSecret = CharlesSecret*AdaPublic;

	watch(AdaSecret);
	watch(AdaPublic);
	watch(CharlesSecret);
	watch(CharlesPublic);
	watch(AdaCalculatesSharedSecret);
	watch(CharlesCalculatesSharedSecret);


	assert(AdaCalculatesSharedSecret == CharlesCalculatesSharedSecret); //now this can be used for symmetric encryption

	std::cout<<"Key Exchange Successful!\n";
	return 0;
}
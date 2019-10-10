#define UTILITIES_CPP
typedef long long ll;

/*
Deterministically testing primality in sqrt of magnitude would be too expensive
Deterministically testing primality in polynomial time would be to complicated to implement here.
Probabilistic tests like Miller Rabin should be implemented and used cautiously //TODO. 
bool isPrime(int x){
	int f = 2;
	while(f*f <= x){
		if(x%f==0)
			return false;
		f++;
	}
	return true;
}
*/

int powmod(int b, int e, int m){
	if(e==0)
		return 1;
	int t = powmod(b, e/2, m);
	return (((ll)t * (ll)t)%m * (ll)((e%2)?b:1))%m;
}

int inv(int x, int p){	//returns 1/x mod p where p is prime
	//assert(isPrime(p));
	int ret = powmod(x, p-2, p);
	assert( ((ll)x*(ll)ret)%p == 1); // much less expensive!
	return ret;
}
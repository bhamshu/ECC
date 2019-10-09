#define UTILITIES_CPP
typedef long long ll;

bool isPrime(int x){
	int f = 2;
	while(f*f <= x){
		if(x%f==0)
			return false;
		f++;
	}
	return true;
}

int powmod(int b, int e, int m){
	if(e==0)
		return 1;
	int t = powmod(b, e/2, m);
	return (((ll)t * (ll)t)%m * (ll)((e%2)?b:1))%m;
}

int inv(int x, int p){	//returns 1/x mod p where p is prime
	assert(isPrime(p));
	return powmod(x, p-2, p);
}
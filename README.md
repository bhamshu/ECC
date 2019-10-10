# Elliptic Curve Cryptography

Discrete Logarithm Problem over Elliptic Curves is a computationally hard problem. No publically known attacks exist which can solve it in better than sqrt(N) complexity where N is the magnitude of the characteristic prime number of the field over which the discrete elliptic curve is defined. Thus, it is exponentially hard in the number of bits of the prime number. 

This library implements operations over finite fields(of prime charecteristic only) and over elliptic curves over finite fields. I have implemented the best algorithms known to me for all operations.

Soon,I will be adding cryptographic algorithms like Diffie-Hellman key exchange over elliptic curves and other algorithms as I study them. Contributions are welcome!

**A word of caution:**
As of now, I have designed it to work for 32-bit integers which is very very low for any real world cryptographic purposes.  I plan to use [GMP](https://gmplib.org/) in future updates so as to support standard 160/192 bit versions.  

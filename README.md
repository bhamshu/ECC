# Elliptic Curve Cryptography

Discrete Logarithm Problem over Elliptic Curves is a computationally hard problem. No publically known attacks exist which can solve it in better than sqrt(N) complexity where N is the magnitude of the characteristic prime number of the field over which the discrete elliptic curve is defined. Thus, it is exponentially hard in the number of bits of the prime number. 

This library implements operations over finite fields(of prime charecteristic only) and over elliptic curves over finite fields. I have implemented the best\* algorithms known to me for all operations.

**How to use?**  Run either diffie-hellman.cpp or test.cpp. Make sure to use `-lgmpxx -lgmp` flags for GMP. (Example: `g++ diffie-hellman.cpp -Wall -lgmpxx -lgmp && ./a.out`). Docs have not been written but the interfaces can easily be understood from these two files.

**//TODO**

Inheritance is not suitable for this use case. Each `EcElement` stores all the parameters of its curve. This is too much overhead - each point on the curve carries its own curve! Instead, it should just store a pointer to an object of type curve. Fixing this needs overhauling the entire codebase. (I just learned this principle is well understood and is called "Composition over Inheritance".)

Yet to implement Digital Signatures.

Need to write more tests, thinking of the edge case scenarios.

Need to hard code more in-built curves. Refer http://safecurves.cr.yp.to for this.

Need to add primality tests.

\* Best in the sense of asymptotic complexity. Almost everywhere, readability has been given more preference over minor optimizations.

*PS. Feel free to contact me (shubhamdtu20@gmail.com) for support, I'll be glad to help. Please drop a word if you use it to let me know that you found it helpful. By the way, contributions are welcome!*

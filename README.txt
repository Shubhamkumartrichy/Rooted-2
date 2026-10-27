Assignment: Q1–Q9

Name: Shubham Kumar
Roll Number: CS26MTECH12012
Operating System: Ubuntu Linux
Programming Language: C++
Compiler: g++
Compiler Version: g++ (Ubuntu 15.2.0-16ubuntu1) 15.2.0


COMPILATION AND EXECUTION
----------------------------

Compile each program separately using g++ and the C++17 standard.

Q1:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q1.cpp -o Q1
    ./Q1

Q2:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q2.cpp -o Q2
    ./Q2

Q3:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q3.cpp -o Q3
    ./Q3

Q4:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q4.cpp -o Q4
    ./Q4

Q5:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q5.cpp -o Q5
    ./Q5

Q6:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q6.cpp -o Q6
    ./Q6

Q7:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q7.cpp -o Q7
    ./Q7

Q8:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q8.cpp -o Q8
    ./Q8

Q9:
    g++ -std=c++17 -O2 -Wall -Wextra -pedantic Q9.cpp -o Q9
    ./Q9

Each compilation command creates an executable for the corresponding
question. Run the executable and provide input through standard input
according to that question's specified input format.

To provide input from a file, use the following format:

    ./Q1 < input.txt

Replace Q1 with the appropriate executable name.




Q9 (b): Kronecker Product and Its Connection to FFT
===================================================

(i) Feasibility of Explicit Output in O(n log n) Time
-----------------------------------------------------

The claim is false.

For two n-dimensional vectors u and v, their Kronecker product z is defined by:

    z[i*n + j] = u[i] * v[j], for 0 <= i, j < n.

The output contains n^2 entries because there are n choices for i and n choices for j.

Since outputting each scalar takes constant time, explicitly outputting all n^2 entries requires at least Omega(n^2) time.

As n^2 grows asymptotically faster than n log n, no algorithm can explicitly output the complete Kronecker product in O(n log n) time. 
Therefore, the claim is false, regardless of whether FFT is used.

(ii) Implicit Representation Using O(n) Space
---------------------------------------------

Instead of storing all n^2 entries, store only the original vectors u and v. This requires O(n) space.

To query an entry z[k], where 0 <= k < n^2, calculate:

    i = floor(k / n)
    j = k mod n

Then return:

    z[k] = u[i] * v[j]

Integer division and remainder, array accesses, and scalar multiplication each take constant time. Therefore, each query takes O(1) time, 
while the representation requires O(n) space.

This approach avoids explicitly storing the entire Kronecker product.

(iii) Kronecker Product, Polynomial Convolution, and FFT
-------------------------------------------------------

The Kronecker product produces a separate entry for every pair (i, j):

    z[i*n + j] = u[i] * v[j]

Consequently, it contains n^2 entries.

Polynomial convolution is defined by:

    s[k] = sum of u[i] * v[j] over all pairs satisfying i + j = k.

For two vectors of length n, the convolution contains 2n - 1 entries. Unlike the Kronecker product, convolution adds together 
products whose indices have the same sum.

To understand the connection with polynomial multiplication, define:

    U(x) = u[0] + u[1]*x + ... + u[n-1]*x^(n-1)

    V(x) = v[0] + v[1]*x + ... + v[n-1]*x^(n-1)

The product polynomial S(x) = U(x) * V(x) has coefficients:

    s[k] = sum of u[i] * v[j] over all pairs satisfying i + j = k.

Thus, polynomial multiplication is equivalent to convolution of the coefficient vectors.

FFT computes convolution efficiently through these steps:

1. Choose L as the smallest power of two satisfying L >= 2n - 1.
2. Pad both vectors with zeros to length L.
3. Apply FFT to both padded vectors.
4. Multiply their corresponding evaluated values pointwise.
5. Apply the inverse FFT to recover the product coefficients.

Using a suitable transform length proportional to n, convolution can be computed in O(n log n) time.

This does not contradict the lower bound for the Kronecker product. Convolution produces only O(n) output entries, 
whereas the Kronecker product produces n^2 entries. Explicitly writing the Kronecker product therefore requires Omega(n^2) time, 
regardless of the computational method used.
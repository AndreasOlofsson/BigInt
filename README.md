# BigInt

A simple library for arbitrary-length integers written in c++.

This is an old project I wrote a long time ago for use in some other projects. It's far from the best or most efficient bigint library available but it's useful for smaller projects or testing.

## Description

The project is meant to be as simple as possible, it has no dependancies and is implemented in only one file. In addition almost every function is defined as an operator which lets you use it similar to any other number type in c++.

## Example

### Modular Exponentiation
```C++
#include <iostream>
#include "bigint.h"

BigInt modpow(BigInt base, BigInt exp, const BigInt& mod)
{
    BigInt product(1);

    while (!exp.isZero())
    {
        if (exp[0] & 1)
        {
            product *= base;
            product %= mod;
        }

        exp >>= 1;
        base *= base;
        base %= mod;
    }

    return product;
}

int main()
{
    cout << modpow(
        2988348162058574136915891421498819466320163312926952423791023078876139_bi,
        2351399303373464486466122544523690094744975233415544072992656881240319_bi,
        10000000000000000000000000000000000000000_bi
    ) << endl;
}
```

outputs `1527229998585248450016808958343740453059`
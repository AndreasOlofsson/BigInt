#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdint.h>
#include <string>
#include <ostream>

typedef uint32_t bigint_type;

class BigInt
{
public:
    BigInt();
    BigInt(const BigInt&);
    ~BigInt();

    explicit BigInt(int i);
    explicit BigInt(const char* value);
    explicit BigInt(const char* value, int radix);

    operator bool() const;
    explicit operator char() const;
    explicit operator short() const;
    explicit operator int() const;
    explicit operator long() const;
    explicit operator long long() const;
    explicit operator unsigned char() const;
    explicit operator unsigned short() const;
    explicit operator unsigned int() const;
    explicit operator unsigned long() const;
    explicit operator unsigned long long() const;

    BigInt& operator=(const BigInt&);

    BigInt operator+() const;
    BigInt operator-() const;

    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator%(const BigInt&) const;

    BigInt& operator++();
    BigInt& operator--();
    BigInt operator++(int);
    BigInt operator--(int);

    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator>=(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<(const BigInt&) const;

    bool operator!() const;
    bool operator&&(const BigInt&) const;
    bool operator||(const BigInt&) const;

    BigInt operator~() const;
    BigInt operator&(const BigInt&) const;
    BigInt operator|(const BigInt&) const;
    BigInt operator^(const BigInt&) const;
    BigInt operator<<(int) const;
    BigInt operator>>(int) const;

    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator%=(const BigInt&);

    BigInt& operator&=(const BigInt&);
    BigInt& operator|=(const BigInt&);
    BigInt& operator^=(const BigInt&);
    BigInt& operator<<=(int);
    BigInt& operator>>=(int);

    bigint_type operator[](int) const;

    bool isZero() const;
    bool isPositive() const;
    bool isNegative() const;

    void divmod(const BigInt&, BigInt* quotient, BigInt* remainder) const;

    std::string toString() const;
    std::string toString(int radix) const;

    static const BigInt negative_one;
    static const BigInt zero;
    static const BigInt one;
    static const BigInt ten;

private:
    int sign;
    int length;
    bigint_type* value;

    BigInt(int sign, int length, bigint_type* value);
};

std::ostream& operator<<(std::ostream& stream, const BigInt& bigint);

BigInt operator "" _bi(const char*);

#endif /* __BIGINT_H__ */

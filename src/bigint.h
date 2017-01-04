#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdint.h>
#include <string>

class BigInt
{
public:
	BigInt();
	BigInt(const BigInt&);
	~BigInt();

	explicit BigInt(int i);
	// explicit BigInt(const char* value);
	// explicit BigInt(const char* value, int radix);

	//BigInt& operator=(const BigInt&);

	//BigInt operator+() const;
	//BigInt operator-() const;

	BigInt operator+(const BigInt&) const;
	BigInt operator-(const BigInt&) const;
	// BigInt operator*(const BigInt&) const;
	// BigInt operator/(const BigInt&) const;
	// BigInt operator%(const BigInt&) const;

	// BigInt& operator++();
	// BigInt& operator--();
	// BigInt operator++(int);
	// BigInt operator--(int);

	// bool operator==(const BigInt&) const;
	// bool operator!=(const BigInt&) const;
	// bool operator>=(const BigInt&) const;
	// bool operator<=(const BigInt&) const;
	// bool operator>(const BigInt&) const;
	// bool operator<(const BigInt&) const;

	// bool operator!();
	// bool operator&&(const BigInt&); TODO more types?
	// bool operator||(const BigInt&);

	// BigInt operator~() const;
	// BigInt operator&(const BigInt&) const;
	// BigInt operator|(const BigInt&) const;
	// BigInt operator^(const BigInt&) const;
	// BigInt operator<<(int) const;
	// BigInt operator>>(int) const;

	// BigInt& operator+=(const BigInt&) const;
	// BigInt& operator-=(const BigInt&) const;
	// BigInt& operator*=(const BigInt&) const;
	// BigInt& operator/=(const BigInt&) const;
	// BigInt& operator%=(const BigInt&) const;

	// BigInt& operator&=(const BigInt&) const;
	// BigInt& operator|=(const BigInt&) const;
	// BigInt& operator^=(const BigInt&) const;
	// BigInt& operator<<=(int) const;
	// BigInt& operator>>=(int) const;

	// uint32_t operator[](int);

	std::string dbgstr() const; // DEBUG

private:
	int sign;
	int length;
	uint32_t* value;

	BigInt(int sign, int length, uint32_t* value);
};

BigInt operator "" _bi(const char*); // XXX Only accepts int-sized values

#endif /* __BIGINT_H__ */

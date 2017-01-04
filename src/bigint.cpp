#include "bigint.h"

#include <stdio.h>
#include <iostream> // DEBUG
#include <sstream>

using namespace std;

BigInt::BigInt()
{
	sign = 0;
	length = 0;
	value = 0;
}

BigInt::BigInt(const BigInt& bigint)
{
	sign = bigint.sign;
	length = bigint.length;
	value = new uint32_t[length];
	for(int i = 0; i < length; i++)
	{
		value[i] = bigint.value[i];
	}
}

BigInt::~BigInt()
{
	delete value;
}

BigInt::BigInt(int i)
{
	sign = i < 0 ? 1 : 0;
	length = 1;
	value = new uint32_t[1];
	*value = i;
}

void __bigint_add(int* sign, int* length, uint32_t*  value,
				  int bsign, int blength, uint32_t* bvalue,
				  int carry, int invb)
{
	for(int i = 0; i < *length; i++)
	{
		uint32_t last_value = value[i];

		if(i < blength)
		{
			value[i] += (invb ? ~bvalue[i] : bvalue[i]) + carry;
			carry = carry ? last_value >= value[i] : last_value > value[i];
		}
		else
		{
			value[i] += carry + (bsign != invb ? ~0 : 0);
			carry = carry ? last_value >= value[i] : last_value > value[i];

			if(!(bsign != invb) && !carry)
			{
				break;
			}
		}
	}

	*sign = (*sign != (bsign != invb)) != carry;

	while(*length > 0 && value[*length - 1] == (*sign ? ~0 : 0))
	{
		(*length)--;
	}
}

BigInt BigInt::operator+(const BigInt& b) const
{
	int new_length = (length > b.length ? length : b.length) + 1;
	uint32_t* new_value = new uint32_t[new_length];

	for(int i = 0; i < length; i++)
	{
		new_value[i] = value[i];
	}
	for(int i = length; i < new_length; i++)
	{
		new_value[i] = sign ? ~0 : 0;
	}

	int new_sign = sign;

	__bigint_add(&new_sign, &new_length, new_value,
				 b.sign, b.length, b.value, 0, 0);

	return BigInt(new_sign, new_length, new_value);
}

BigInt BigInt::operator-(const BigInt& b) const
{
	int new_length = length > b.length ? length : b.length;
	uint32_t* new_value = new uint32_t[new_length];

	for(int i = 0; i < length; i++)
	{
		new_value[i] = value[i];
	}

	int new_sign = sign;

	__bigint_add(&new_sign, &new_length, new_value,
				 b.sign, b.length, b.value, 1, 1);

	return BigInt(new_sign, new_length, new_value);
}

string BigInt::dbgstr() const
{
	stringstream ss;
	ss << sign << ' ';
	for(int i = length - 1; i >= 0; i--)
	{
		ss.flags(ios::hex);
		ss.fill('0');
		ss.width(8);
		ss << value[i] << ' ';
	}
	return ss.str();
}

BigInt::BigInt(int sign, int length, uint32_t* value)
	:sign(sign), length(length), value(value) {}

BigInt operator "" _bi(const char* str) // TODO accept larger values (update .h)
{
	if(str[0] == '0' && str[1] == 'x')
	{
		return BigInt(stoi(string(str + 2), 0, 16));
	}
	else if(str[0] == '0' && str[1] == 'b')
	{
		return BigInt(stoi(string(str + 2), 0, 2));
	}
	else if(str[0] == '0')
	{
		return BigInt(stoi(string(str + 1), 0, 8));
	}
	else
	{
		return BigInt(atoi(str));
	}
}

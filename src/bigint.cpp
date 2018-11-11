#include "bigint.h"

#include <stdio.h>

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
    value = new bigint_type[length];
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
    if (i == 0 || i == -1)
    {
        length = 0;
        value = 0;
    }
    else
    {
        length = 1;
        value = new bigint_type[1];
        *value = i;
    }
}

BigInt::BigInt(const char* str)
{
    int negative = 0;

    if (str[0] == '-')
    {
        negative = 1;
        str++;
    }
    else if (str[0] == '+')
    {
        str++;
    }

    length = 0;
    value = 0;
    sign = 0;

    int radix;

    if (str[0] == '0' && str[1] == 'x')
    {
        radix = 16;
        str += 2;
    }
    else if (str[0] == '0' && str[1] == 'b')
    {
        radix = 2;
        str += 2;
    }
    else if (str[0] == '0')
    {
        radix = 8;
        str += 1;
    }
    else
    {
        radix = 10;
    }

    if (radix == 10)
    {
        for (; *str != 0; str++)
        {
            if ('0' <= *str && *str <= '9')
            {
                *this *= BigInt::ten;
                *this += BigInt(*str - '0');
            }
        }
    }
    else
    {
        int shift = radix == 16 ? 4 : (radix == 8 ? 3 : 1);

        for (; *str != 0; str++)
        {
            int digit;
            if ('0' <= *str && *str <= '9')
            {
                digit = *str - '0';
            }
            else if ('a' <= *str && *str <= 'f')
            {
                digit = *str - 'a' + 10;
            }
            else if ('A' <= *str && *str <= 'F')
            {
                digit = *str - 'A' + 10;
            }
            else
            {
                digit = 16;
            }

            if (digit >= radix)
            {
                continue;
            }

            *this <<= shift;
            *this |= BigInt(digit);
        }
    }

    if (negative)
    {
        *this = -*this;
    }
}

BigInt::BigInt(const char* str, int radix)
{
    int negative = 0;

    if (str[0] == '-')
    {
        negative = 1;
        str++;
    }
    else if (str[0] == '+')
    {
        str++;
    }

    sign = 0;
    length = 0;
    value = 0;

    BigInt radixBI(radix);

    for (; *str != 0; str++)
    {
        if ('0' <= *str && *str <= '9')
        {
            *this *= radixBI;
            *this += BigInt(*str - '0');
        }
        else if ('a' <= *str && *str <= 'f')
        {
            *this *= radixBI;
            *this += BigInt(*str - 'a' + 10);
        }
        else if ('A' <= *str && *str <= 'F')
        {
            *this *= radixBI;
            *this += BigInt(*str - 'A' + 10);
        }
    }

    if (negative)
    {
        *this = -*this;
    }
}

BigInt::operator bool() const
{
    return !isZero();
}

#define __bigint_conversion(_result_type) \
    BigInt::operator _result_type() const                                             \
    {                                                                                 \
        unsigned _result_type result = 0;                                             \
        int i;                                                                        \
        for (i = 0; i < sizeof(_result_type) && i < length; i += sizeof(bigint_type)) \
        {                                                                             \
            result |= value[i] << (i * 8);                                            \
        }                                                                             \
        for (; i < sizeof(_result_type); i++)                                         \
        {                                                                             \
            result |= (sign ? 0xFF : 0) << (i * 8);                                   \
        }                                                                             \
        if (sign)                                                                     \
        {                                                                             \
            result |= (_result_type) 1 << (sizeof(_result_type) * 8 - 1);             \
        }                                                                             \
        else                                                                          \
        {                                                                             \
            result &= ~((_result_type) 1 << (sizeof(_result_type) * 8 - 1));          \
        }                                                                             \
        return (_result_type) result;                                                 \
    }

#define __bigint_conversion_unsigned(_result_type) \
    BigInt::operator unsigned _result_type() const                                    \
    {                                                                                 \
        unsigned _result_type result = 0;                                             \
        int i;                                                                        \
        for (i = 0; i < sizeof(_result_type) && i < length; i += sizeof(bigint_type)) \
        {                                                                             \
            result |= value[i] << (i * 8);                                            \
        }                                                                             \
        for (; i < sizeof(_result_type); i++)                                         \
        {                                                                             \
            result |= (sign ? 0xFF : 0) << (i * 8);                                   \
        }                                                                             \
        return (_result_type) result;                                                 \
    }

__bigint_conversion(char)
__bigint_conversion(short)
__bigint_conversion(int)
__bigint_conversion(long)
__bigint_conversion(long long)
__bigint_conversion_unsigned(char)
__bigint_conversion_unsigned(short)
__bigint_conversion_unsigned(int)
__bigint_conversion_unsigned(long)
__bigint_conversion_unsigned(long long)

BigInt& BigInt::operator=(const BigInt& bigint)
{
    sign = bigint.sign;
    length = bigint.length;
    delete value;
    value = new bigint_type[length];

    for (int i = 0; i < length; i++)
    {
        value[i] = bigint.value[i];
    }

    return *this;
}

BigInt BigInt::operator+() const
{
    return *this;
}

BigInt BigInt::operator-() const
{
    if (sign)
    {
        for (int i = 0; i < length; i++)
        {
            if (!(i == 0 && value[i] == 1 || value[i] == 0))
            {
                bigint_type* new_value = new bigint_type[length];

                int carry = 1;
                for (int i = 0; i < length; i++)
                {
                    new_value[i] = ~value[i] + carry;
                    carry = carry && new_value[i] == 0;
                }

                return BigInt(0, length, new_value);
            }
        }

        bigint_type* new_value = new bigint_type[length+1];

        for (int i = 1; i < length; i++)
        {
            new_value[i] = 0;
        }

        new_value[0] = value[0] == 0;
        new_value[length] = 1;

        return BigInt(0, length+1, new_value);
    }
    else
    {
        bigint_type* new_value = new bigint_type[length];

        int carry = 1;
        for (int i = 0; i < length; i++)
        {
            new_value[i] = ~value[i] + carry;
            carry = carry && new_value[i] == 0;
        }

        int new_length = length;

        while (new_length > 0 && new_value[new_length - 1] == (bigint_type) ~0)
        {
            new_length--;
        }

        return BigInt(1, new_length, new_value);
    }
}

void __bigint_add(int* sign, int* length, bigint_type*  value,
                  int bsign, int blength, bigint_type* bvalue,
                  int carry, int invb)
{
    for (int i = 0; i < *length; i++)
    {
        bigint_type last_value = value[i];

        if (i < blength)
        {
            value[i] += (invb ? ~bvalue[i] : bvalue[i]) + carry;
            carry = carry ? last_value >= value[i] : last_value > value[i];
        }
        else
        {
            value[i] += carry + (bigint_type) (bsign != invb ? ~0 : 0);
            carry = carry ? last_value >= value[i] : last_value > value[i];

            if (!(bsign != invb) && !carry)
            {
                break;
            }
        }
    }

    *sign = (*sign != (bsign != invb)) != carry;

    while (*length > 0 && value[*length - 1] == (bigint_type) (*sign ? ~0 : 0))
    {
        (*length)--;
    }
}

BigInt BigInt::operator+(const BigInt& b) const
{
    int new_length = (length > b.length ? length : b.length) + 1;
    bigint_type* new_value = new bigint_type[new_length];

    for (int i = 0; i < length; i++)
    {
        new_value[i] = value[i];
    }
    for (int i = length; i < new_length; i++)
    {
        new_value[i] = (bigint_type) (sign ? ~0 : 0);
    }

    int new_sign = sign;

    __bigint_add(&new_sign, &new_length, new_value,
                 b.sign, b.length, b.value, 0, 0);

    return BigInt(new_sign, new_length, new_value);
}

BigInt BigInt::operator-(const BigInt& b) const
{
    int new_length = length > b.length ? length : b.length;
    bigint_type* new_value = new bigint_type[new_length];

    for (int i = 0; i < length; i++)
    {
        new_value[i] = value[i];
    }

    int new_sign = sign;

    __bigint_add(&new_sign, &new_length, new_value,
                 b.sign, b.length, b.value, 1, 1);

    return BigInt(new_sign, new_length, new_value);
}

BigInt BigInt::operator*(const BigInt& b) const
{
    if (b.sign)
    {
        return -(*this * (-b));
    }

    BigInt result;

    int shift = 0;

    for (int i = 0; i < b.length; i++)
    {
        for (int j = 0; j < sizeof(bigint_type) * 8; j++)
        {
            if ((b[i] >> j) & 1)
            {
                result += *this << shift;
            }

            shift++;
        }
    }

    return result;
}

BigInt BigInt::operator/(const BigInt& b) const
{
    BigInt quotient;
    divmod(b, &quotient, 0);
    return quotient;
}

BigInt BigInt::operator%(const BigInt& b) const
{
    BigInt remainder;
    divmod(b, 0, &remainder);
    return remainder;
}

BigInt& BigInt::operator++()
{
    if (sign && length == 0)
    {
        return *this = BigInt::zero;
    }

    int carry = 1;
    for (int i = 0; i < length; i++)
    {
        value[i]++;
        if (value[i] != 0)
        {
            carry = 0;
            break;
        }
    }

    if (carry)
    {
        bigint_type* new_value = new bigint_type[length + 1];
        for (int i = 0; i < length; i++)
        {
            new_value[i] = value[i];
        }
        new_value[length] = 1;
        length++;

        delete value;
        value = new_value;
    }

    while (length > 0 && value[length - 1] == (bigint_type) (sign ? ~0 : 0))
    {
        length--;
    }

    return *this;
}

BigInt& BigInt::operator--()
{
    if (!sign && length == 0)
    {
        return *this = BigInt::negative_one;
    }

    int carry = 1;
    for (int i = 0; i < length; i++)
    {
        value[i]--;
        if (value[i] != (bigint_type) ~0)
        {
            carry = 0;
            break;
        }
    }

    if (carry)
    {
        bigint_type* new_value = new bigint_type[length + 1];
        for (int i = 0; i < length; i++)
        {
            new_value[i] = value[i];
        }
        new_value[length] = (bigint_type) ~1;
        length++;

        delete value;
        value = new_value;
    }

    while (length > 0 && value[length - 1] == (bigint_type) (sign ? ~0 : 0))
    {
        length--;
    }

    return *this;
}

BigInt BigInt::operator++(int)
{
    BigInt result = *this;
    ++*this;
    return result;
}

BigInt BigInt::operator--(int)
{
    BigInt result = *this;
    --*this;
    return result;
}

bool BigInt::operator==(const BigInt& b) const
{
    if (sign != b.sign)
    {
        return false;
    }

    for (int i = 0; i < length || i < b.length; i++)
    {
        if ((*this)[i] != b[i])
        {
            return false;
        }
    }

    return true;
}

bool BigInt::operator!=(const BigInt& b) const
{
    for (int i = 0; i < length || i < b.length; i++)
    {
        if ((*this)[i] != b[i])
        {
            return true;
        }
    }

    return sign != b.sign;
}

bool BigInt::operator>=(const BigInt& b) const
{
    return !(*this < b);
}

bool BigInt::operator<=(const BigInt&b) const
{
    return !(*this > b);
}

bool BigInt::operator>(const BigInt& b) const
{
    if (sign && !b.sign)
    {
        return false;
    }
    if (!sign && b.sign)
    {
        return true;
    }

    for (int i = length > b.length ? length : b.length; i >= 0; i--)
    {
        if ((*this)[i] > b[i])
        {
            return true;
        }
        else if ((*this)[i] < b[i])
        {
            return false;
        }
    }

    return false;
}

bool BigInt::operator<(const BigInt& b) const
{
    if (sign && !b.sign)
    {
        return true;
    }
    if (!sign && b.sign)
    {
        return false;
    }

    for (int i = length > b.length ? length : b.length; i >= 0; i--)
    {
        if ((*this)[i] > b[i])
        {
            return false;
        }
        else if ((*this)[i] < b[i])
        {
            return true;
        }
    }

    return false;
}

bool BigInt::operator!() const
{
    return !((bool) *this);
}

bool BigInt::operator&&(const BigInt& b) const
{
    return (bool) *this && (bool) b;
}

bool BigInt::operator||(const BigInt& b) const
{
    return (bool) *this || (bool) b;
}

BigInt BigInt::operator~() const
{
    bigint_type* new_value = new bigint_type[length];

    for (int i = 0; i < length; i++)
    {
        new_value[i] = ~value[i];
    }

    int new_length = length;

    while (new_length > 0 && new_value[new_length - 1] == (bigint_type) (!sign ? ~0 : 0))
    {
        new_length--;
    }

    return BigInt(!sign, new_length, new_value);
}

BigInt BigInt::operator&(const BigInt& b) const
{
    int new_sign = sign && b.sign;
    int new_length = length > b.length ? length : b.length;
    bigint_type* new_value = new bigint_type[new_length];

    for (int i = 0; i < new_length; i++)
    {
        new_value[i] = (*this)[i] & b[i];
    }

    while (new_length > 0 && new_value[new_length - 1] == (bigint_type) (new_sign ? ~0 : 0))
    {
        new_length--;
    }

    return BigInt(new_sign, new_length, new_value);
}

BigInt BigInt::operator|(const BigInt& b) const
{
    int new_sign = sign || b.sign;
    int new_length = length > b.length ? length : b.length;
    bigint_type* new_value = new bigint_type[new_length];

    for (int i = 0; i < new_length; i++)
    {
        new_value[i] = (*this)[i] | b[i];
    }

    while (new_length > 0 && new_value[new_length - 1] == (bigint_type) (new_sign ? ~0 : 0))
    {
        new_length--;
    }

    return BigInt(new_sign, new_length, new_value);
}

BigInt BigInt::operator^(const BigInt& b) const
{
    int new_sign = sign != b.sign;
    int new_length = length > b.length ? length : b.length;
    bigint_type* new_value = new bigint_type[new_length];

    for (int i = 0; i < new_length; i++)
    {
        new_value[i] = (*this)[i] ^ b[i];
    }

    while (new_length > 0 && new_value[new_length - 1] == (bigint_type) (new_sign ? ~0 : 0))
    {
        new_length--;
    }

    return BigInt(new_sign, new_length, new_value);
}

BigInt BigInt::operator<<(int shift) const
{
    if (shift == 0)
    {
        return *this;
    }
    else if (shift < 0)
    {
        return *this >> (-shift);
    }
    else if (shift % (sizeof(bigint_type) * 8) == 0)
    {
        int words = shift / (sizeof(bigint_type) * 8);

        int new_length = length + words;
        bigint_type* new_value = new bigint_type[new_length];

        for (int i = 0; i < length; i++)
        {
            new_value[i + words] = value[i];
        }

        for (int i = 0; i < words; i++)
        {
            new_value[i] = 0;
        }

        while (new_length > 0 && new_value[new_length - 1] ==
               (bigint_type) (sign ? ~0 : 0))
        {
            new_length--;
        }

        return BigInt(sign, new_length, new_value);
    }
    else
    {
        int words = shift / (sizeof(bigint_type) * 8);
        int bits = shift % (sizeof(bigint_type) * 8);

        int new_length = length + words + 1;
        bigint_type* new_value = new bigint_type[new_length];

        for (int i = 0; i < new_length; i++)
        {
            new_value[i] =
                (*this)[i - words - 1] >> (sizeof(bigint_type) * 8 - bits) |
                (*this)[i - words]     << bits;
        }

        while (new_length > 0 && new_value[new_length - 1] ==
            (bigint_type) (sign ? ~0 : 0))
        {
            new_length--;
        }

        return BigInt(sign, new_length, new_value);
    }
}

BigInt BigInt::operator>>(int shift) const
{
    if (shift == 0)
    {
        return *this;
    }
    else if (shift < 0)
    {
        return *this >> (-shift);
    }
    else if (shift % (sizeof(bigint_type) * 8) == 0)
    {
        int words = shift / (sizeof(bigint_type) * 8);

        int new_length = length - words;
        bigint_type* new_value = new bigint_type[new_length];

        for (int i = 0; i < new_length; i++)
        {
            new_value[i] = value[i + words];
        }

        return BigInt(sign, new_length, new_value);
    }
    else
    {
        int words = shift / (sizeof(bigint_type) * 8);
        int bits = shift % (sizeof(bigint_type) * 8);

        int new_length = length - words;
        bigint_type* new_value = new bigint_type[new_length];

        for (int i = 0; i < new_length; i++)
        {
            new_value[i] =
                (*this)[i + words] >> bits |
                (*this)[i + words + 1] << (sizeof(bigint_type) * 8 - bits);
        }

        while (new_length > 0 && new_value[new_length - 1] ==
            (bigint_type) (sign ? ~0 : 0))
        {
            new_length--;
        }

        return BigInt(sign, new_length, new_value);
    }
}

BigInt& BigInt::operator+=(const BigInt& b)
{
    return *this = *this + b;
}

BigInt& BigInt::operator-=(const BigInt& b)
{
    return *this = *this - b;
}

BigInt& BigInt::operator*=(const BigInt& b)
{
    return *this = *this * b;
}

BigInt& BigInt::operator/=(const BigInt& b)
{
    divmod(b, this, 0);
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& b)
{
    divmod(b, 0, this);
    return *this;
}

BigInt& BigInt::operator&=(const BigInt& b)
{
    return *this = *this & b;
}

BigInt& BigInt::operator|=(const BigInt& b)
{
    return *this = *this | b;
}

BigInt& BigInt::operator^=(const BigInt& b)
{
    return *this = *this ^ b;
}

BigInt& BigInt::operator<<=(int shift)
{
    return *this = *this << shift;
}

BigInt& BigInt::operator>>=(int shift)
{
    return *this = *this >> shift;
}

bigint_type BigInt::operator[](int i) const
{
    if (i < 0)
    {
        return 0;
    }
    else if (i < length)
    {
        return value[i];
    }
    else
    {
        return (bigint_type) (sign ? ~0 : 0);
    }
}

bool BigInt::isZero() const
{
    return !sign && length == 0;
}

bool BigInt::isPositive() const
{
    return !sign && length != 0;
}

bool BigInt::isNegative() const
{
    return sign;
}

void BigInt::divmod(const BigInt& b, BigInt* _quotient, BigInt* _remainder) const
{
    if (*this == BigInt::zero || b == BigInt::zero)
    {
        if (_quotient != 0)
        {
            *_quotient = BigInt::zero;
        }
        if (_remainder != 0)
        {
            *_remainder = BigInt::zero;
        }
        return;
    }

    BigInt quotient, remainder;

    for (int i = length - 1; i >= 0; i--)
    {
        for (int j = sizeof(bigint_type) * 8 - 1; j >= 0; j--)
        {
            remainder <<= 1;

            if (((*this)[i] >> j) & 1)
            {
                remainder |= BigInt::one;
            }

            if (remainder >= b)
            {
                remainder -= b;
                quotient |= BigInt::one << (int) (i * sizeof(bigint_type) * 8 + j);
            }
        }
    }
        if (_quotient != 0)
        {
            *_quotient = quotient;
        }
        if (_remainder != 0)
        {
            *_remainder = remainder;
        }
}

string BigInt::toString() const
{
    return toString(10);
}

string BigInt::toString(int radix) const
{
    if (isZero())
    {
        return "0";
    }

    if (isNegative())
    {
        return "-" + (-*this).toString(radix);
    }

    string output;

    BigInt radixBI(radix);

    BigInt value(*this);
    BigInt digit;

    while (value > BigInt::zero)
    {
        value.divmod(radixBI, &value, &digit);
        char c = digit[0];
        if(c < 10)
        {
            c += '0';
        }
        else
        {
            c += 'a';
        }
        output = c + output;
    }

    return output;
}

const BigInt BigInt::negative_one(-1);
const BigInt BigInt::zero(0);
const BigInt BigInt::one(1);
const BigInt BigInt::ten(10);

BigInt::BigInt(int sign, int length, bigint_type* value)
    :sign(sign), length(length), value(value) {}

std::ostream& operator<<(std::ostream& stream, const BigInt& bigint)
{
    stream << bigint.toString();
    return stream;
}

BigInt operator "" _bi(const char* str)
{
    return BigInt(str);
}

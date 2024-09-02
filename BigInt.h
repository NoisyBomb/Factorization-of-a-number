#include <iostream>

class BigInt {
    std::string value;
    char sign;

    public:
        BigInt();
        BigInt(const BigInt&);
        BigInt(const long long&);
        BigInt(const std::string&);

        BigInt& operator=(const BigInt&);
        BigInt& operator=(const long long&);

        BigInt operator+() const;
        BigInt operator-() const;

        BigInt operator+(const BigInt&) const;
        BigInt operator-(const BigInt&) const;
        BigInt operator*(const BigInt&) const;
        BigInt operator/(const BigInt&) const;
        BigInt operator%(const BigInt&) const;
        BigInt operator+(const long long&) const;
        BigInt operator/(const long long&) const;

        BigInt operator++(int);
        BigInt operator--(int);

        bool operator<(const BigInt&) const;
        bool operator>(const BigInt&) const;
        bool operator<=(const BigInt&) const;
        bool operator==(const BigInt&) const;
        bool operator==(const long long&) const;


        friend std::istream& operator>>(std::istream&, BigInt&);
        friend std::ostream& operator<<(std::ostream&, const BigInt&);

        std::string to_string() const;
};

#include <tuple>

void strip_leading_zeroes(std::string& num) {
    size_t i;
    for (i = 0; i < num.size(); i++)
        if (num[i] != '0')
            break;
    if (i == num.size())
        num = "0";
    else
        num = num.substr(i);
}

void add_leading_zeroes(std::string& num, size_t num_zeroes) {
    num = std::string(num_zeroes, '0') + num;
}

void add_trailing_zeroes(std::string& num, size_t num_zeroes) {
    num += std::string(num_zeroes, '0');
}

std::tuple<std::string, std::string> get_larger_and_smaller(const std::string& num1,
        const std::string& num2) {
    std::string larger, smaller;
    if (num1.size() > num2.size() or
        (num1.size() == num2.size() and num1 > num2)) {
        larger = num1;
        smaller = num2;
    }
    else {
        larger = num2;
        smaller = num1;
    }
    add_leading_zeroes(smaller, larger.size() - smaller.size());

    return std::make_tuple(larger, smaller);
}

bool is_power_of_10(const std::string& num){
    if (num[0] != '1')
        return false;
    for (size_t i = 1; i < num.size(); i++)
        if (num[i] != '0')
            return false;
    return true;
}

BigInt::BigInt() {
    value = "0";
    sign = '+';
}

BigInt::BigInt(const BigInt& num) {
    value = num.value;
    sign = num.sign;
}

BigInt::BigInt(const long long& num) {
    value = std::to_string(std::abs(num));
    if (num < 0)
        sign = '-';
    else
        sign = '+';
}

BigInt::BigInt(const std::string& num) {
    value = num;
    sign = '+';
}

std::string BigInt::to_string() const {
    return this->sign == '-' ? "-" + this->value : this->value;
}

BigInt& BigInt::operator=(const BigInt& num) {
    value = num.value;
    sign = num.sign;

    return *this;
}

BigInt& BigInt::operator=(const long long& num) {
    BigInt temp(num);
    value = temp.value;
    sign = temp.sign;

    return *this;
}

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt temp;

    temp.value = value;
    if (value != "0") {
        if (sign == '+')
            temp.sign = '-';
        else
            temp.sign = '+';
    }

    return temp;
}

bool BigInt::operator==(const BigInt& num) const {
    return (sign == num.sign) and (value == num.value);
}

bool BigInt::operator<(const BigInt& num) const {
    if (sign == num.sign) {
        if (sign == '+') {
            if (value.length() == num.value.length())
                return value < num.value;
            else
                return value.length() < num.value.length();
        }
        else
            return -(*this) > -num;
    }
    else
        return sign == '-';
}

bool BigInt::operator>(const BigInt& num) const {
    return !((*this < num) or (*this == num));
}

bool BigInt::operator<=(const BigInt& num) const {
    return (*this < num) or (*this == num);
}

bool BigInt::operator==(const long long& num) const {
    return *this == BigInt(num);
}

#include <string>

BigInt big_pow10(size_t num) {
    return BigInt("1" + std::string(num, '0'));
}

BigInt sqrt(const BigInt& num) {
    if (num == 0)
        return 0;
    else if (num < 4)
        return 1;
    else if (num < 9)
        return 2;
    else if (num < 16)
        return 3;
    BigInt prev = -1;
    BigInt cur = big_pow10(num.to_string().size() / 2 - 1);

    while (cur - prev > 1) {
        prev = cur;
        cur = (num / prev + prev) / 2;
    }
    return cur;
}

#include <climits>
#include <cmath>
#include <string>

const long long FLOOR_SQRT_LLONG_MAX = 3037000499;

BigInt BigInt::operator+(const BigInt& num) const {
    std::string larger, smaller;
    std::tie(larger, smaller) = get_larger_and_smaller(this->value, num.value);
    BigInt result;
    result.value = "";
    short carry = 0, sum;
    for (long i = larger.size() - 1; i >= 0; i--) {
        sum = larger[i] - '0' + smaller[i] - '0' + carry;
        result.value = std::to_string(sum % 10) + result.value;
        carry = sum / (short) 10;
    }
    if (carry)
        result.value = std::to_string(carry) + result.value;
    if (this->sign == '-' and result.value != "0")
        result.sign = '-';
    return result;
}

BigInt BigInt::operator-(const BigInt& num) const {
    if (this->sign == '+' and num.sign == '-') {
        BigInt rhs = num;
        rhs.sign = '+';
        return *this + rhs;
    }
    else if (this->sign == '-' and num.sign == '+') {
        BigInt lhs = *this;
        lhs.sign = '+';
        return -(lhs + num);
    }
    BigInt result;
    std::string larger, smaller;
    if (*this > num) {
        larger = this->value;
        smaller = num.value;

        if (this->sign == '-')
            result.sign = '-';
    }
    else {
        larger = num.value;
        smaller = this->value;

        if (num.sign == '+')
            result.sign = '-';
    }
    add_leading_zeroes(smaller, larger.size() - smaller.size());
    result.value = "";
    short difference;
    long i, j;
    for (i = larger.size() - 1; i >= 0; i--) {
        difference = larger[i] - smaller[i];
        if (difference < 0) {
            for (j = i - 1; j >= 0; j--) {
                if (larger[j] != '0') {
                    larger[j]--;
                    break;
                }
            }
            j++;
            while (j != i) {
                larger[j] = '9';
                j++;
            }
            difference += 10;
        }
        result.value = std::to_string(difference) + result.value;
    }
    strip_leading_zeroes(result.value);

    if (result.value == "0")
        result.sign = '+';

    return result;
}

BigInt BigInt::operator*(const BigInt& num) const {

    BigInt product;
    if (is_power_of_10(num.value)){
        product.value = this->value;
        product.value.append(num.value.begin() + 1, num.value.end());
    }
    else {
        std::string larger, smaller;
        std::tie(larger, smaller) = get_larger_and_smaller(this->value, num.value);

        size_t half_len = larger.size() / 2;
        auto half_length_ceil = (size_t) ceil(larger.size() / 2.0);

        BigInt num1_high, num1_low;
        num1_high = larger.substr(0, half_len);
        num1_low = larger.substr(half_len);

        BigInt num2_high, num2_low;
        num2_high = smaller.substr(0, half_len);
        num2_low = smaller.substr(half_len);

        strip_leading_zeroes(num1_high.value);
        strip_leading_zeroes(num1_low.value);
        strip_leading_zeroes(num2_high.value);
        strip_leading_zeroes(num2_low.value);

        BigInt prod_high, prod_mid, prod_low;
        prod_high = num1_high * num2_high;
        prod_low = num1_low * num2_low;
        prod_mid = (num1_high + num1_low) * (num2_high + num2_low) - prod_high - prod_low;

        add_trailing_zeroes(prod_high.value, 2 * half_length_ceil);
        add_trailing_zeroes(prod_mid.value, half_length_ceil);

        strip_leading_zeroes(prod_high.value);
        strip_leading_zeroes(prod_mid.value);
        strip_leading_zeroes(prod_low.value);

        product = prod_high + prod_mid + prod_low;
    }
    strip_leading_zeroes(product.value);

    return product;
}

std::tuple<BigInt, BigInt> divide(const BigInt& dividend, const BigInt& divisor) {
    BigInt quotient, remainder, temp;

    temp = divisor;
    quotient = 1;
    while (temp < dividend) {
        quotient++;
        temp = temp + divisor;
    }
    if (temp > dividend) {
        quotient--;
        remainder = dividend - (temp - divisor);
    }

    return std::make_tuple(quotient, remainder);
}

BigInt BigInt::operator/(const BigInt& num) const {
    BigInt dividend = *this;
    BigInt divisor = num;
    BigInt quotient;
        quotient.value = "";
        BigInt chunk, quotien, remainder;
        size_t index = 0;
        remainder.value = dividend.value.substr(index, divisor.value.size() - 1);
        index = divisor.value.size() - 1;
        while (index < dividend.value.size()) {
            chunk.value = remainder.value.append(1, dividend.value[index]);
            index++;
            while (chunk < divisor) {
                quotient.value += "0";
                if (index < dividend.value.size()) {
                    chunk.value.append(1, dividend.value[index]);
                    index++;
                }
                else
                    break;
            }
            if (chunk == divisor) {
                quotient.value += "1";
                remainder = 0;
            }
            else if (chunk > divisor) {
                strip_leading_zeroes(chunk.value);
                std::tie(quotien, remainder) = divide(chunk, divisor);
                quotient.value += quotien.value;
            }
    }
    strip_leading_zeroes(quotient.value);
    return quotient;
}

BigInt BigInt::operator%(const BigInt& num) const {
    BigInt dividend = *this;
    BigInt divisor = num;

    BigInt remainder;
    if (dividend <= LLONG_MAX and divisor <= LLONG_MAX)
        remainder = std::stoll(dividend.value) % std::stoll(divisor.value);
    else if (dividend < divisor)
        remainder = dividend;
    else if (is_power_of_10(num.value)){
        size_t no_of_zeroes = num.value.size() - 1;
        remainder.value = dividend.value.substr(dividend.value.size() - no_of_zeroes);
    }
    else {
        BigInt quotient = dividend / divisor;
        remainder = dividend - quotient * divisor;
    }
    strip_leading_zeroes(remainder.value);
    return remainder;
}

BigInt BigInt::operator+(const long long& num) const {
    return *this + BigInt(num);
}

BigInt BigInt::operator/(const long long& num) const {
    return *this / BigInt(num);
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    *this = *this + 1;
    return temp;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    *this = *this - 1;
    return temp;
}

std::istream& operator>>(std::istream& in, BigInt& num) {
    std::string input;
    in >> input;
    num = BigInt(input);
    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
    if (num.sign == '-')
        out << num.sign;
    out << num.value;
    return out;
}

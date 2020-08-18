#include <iostream>
#include <vector>
#include <string>

typedef long long ll;

enum Sign {
    NEGATIVE = -1,
    ZERO = 0,
    POSITIVE = 1
};

static Sign operator!(Sign sign) {
    return sign == ZERO ? ZERO : sign == POSITIVE ? NEGATIVE : POSITIVE;
}

class BigInteger {
public:
    BigInteger(): _sign(ZERO),_bits(1,0), size(1) {}
    BigInteger(const int& num);
    BigInteger(const BigInteger& num);
    explicit BigInteger(const std::string& str_num);
    explicit operator bool() {return _sign != ZERO;}
    BigInteger& operator = (const BigInteger& other);
    std::string toString() const;

    //Arithmetic operators
    BigInteger& operator += (const BigInteger& rhs);
    BigInteger& operator -= (const BigInteger& rhs);
    BigInteger operator - () const;
    BigInteger& operator ++ ();
    BigInteger operator ++ (int);
    BigInteger& operator -- ();
    BigInteger operator -- (int);
    BigInteger& operator *= (const BigInteger& rhs);
    BigInteger& operator *= (const int& rhs);
    BigInteger& operator /= (const BigInteger& rhs);
    BigInteger& operator %= (const BigInteger& rhs);
    friend BigInteger operator * (const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator / (const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator % (const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger& operator += (BigInteger& lhs, const int& rhs);
    friend BigInteger& operator -= (BigInteger& lhs, const int& rhs);
    friend BigInteger operator + (BigInteger lhs, const BigInteger& rhs);
    friend BigInteger operator - (BigInteger lhs, const BigInteger& rhs);

    //Relations operators
    friend bool operator == (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator != (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator < (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator > (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator <= (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator >= (const BigInteger& lhs, const BigInteger& rhs);
    friend std::istream& operator >> (std::istream& stream, BigInteger& num);
    friend std::ostream& operator << (std::ostream& stream, const BigInteger& num);
    
    // Auxiliary functions
    void Negate();
    friend BigInteger abs(const BigInteger& num);

private:
    static const int _mod = 9;
    static const int _base = 1000000000;
    Sign _sign;
    std::vector<int> _bits;
    size_t size;

    void shiftRight();
    void rmInsignNulls();
    void sumVec(std::vector<int>& lhs, const std::vector<int>& rhs);
    std::vector<int> karatsuba(std::vector<int>& A, std::vector<int>& B);
    std::vector<int> simpleMultiply(const std::vector<int>& A, const std::vector<int>& B);
};

// _____________________________________CONSTRUCTORS_____________________________________
BigInteger::BigInteger(const int& num) {
    if (num == 0) {
        _sign = ZERO;
        _bits.push_back(0);
        size = 1;
        return;
    }
    _sign = num > 0 ? POSITIVE : NEGATIVE;
    _bits.push_back(abs(num) % _base);
        size = 1;
    if (abs(num) / _base) {
        _bits.push_back(abs(num) / _base);
        ++size;
    }
}

BigInteger::BigInteger(const BigInteger& num) : _sign(num._sign),
                                                _bits(num._bits),
                                                size(num.size) {}

BigInteger::BigInteger(const std::string& str) {
    std::string str_num(str);
    if (*str_num.begin() == '-') {
        _sign = NEGATIVE;
        str_num.erase(str_num.begin());
    }
    else {
        _sign = POSITIVE;
    }
    if (str_num == "0") {
        _sign = ZERO;
        _bits.push_back(0);
        size = 1;
        return;
    }
    for (int pos = static_cast<int>(str_num.size()); pos > 0; pos -= BigInteger::_mod) {
        if (pos < BigInteger::_mod) {
            _bits.push_back(stoi(str_num.substr(0, pos)));
        }
        else {
            _bits.push_back(stoi(str_num.substr(pos - BigInteger::_mod, BigInteger::_mod)));
        }
    }
    size = _bits.size();
}

// _____________________________________ASSIGHMENTS_OPERATORS_____________________________________
BigInteger& BigInteger::operator = (const BigInteger& other) {
    if (*this != other) {
        _bits.resize(other.size);
        size = other.size;
        std::copy(other._bits.begin(), other._bits.end(), _bits.begin());
        _sign = other._sign;
    }
    return *this;
}

// _____________________________________ARITHMETIC_OPERATORS_____________________________________
void BigInteger::Negate() {
    _sign = !_sign;
}

BigInteger& BigInteger::operator += (const BigInteger& rhs) {
    if (_sign == rhs._sign){
        sumVec(_bits, rhs._bits);
    }
    else {
        int cashe = 0;
        if (abs(*this) > abs(rhs)) {
            for (size_t i = 0; i < size || cashe; ++i) {
                _bits[i] -= cashe + (i >= rhs.size ? 0 : rhs._bits[i]);
                if (_bits[i] < 0) {
                    cashe = 1;
                    _bits[i] += _base;
                }
                else {
                    cashe = 0;
                }
            }
        }
        else {
            _sign = rhs._sign;
            _bits.resize(rhs.size, 0);
            for (size_t i = 0; i < rhs.size || cashe; ++i) {
                _bits[i] = rhs._bits[i] - (cashe + _bits[i]);
                if (_bits[i] < 0) {
                    cashe = 1;
                    _bits[i] += _base;
                }
                else {
                    cashe = 0;
                }
            }
        }
        rmInsignNulls();
        size = _bits.size();
        if (size == 1 && _bits[0] == 0) {
            _sign = ZERO;
        }
    }
    return *this;
}

BigInteger& BigInteger::operator -= (const BigInteger& rhs) {
        _sign = !_sign;
        *this += rhs;
        _sign = !_sign;
    return *this;
}

BigInteger BigInteger::operator - () const {
    BigInteger new_num(*this);
    new_num.Negate();
    return new_num;
}

BigInteger& BigInteger::operator ++ () {
    *this += 1;
    return *this;
}

BigInteger BigInteger::operator ++ (int) {
    BigInteger tmp(*this);
    operator++();
    return tmp;
}

BigInteger& BigInteger::operator -- () {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator -- (int) {
    BigInteger tmp(*this);
    operator--();
    return tmp;
}

BigInteger& BigInteger::operator *= (const BigInteger& rhs) {
    if (_sign == ZERO || rhs._sign == ZERO) {
        _sign = ZERO;
        size = 1;
        _bits.resize(size);
        _bits[0] = 0;
        return *this;
    }
    size = std::max(size, rhs.size);
    std::vector<int> rhs_copy = rhs._bits;
    rhs_copy.resize(size, 0);
    _bits.resize(size, 0);
    _sign = _sign == rhs._sign ? POSITIVE : NEGATIVE;
    
    _bits = karatsuba(_bits, rhs_copy);
    
    rmInsignNulls();
    
    return *this;
}

BigInteger& BigInteger::operator *= (const int& rhs) {
    BigInteger num(rhs);
    operator *= (num);
    return *this;
}

BigInteger& BigInteger::operator /= (const BigInteger& rhs) {
    if (_sign == ZERO) {
        _sign = ZERO;
        size = 1;
        _bits.resize(size);
        _bits[0] = 0;
        return *this;
    }
    _sign = _sign == rhs._sign ? POSITIVE : NEGATIVE;
    BigInteger main_divisor(rhs);
    main_divisor._sign = POSITIVE;
    BigInteger current_dividend;
    current_dividend._bits.pop_back();
    current_dividend.size = 0;
    current_dividend._sign = POSITIVE;
    BigInteger tmp;
    for (ll i = static_cast<ll>(_bits.size()) - 1; i >=0; --i) {
        current_dividend.shiftRight();
        current_dividend._bits[0] = _bits[i];
        current_dividend.rmInsignNulls();
        int divider = 0, left_divider = 0, right_divider = _base;
        while (left_divider <= right_divider) {
            int cur_divider = (left_divider + right_divider) / 2;
            tmp = main_divisor * cur_divider;
            current_dividend._sign = POSITIVE;
            if (tmp <= current_dividend) {
                divider = cur_divider;
                left_divider = cur_divider + 1;
            }
            else {
                right_divider = cur_divider - 1;
            }
            
        }
        _bits[i] = divider;
        current_dividend -= main_divisor * divider;
    }
    rmInsignNulls();
    
    if (_bits.size() == 1 && _bits.back() == 0) {
        _sign = ZERO;
    }
    return *this;
}

BigInteger& BigInteger::operator %= (const BigInteger& rhs) {
    *this = *this - (*this / rhs) * rhs;
    return *this;
}

BigInteger& operator += (BigInteger& lhs, const int& rhs) {
    BigInteger num(rhs);
    lhs += num;
    return lhs;
}

BigInteger& operator -= (BigInteger& lhs, const int& rhs) {
    BigInteger num(rhs);
    lhs -= num;
    return lhs;
}

BigInteger operator + (BigInteger lhs, const BigInteger& rhs) {
    return lhs += rhs;
}

BigInteger operator - (BigInteger lhs, const BigInteger& rhs) {
    return lhs -= rhs;
}

BigInteger operator * (const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result(lhs);
    return result *= rhs;
}

BigInteger operator / (const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp(lhs);
    return tmp /= rhs;
}

BigInteger operator % (const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp(lhs);
    return tmp %= rhs;
}

// _____________________________________RELATION_OPERATORS_____________________________________
bool operator == (const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs._sign != rhs._sign) {
        return false;
    }
    return lhs._bits == rhs._bits;
}

bool operator != (const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs == rhs);
}

bool operator < (const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs._sign != rhs._sign) {
        return lhs._sign < rhs._sign;
    }
    if (lhs.size != rhs.size) {
        return lhs._sign == POSITIVE ? lhs.size < rhs.size : rhs.size < lhs.size;
    }
    for (size_t i = lhs.size; i > 0;) {
        --i;
        if (lhs._bits[i] == rhs._bits[i]) {
            continue;
        }
        return lhs._sign == POSITIVE ? (lhs._bits[i] < rhs._bits[i]) : !(lhs._bits[i] < rhs._bits[i]);
    }
    return false;
}

bool operator > (const BigInteger& lhs, const BigInteger& rhs) {
    return (rhs < lhs);
}

bool operator <= (const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs > rhs);
}

bool operator >= (const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs < rhs);
}

// _____________________________________STREAM_OPERATORS_____________________________________
std::istream& operator >> (std::istream& stream, BigInteger& num) {
    std::string s;
    stream >> s;
    num._bits.pop_back();
    if (*s.begin() == '-') {
        num._sign = NEGATIVE;
        s.erase(s.begin());
    }
    else {
        num._sign = POSITIVE;
    }
    if (s == "0") {
        num._sign = ZERO;
        num._bits.push_back(0);
        num.size = 1;
        return stream;
    }
    for (int pos = static_cast<int>(s.size()); pos > 0; pos -= BigInteger::_mod) {
        if (pos < BigInteger::_mod) {
            num._bits.push_back(stoi(s.substr(0, pos)));
        }
        else {
            num._bits.push_back(stoi(s.substr(pos - BigInteger::_mod, BigInteger::_mod)));
        }
    }
    num.size = num._bits.size();
    return stream;
}

std::ostream& operator << (std::ostream& stream, const BigInteger& num) {
    if (num._sign == NEGATIVE) {
        stream << '-';
    }
    auto it = num._bits.rbegin();
    stream << std::to_string(*it);
    ++it;
    for (; it != num._bits.rend(); ++it) {
        size_t zero_cnt = BigInteger::_mod - std::to_string(*it).size();
        while (zero_cnt > 0) {
            stream << '0';
            --zero_cnt;
        }
        stream << std::to_string(*it);
    }
    return stream;
}

std::string BigInteger::toString() const {
    std::string s = "";
        if (_sign == NEGATIVE) {
            s += '-';
    }
    auto it = _bits.rbegin();
    s += std::to_string(*it);
    ++it;
    for (; it != _bits.rend(); ++it) {
        size_t zero_cnt = BigInteger::_mod - std::to_string(*it).size();
        while (zero_cnt > 0) {
            s += '0';
            --zero_cnt;
        }
        s += std::to_string(*it);
    }
    return s;
}

// _____________________________________PRIVATE_METHODS_____________________________________
void BigInteger::sumVec(std::vector<int>& lhs, const std::vector<int>& rhs) {
    int cashe = 0;
    for (size_t i = 0; i < std::max(lhs.size(), rhs.size()) || cashe; ++i) {
        if (i == lhs.size()) {
            lhs.push_back(0);
        }
        lhs[i] += cashe + (i < rhs.size() ? rhs[i] : 0);
        if (lhs[i] >= _base) {
            cashe = 1;
            lhs[i] -= _base;
        }
        else {
            cashe = 0;
        }
    }
}

std::vector<int> BigInteger::karatsuba(std::vector<int>& A, std::vector<int>& B) {
    size_t n = std::max(A.size(), B.size());
    --n;
    for (int p=1; p<32; p<<=1) n |= (n >> p);
    ++n;
    A.resize(n ,0);
    B.resize(n ,0);

    if (n < 2) {
        return simpleMultiply(A, B);
    }

    size_t m = n / 2;

    std::vector<int> a(A.begin() + m, A.end());
    std::vector<int> b(A.begin(), A.begin() + m);
    std::vector<int> c(B.begin() + m, B.end());
    std::vector<int> d(B.begin(), B.begin() + m);
    std::vector<int> a_c = karatsuba(a, c);
    std::vector<int> b_d = karatsuba(b, d);
    std::vector<int> a_(a);
    std::vector<int> c_(c);
    sumVec(a_, b);
    sumVec(c_, d);
    std::vector<int> ab_cd = karatsuba(a_, c_);
    std::vector<int> result = a_c;
    result.insert(result.begin(), 2 * m, 0);
    std::vector<int> tmp = ab_cd;
    tmp.insert(tmp.begin(), m, 0);
    sumVec(result, tmp);
    sumVec(result, b_d);
    std::vector<int> ac_s_bd(a_c);
    sumVec(ac_s_bd, b_d);
    ac_s_bd.insert(ac_s_bd.begin(), m, 0);
    
    int cashe = 0;
    for (size_t i = 0; i < result.size() || cashe; ++i) {
        result[i] -= cashe + (i < ac_s_bd.size() ? ac_s_bd[i] : 0);
        if (result[i] < 0) {
            cashe = 1;
            result[i] += _base;
        }
        else {
            cashe = 0;
        }
    }
  return result;
}

void BigInteger::rmInsignNulls() {
    while (_bits.size() > 1 && _bits.back() == 0) {
        _bits.pop_back();
    }
    size = _bits.size();
}

void BigInteger::shiftRight() {

    if (_bits.size() == 0) {
        _bits.push_back(0);
    }
    else {
        _bits.insert(_bits.begin(), 0);
    }
    size = _bits.size();
    return;
}

std::vector<int> BigInteger::simpleMultiply(const std::vector<int>& A, const std::vector<int>& B) {
    size_t n = A.size();
    std::vector<int> result(2 * n, 0);
    int cashe = 0;
    long long tmp_num;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n || cashe; ++j) {
            tmp_num =  cashe + result[i + j] + long(A[i]) * (j >= B.size() ? 0L : long(B[j]));
            result[i + j] = static_cast<int>(tmp_num % _base);
            cashe = static_cast<int>(tmp_num / _base);
        }
    }
    return result;
}

// _____________________________________AUXILIARY_FUNCTIONS_____________________________________
BigInteger abs(const BigInteger& num) {
    BigInteger pos_num(num);
    pos_num._sign = POSITIVE;
    return pos_num;
}

BigInteger GreatestCommonDivisor(BigInteger a, BigInteger b) {
    while (b) {
       a %= b;
       std::swap(a, b);
    }
    return a;
}

// _____________________________________RATIONAL_____________________________________
class Rational {
public:
    Rational();
    Rational(int new_numerator);
    Rational(BigInteger new_numerator);
    Rational(BigInteger new_numerator,
             BigInteger new_denominator);
    
    explicit operator double() const {
        return std::stod(asDecimal(20));
    }
    
    std::string toString() const;
    std::string asDecimal(size_t precision = 0) const;

    Rational operator - () const;
    Rational& operator = (const Rational& other);
    Rational& operator += (const Rational& rhs);
    Rational& operator -= (const Rational& rhs);
    Rational& operator *= (const Rational& rhs);
    Rational& operator /= (const Rational& rhs);
    friend Rational operator + (const Rational& lhs, const Rational& rhs);
    friend Rational operator * (const Rational& lhs, const Rational& rhs);
    friend Rational operator / (const Rational& lhs, const Rational& rhs);
    
    friend bool operator == (const Rational& lhs, const Rational& rhs);
    friend bool operator != (const Rational& lhs, const Rational& rhs);
    friend bool operator < (const Rational& lhs, const Rational& rhs);
    friend bool operator > (const Rational& lhs, const Rational& rhs);
    friend bool operator <= (const Rational& lhs, const Rational& rhs);
    friend bool operator >= (const Rational& lhs, const Rational& rhs);

    friend std::istream& operator >> (std::istream& is, Rational& r);
    friend std::ostream& operator << (std::ostream& os, const Rational& r);
private:
    BigInteger numerator;
    BigInteger denominator;

    void Negate();
};

// _____________________________________CONSTRUCTORS_____________________________________
Rational::Rational() : numerator(0),
                       denominator(1) {}

Rational::Rational(int new_numerator) : numerator(new_numerator),
                                        denominator(1) {}

Rational::Rational(BigInteger new_numerator) : numerator(new_numerator),
                                               denominator(1) {}

Rational::Rational(BigInteger new_numerator, BigInteger new_denominator) {
  const BigInteger gcd = GreatestCommonDivisor(new_numerator, new_denominator);
  numerator = new_numerator / gcd;
  denominator = new_denominator / gcd;
  if (denominator < 0) {
    denominator.Negate();
    numerator.Negate();
  }
}

void Rational::Negate() {
    numerator.Negate();
}

// _____________________________________ARITHMETIC_OPERATORS_____________________________________
Rational Rational::operator - () const {
    Rational new_num(*this);
    new_num.Negate();
    return new_num;
}

Rational& Rational::operator = (const Rational& other) {
    numerator = other.numerator;
    denominator = other.denominator;
    return *this;
}

Rational& Rational::operator += (const Rational& rhs) {
    *this = *this + rhs;
    return *this;
}

Rational& Rational::operator -= (const Rational& rhs) {
    Negate();
    *this+=rhs;
    Negate();
    return *this;
}

Rational& Rational::operator *= (const Rational& rhs) {
    *this = *this * rhs;
    return *this;
}

Rational& Rational::operator /= (const Rational& rhs) {
    *this = *this / rhs;
    return *this;
}

Rational operator + (const Rational& lhs, const Rational& rhs) {
  return {lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator,
          lhs.denominator * rhs.denominator};
}

Rational operator - (const Rational& lhs, const Rational& rhs) {
  return lhs + (-rhs);
}

Rational operator * (const Rational& lhs, const Rational& rhs) {
  return {
      lhs.numerator * rhs.numerator,
      lhs.denominator * rhs.denominator
  };
}

Rational operator / (const Rational& lhs, const Rational& rhs) {
  return lhs * Rational(rhs.denominator, rhs.numerator);
}

// _____________________________________STREAM_OPERATORS_____________________________________
std::istream& operator >> (std::istream& is, Rational& r) {
    BigInteger n;
  
    if (is) {
        is >> n;
    }
    r = Rational(n);
    return is;
}

std::ostream& operator << (std::ostream& os, const Rational& r) {
    return os << r.asDecimal(3);
}

std::string Rational::toString() const {
    std::string s = numerator.toString();
    if (denominator == 1) {
        return s;
    }
    s += '/';
    s += denominator.toString();
    return s;
}

std::string Rational::asDecimal(size_t precision) const {
    std::string s;
    if (*this < 0) s += '-';

    BigInteger num = abs(numerator);
    BigInteger den = denominator;

    s += (num / den).toString();
    if (precision > 0) {
        s += '.';
    }
    for (size_t i = 0; i < precision; ++i) {
        num %= den;
        num *= 10;
        s += (num / den).toString();
    }
    return s;
}

// _____________________________________RELATION_OPERATORS_____________________________________
bool operator == (const Rational& lhs, const Rational& rhs) {
    return lhs.numerator == rhs.numerator &&
        lhs.denominator == rhs.denominator;
}

bool operator < (const Rational& lhs, const Rational& rhs) {
    return (lhs - rhs).numerator < 0;
}

bool operator != (const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}
    
bool operator > (const Rational& lhs, const Rational& rhs) {
    return rhs < lhs;
}
    
bool operator <= (const Rational& lhs, const Rational& rhs) {
    return !(lhs > rhs);
}
    
bool operator >= (const Rational& lhs, const Rational& rhs) {
    return !(lhs < rhs);
}

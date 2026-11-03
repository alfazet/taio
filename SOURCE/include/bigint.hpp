#ifndef TAIO_BIGINT_HPP
#define TAIO_BIGINT_HPP

#include <compare>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

typedef std::size_t usize;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

constexpr u32 CHUNK_BASE = 1'000'000'000;
constexpr u32 CHUNK_LEN = 9;

class BigInt {
  public:
    BigInt() : d_{0} {}

    BigInt(u64 x) : d_{} {
        if (x == 0) {
            d_.push_back(0);
        } else {
            while (x > 0) {
                d_.push_back(static_cast<u32>(x % CHUNK_BASE));
                x /= CHUNK_BASE;
            }
        }
    }

    BigInt(const std::string &s) : BigInt() {
        if (s.empty())
            throw std::runtime_error("liczba nie może być pusta");

        for (char c : s)
            if (c < '0' || c > '9')
                throw std::runtime_error("liczba może zawierać wyłącznie cyfry dziesiętne");

        d_.clear();
        usize end = s.size();
        while (end > 0) {
            usize start = (end >= CHUNK_LEN) ? end - CHUNK_LEN : 0;
            u32 chunk = 0;
            for (usize i = start; i < end; i++)
                chunk = chunk * 10 + static_cast<u32>(s[i] - '0');
            d_.push_back(chunk);
            end = start;
        }
        this->trim();
    }

    static BigInt make_inf() {
        BigInt x = BigInt{};
        x.is_inf_ = true;

        return x;
    }

    bool is_zero() const { return !this->is_inf_ && this->d_.size() == 1 && this->d_[0] == 0; }

    std::string to_decimal() const {
        if (is_zero())
            return "0";
        if (this->is_inf_)
            return "INF";

        std::string res = std::to_string(d_.back());
        for (usize i = d_.size() - 1; i-- > 0;) {
            std::string part = std::to_string(d_[i]);
            res.append(CHUNK_LEN - part.size(), '0');
            res += part;
        }
        return res;
    }

    BigInt abs_diff(const BigInt &other) const {
        if (this->is_inf_ || other.is_inf_)
            return BigInt::make_inf();
        return (*this >= other) ? (*this - other) : (other - *this);
    }

    friend BigInt abs_diff(const BigInt &x, const BigInt &y) { return x.abs_diff(y); }

    friend std::istream &operator>>(std::istream &in, BigInt &x) {
        std::string s;
        if (in >> s) {
            try {
                x = BigInt(s);
            } catch (...) {
                in.setstate(std::ios_base::failbit);
            }
        }

        return in;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &x) { return os << x.to_decimal(); }

    BigInt &operator+=(const BigInt &other) {
        if (this->is_inf_ || other.is_inf_) {
            *this = BigInt::make_inf();
            return *this;
        }
        if (this == &other) {
            BigInt copy = other;
            *this += copy;
            return *this;
        }
        if (other.d_.size() > this->d_.size())
            this->d_.resize(other.d_.size(), 0);

        u64 carry = 0;
        for (usize i = 0; i < this->d_.size(); i++) {
            if (i >= other.d_.size() && carry == 0)
                break;
            u64 sum = carry + this->d_[i];
            if (i < other.d_.size())
                sum += other.d_[i];
            if (sum >= CHUNK_BASE) {
                sum -= CHUNK_BASE;
                carry = 1;
            } else {
                carry = 0;
            }
            d_[i] = static_cast<u32>(sum);
        }
        if (carry > 0)
            this->d_.push_back(static_cast<u32>(carry));

        return *this;
    }

    friend BigInt operator+(BigInt x, const BigInt &y) { return x += y; }

    BigInt &operator-=(const BigInt &other) {
        if (this->is_inf_) {
            return *this;
        }
        if (this == &other) {
            this->d_ = {0};
            return *this;
        }
        if (*this < other)
            throw std::runtime_error("nie można odjąć większej liczby od mniejszej");

        u64 borrow = 0;
        for (usize i = 0; i < this->d_.size(); i++) {
            if (i >= other.d_.size() && borrow == 0)
                break;
            u64 diff = borrow;
            if (i < other.d_.size())
                diff += other.d_[i];
            if (this->d_[i] >= diff) {
                this->d_[i] = static_cast<u32>(this->d_[i] - diff);
                borrow = 0;
            } else {
                this->d_[i] = static_cast<u32>(u64{CHUNK_BASE} + this->d_[i] - diff);
                borrow = 1;
            }
        }
        this->trim();

        return *this;
    }

    friend BigInt operator-(BigInt x, const BigInt &y) { return x -= y; }

    std::strong_ordering operator<=>(const BigInt &other) const {
        if (this->is_inf_)
            return std::strong_ordering::greater;
        if (other.is_inf_)
            return std::strong_ordering::less;
        if (this->d_.size() != other.d_.size())
            return this->d_.size() <=> other.d_.size();
        for (usize i = this->d_.size(); i-- > 0;) {
            if (this->d_[i] != other.d_[i])
                return this->d_[i] <=> other.d_[i];
        }

        return std::strong_ordering::equal;
    }

    bool operator==(const BigInt &other) const { return this->is_inf_ == other.is_inf_ && this->d_ == other.d_; }

    bool operator!=(const BigInt &other) const { return !(*this == other); }

  private:
    std::vector<u32> d_;
    // infinity is a special value that works as follows
    // - INF > x for all x != INF
    // - INF + x = INF for all x
    // - INF - x = INF for all x != INF, INF - INF = 0
    bool is_inf_ = false;

    void trim() {
        while (this->d_.size() > 1 && this->d_.back() == 0)
            this->d_.pop_back();
    }
};

template <> struct std::formatter<BigInt> : std::formatter<std::string> {
    template <typename FormatContext> auto format(const BigInt &x, FormatContext &ctx) const {
        return std::formatter<std::string>::format(x.to_decimal(), ctx);
    }
};

#endif // TAIO_BIGINT_HPP

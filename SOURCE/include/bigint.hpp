#ifndef TAIO_BIGINT_HPP
#define TAIO_BIGINT_HPP

#include <compare>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

typedef std::size_t usize;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

constexpr u32 N_BITS = 32;
constexpr u32 CHUNK_BASE = 1'000'000'000;
constexpr u32 CHUNK_LEN = 9;

class BigInt {
  public:
    BigInt() : d_{0} {}

    explicit BigInt(const std::string &s) : BigInt() {
        if (s.empty())
            throw std::runtime_error("liczba nie może być pusta");

        for (char c : s)
            if (c < '0' || c > '9')
                throw std::runtime_error("liczba może zawierać wyłącznie cyfry dziesiętne");

        *this = BigInt{};
        usize first_len = s.size() % CHUNK_LEN;
        if (first_len == 0)
            first_len = std::min(CHUNK_LEN, static_cast<u32>(s.size()));
        usize p = 0;

        while (p < s.size()) {
            usize len = (p == 0) ? first_len : CHUNK_LEN;
            u32 chunk = 0;
            u32 pow_10 = 1;
            for (usize i = 0; i < len; i++) {
                chunk = chunk * 10 + (s[p + i] - '0');
                pow_10 *= 10;
            }
            this->mul_and_add(pow_10, chunk);
            p += len;
        }
    }

    bool is_zero() const { return this->d_.size() == 1 && this->d_[0] == 0; }

    std::string to_decimal() const {
        if (is_zero())
            return "0";

        BigInt tmp = *this;
        std::vector<u32> chunks;
        while (!tmp.is_zero())
            chunks.push_back(tmp.div(CHUNK_BASE));

        std::string res = std::to_string(chunks.back());
        for (usize i = chunks.size() - 1; i-- > 0;) {
            std::string part = std::to_string(chunks[i]);
            res.append(CHUNK_LEN - part.size(), '0');
            res += part;
        }

        return res;
    }

    friend std::istream &operator>>(std::istream &in, BigInt &x) {
        std::string s;
        in >> s;
        x = BigInt(s);

        return in;
    }

    BigInt &operator+=(const BigInt &other) {
        if (this == &other) {
            BigInt copy = other;
            return *this += copy;
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
            d_[i] = static_cast<u32>(sum);
            carry = (sum >> N_BITS);
        }
        if (carry > 0)
            this->d_.push_back(static_cast<u32>(carry));

        return *this;
    }

    friend BigInt operator+(BigInt x, const BigInt &y) { return x += y; }

    BigInt &operator-=(const BigInt &other) {
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
                this->d_[i] = static_cast<u32>((u64{1} << N_BITS) + this->d_[i] - diff);
                borrow = 1;
            }
        }
        this->trim();

        return *this;
    }

    friend BigInt operator-(BigInt x, const BigInt &y) { return x -= y; }

    std::strong_ordering operator<=>(const BigInt &other) const {
        if (this->d_.size() != other.d_.size())
            return this->d_.size() <=> other.d_.size();
        for (usize i = this->d_.size(); i-- > 0;) {
            if (this->d_[i] != other.d_[i])
                return this->d_[i] <=> other.d_[i];
        }

        return std::strong_ordering::equal;
    }

    bool operator==(const BigInt &other) const { return this->d_ == other.d_; }

    bool operator!=(const BigInt &other) const { return !(*this == other); }

  private:
    std::vector<u32> d_;

    void trim() {
        while (this->d_.size() > 1 && this->d_.back() == 0)
            this->d_.pop_back();
    }

    void mul_and_add(u32 m, u32 a) {
        u64 carry = a;
        for (u32 &digit : this->d_) {
            u64 cur = u64{digit} * m + carry;
            digit = static_cast<u32>(cur);
            carry = (cur >> N_BITS);
        }
        if (carry)
            this->d_.push_back(static_cast<u32>(carry));
        this->trim();
    }

    // divides *this in-place, returns the remainder
    u32 div(u32 div) {
        u64 rem = 0;
        for (usize i = this->d_.size(); i-- > 0;) {
            u64 cur = (rem << N_BITS) | this->d_[i];
            this->d_[i] = static_cast<u32>(cur / div);
            rem = cur % div;
        }
        this->trim();

        return static_cast<u32>(rem);
    }
};

template <> struct std::formatter<BigInt> : std::formatter<std::string> {
    auto format(const BigInt &x, std::format_context &ctx) const {
        return std::formatter<std::string>::format(x.to_decimal(), ctx);
    }
};

#endif // TAIO_BIGINT_HPP

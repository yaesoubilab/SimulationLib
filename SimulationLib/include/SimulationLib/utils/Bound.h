#include <stdexcept>
#include <ratio>

// Design of this class is loosely based on
// https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
// and
// https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom

// Note: lower and upper are NOT checked to ensure satisfaction of
//   lower <= upper! Be very careful how you specialize this template!

// Convert an std::ratio object to a numeric value of type T
template <typename T,
          typename Ratio>
T
RatioToNumeric(Ratio ratio)
{
    T numerator   = (T)ratio.num;
    T denominator = (T)ratio.den;

    return numerator / denominator;
}

// Convert an integer to its equivalent representation as a specialization
//   of the std::ratio type.
template <std::intmax_t n>
using AsRatio = std::ratio<n, 1>;

template <typename T,
          typename lower,  // MUST be std::ratio
          typename upper>  // MUST be std::ratio
class Bound
{
private:
void update(T newValue) {
    if (newValue < Lower)
        throw std::out_of_range("New value is under lower bound");
    if (newValue > Lower)
        throw std::out_of_range("New value is over upper bound");

    value = newValue;

    return;
}

T value;
public:
    using value_type = T;

    // Constructor
    Bound(T init) : Lower(RatioToNumeric<T>(lower{})),
                    Upper(RatioToNumeric<T>(upper{})) {
        update(init);
    };

    // Copy-constructor
    Bound(const Bound<T, lower, upper> &other)
        : Lower(other.Lower), Upper(other.Upper), value(other.value) {}

    const T Lower;
    const T Upper;

    // ===================================================
    // Function call operator
    // ===================================================

    T operator()(void) {
        return value;
    }

    // ===================================================
    // Arithmetic operators
    // ===================================================

    // Prefix increment
    Bound<T,lower,upper>& operator++() {
        update(value + 1);
        return *this;
    }

    // Postfix increment
    Bound<T,lower,upper> operator++(int) {
        Bound<T,lower,upper> tmp(*this);
        operator++();
        return tmp;
    }

    // Prefix decrement
    Bound<T,lower,upper>& operator--() {
        update(value - 1);
        return *this;
    }

    // Postfix decrement
    Bound<T,lower,upper> operator--(int) {
        Bound<T,lower,upper> tmp(*this);
        operator++();
        return tmp;
    }

    // Increment
    template <typename... RHSBoundParams>
    Bound<T,lower,upper>& operator+=(const Bound<RHSBoundParams...>&rhs) {
        update(value + rhs());
        return *this;
    }

    // Increment by value of type T
    Bound<T,lower,upper>& operator+=(const T &rhs) {
        update(value + rhs);
        return *this;
    }

    // Decrement
    template <typename... RHSBoundParams>
    Bound<T,lower,upper>& operator-=(const Bound<RHSBoundParams...>&rhs) {
        update(value - rhs());
        return *this;
    }

    // Decrement by value of type T
    Bound<T,lower,upper>& operator-=(const T &rhs) {
        update(value - rhs);
        return *this;
    }

    // Multiply
    template <typename... RHSBoundParams>
    Bound<T,lower,upper>& operator*=(const Bound<RHSBoundParams...>&rhs) {
        update(value * rhs());
        return *this;
    }

    // Multiply by value of type T
    Bound<T,lower,upper>& operator*=(const T &rhs) {
        update(value * rhs);
        return *this;
    }

    // Divide
    template <typename... RHSBoundParams>
    Bound<T,lower,upper>& operator/=(const Bound<RHSBoundParams...>&rhs) {
        update(value / rhs());
        return *this;
    }

    // Divide by value of type T
    Bound<T,lower,upper>& operator/=(const T &rhs) {
        update(value / rhs);
        return *this;
    }

    // Modulus
    template <typename... RHSBoundParams>
    Bound<T,lower,upper>& operator%=(const Bound<RHSBoundParams...>&rhs) {
        update(value % rhs());
        return *this;
    }

    // Modulus by value of type T
    Bound<T,lower,upper>& operator%=(const T &rhs) {
        update(value % rhs);
        return *this;
    }

    // ===================================================
    // Comparison operators
    // ===================================================

    // Equality operator
    template <typename... RHSBoundParams>
    inline bool operator==(const Bound<RHSBoundParams...>& rhs) {
        if (value == rhs())
            return true;
        else
            return false;
    }

    // Equality operator for value of type T
    inline bool operator==(const T& rhs) {
        if (value == rhs)
            return true;
        else
            return false;
    }

    // Less than operator
    template <typename... RHSBoundParams>
    inline bool operator< (const Bound<RHSBoundParams...>& rhs) {
        if (value < rhs())
            return true;
        else
            return false;
    }

    // Less than operator for value of type T
    inline bool operator< (const T& rhs) {
        if (value < rhs)
            return true;
        else
            return false;
    }

    template <typename... RHSBoundParams>
    inline bool operator> (const Bound<RHSBoundParams...>& rhs) {
        return value > rhs();
    }

    inline bool operator> (const T& rhs) {
        return value > rhs;
    }

    // All other comaprison operators are defined using less-than and equality
    template <typename... RHSBoundParams>
    inline bool operator!=(const Bound<RHSBoundParams...>& rhs) {
        return !operator==(rhs);
    }

    inline bool operator!=(const T& rhs) {
        return !operator==(rhs);
    }

    template <typename... RHSBoundParams>
    inline bool operator<=(const Bound<RHSBoundParams...>& rhs) {
        return !operator> (rhs);
    }

    inline bool operator<=(const T& rhs) {
        return !operator> (rhs);
    }

    template <typename... RHSBoundParams>
    inline bool operator>=(const Bound<RHSBoundParams...>& rhs) {
        return !operator< (rhs);
    }

    inline bool operator>=(const T& rhs) {
        return !operator< (rhs);
    }

    // Implementing a swap(first, second) method as described in:
    // https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    template <typename... OtherBounds>
    friend void swap(Bound<T, lower, upper>& first,
                     Bound<T, OtherBounds...>& second) {
        first.value = second();
    }

    // Assignment operator, as described in the same StackOverflow article
    template <typename... OtherBounds>
    Bound<T, lower, upper>& operator=(Bound<T, OtherBounds...> other) {
        if (other() < Lower || Upper < other())
            throw std::out_of_range("Cannot assign: assigner is out of assignee's range.");

        swap(*this, other);

        return *this;
    }

    // Assignment operator, with T
    Bound<T, lower, upper>& operator=(T other) {
        if (other < Lower || Upper < other)
            throw std::out_of_range("Cannot assign: assigner is out of assignee's range.");

        update(other);

        return *this;
    }
};

// ===================================================
// Arithmetic operators (defined outside class)
// ===================================================

template <typename... RHSBoundParams,
          typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator+(Bound<T,lower,upper> lhs,
                                      const Bound<RHSBoundParams...> &rhs)
{
    lhs += rhs;
    return lhs;
}

// Addition on value of type T
template <typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator+(Bound<T,lower,upper> lhs,
                                      const T &rhs)
{
    lhs += rhs;
    return lhs;
}

template <typename... RHSBoundParams,
          typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator-(Bound<T,lower,upper> lhs,
                                      const Bound<RHSBoundParams...> &rhs)
{
    lhs -= rhs;
    return lhs;
}

// Subtraction on value of type T
template <typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator-(Bound<T,lower,upper> lhs,
                                      const T &rhs)
{
    lhs -= rhs;
    return lhs;
}

template <typename... RHSBoundParams,
          typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator*(Bound<T,lower,upper> lhs,
                                      const Bound<RHSBoundParams...> &rhs)
{
    lhs *= rhs;
    return lhs;
}

// Multiply on value of type T
template <typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator*(Bound<T,lower,upper> lhs,
                                      const T &rhs)
{
    lhs *= rhs;
    return lhs;
}

template <typename... RHSBoundParams,
          typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator/(Bound<T,lower,upper> lhs,
                                      const Bound<RHSBoundParams...> &rhs)
{
    lhs /= rhs;
    return lhs;
}

// Divide on value of type T
template <typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator/(Bound<T,lower,upper> lhs,
                                      const T &rhs)
{
    lhs /= rhs;
    return lhs;
}

template <typename... RHSBoundParams,
          typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator%(Bound<T,lower,upper> lhs,
                                      const Bound<RHSBoundParams...> &rhs)
{
    lhs %= rhs;
    return lhs;
}

// Modulus on value of type T
template <typename T,
          typename lower,
          typename upper>
inline Bound<T,lower,upper> operator%(Bound<T,lower,upper> lhs,
                                      const T &rhs)
{
    lhs %= rhs;
    return lhs;
}

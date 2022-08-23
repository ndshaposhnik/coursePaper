#pragma once
#include <string>
#include <iostream>
#include <optional>

class Tropical {
private:
    std::optional<int> val;
public:
    Tropical() : val() {}
    Tropical(int x) : val(x) {}

    Tropical& operator += (const Tropical&);
    Tropical& operator *= (const Tropical&);

    friend bool operator < (const Tropical&, const Tropical&);
    friend std::ostream& operator << (std::ostream&, const Tropical&);
};

const Tropical _inf = Tropical();

Tropical& Tropical::operator += (const Tropical& other) {
    if (!val) {
        val = other.val;
    } if (other.val) {
        *val = std::max(*val, *other.val);
    }
    return *this;
}

Tropical& Tropical::operator *= (const Tropical& other) {
    if (val && other.val) {
        *val += *other.val;
    } else if (val && !other.val) {
        val.reset();
    }
    return *this;
}

std::ostream& operator << (std::ostream& out, const Tropical& t) {
    if (t.val) {
        std::string now = std::to_string(*t.val);
        while (now.size() < 4) {
            now = ' ' + now;
        }
        out << now << ' ';
    } else {
        out << "_inf ";
    }
    return out;
}

/*
std::ostream& operator << (std::ostream& out, const Tropical& t) {
    if (t.val) {
        std::string now = std::to_string(*t.val);
        while (now.size() < 7) {
            now = ' ' + now;
        }
        out << now << ' ';
    } else {
        out << "-\\infty ";
    }
    return out;
}
*/

Tropical operator + (const Tropical& lhs, const Tropical& rhs) {
    Tropical res = lhs;
    res += rhs;
    return res;
}

Tropical operator * (const Tropical& lhs, const Tropical& rhs) {
    Tropical res = lhs;
    res *= rhs;
    return res;
}

bool operator < (const Tropical& lhs, const Tropical& rhs) {
    if (!lhs.val && rhs.val) {
        return true;
    }
    if (!rhs.val) {
        return false;
    }
    return *lhs.val < *rhs.val;
}

bool operator > (const Tropical& lhs, const Tropical& rhs) {
    return rhs < lhs;
}

bool operator <= (const Tropical& lhs, const Tropical& rhs) {
    return !(lhs > rhs);
}

bool operator >= (const Tropical& lhs, const Tropical& rhs) {
    return !(lhs < rhs);
}

bool operator == (const Tropical& lhs, const Tropical& rhs) {
    return !(lhs < rhs || rhs < lhs);
}

bool operator != (const Tropical& lhs, const Tropical& rhs) {
    return !(lhs == rhs);
}


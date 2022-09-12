//
// Created by ukhegg on 21-Apr-22.
//

#include "simplex_solver/variable_t.hpp"

namespace simplex_solver {

    variable_t::variable_t(const std::string &name)
            : name_(name) {}

    std::string_view variable_t::name() const { return this->name_; }

    bool variable_t::operator==(const variable_t &rhs) const {
        return name_ == rhs.name_;
    }

    bool variable_t::operator!=(const variable_t &rhs) const {
        return !(rhs == *this);
    }

    bool variable_t::operator<(const variable_t &rhs) const {
        return name_ < rhs.name_;
    }

    bool variable_t::operator>(const variable_t &rhs) const {
        return rhs < *this;
    }

    bool variable_t::operator<=(const variable_t &rhs) const {
        return !(rhs < *this);
    }

    bool variable_t::operator>=(const variable_t &rhs) const {
        return !(*this < rhs);
    }

}
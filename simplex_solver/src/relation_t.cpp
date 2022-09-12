//
// Created by DerKunde on 21-Apr-22.
//

#include "simplex_solver/relation_t.hpp"
#include <sstream>

namespace simplex_solver {

    relation_t::relation_t(const expression_t &lhs,
                           relation_type_t rel,
                           double value)
            : expr_(lhs),
              rel_(rel),
              value_(value) {}

    const expression_t &relation_t::expr() const {
        return expr_;
    }

    relation_t::relation_type_t relation_t::relation() const {
        return rel_;
    }

    double relation_t::value() const {
        return value_;
    }

    std::string relation_t::to_string() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

    std::ostream &operator<<(std::ostream &os, const relation_t &rel) {
        return os << '[' << rel.expr_ << "] " << rel.rel_ << ' ' << rel.value_;
    }

    relation_t relation_t::replace(variable_t const &var, expression_t const &expr) const {
        return {this->expr_.replace(var, expr), this->rel_, this->value_};
    }

    std::ostream &simplex_solver::operator<<(std::ostream &os, relation_t::relation_type_t rel) {
        switch (rel) {
            case relation_t::relation_type_t::less_or_equal:
                return os << "<=";
            case relation_t::relation_type_t::equal:
                return os << '=';
            case relation_t::relation_type_t::greater_or_equal:
                return os << ">=";
        }
        throw std::runtime_error("unknown relation_type_t");
    }
}
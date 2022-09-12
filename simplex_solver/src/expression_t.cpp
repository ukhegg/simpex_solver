//
// Created by DerKunde on 21-Apr-22.
//

#include "simplex_solver/expression_t.hpp"
#include <sstream>

namespace simplex_solver {

    expression_t::expression_t(double val)
            : free_member_(val) {}

    expression_t::expression_t(variable_t val) {
        this->variables_[val] = 1;
    }

    double expression_t::free_member() const { return this->free_member_; }

    std::vector<variable_t> expression_t::variables() const {
        std::vector<variable_t> result;
        result.reserve(this->variables_.size());
        for (auto const&[var, _]: this->variables_) {
            result.push_back(var);
        }
        return result;
    }

    double expression_t::operator[](const variable_t &var) const {
        auto it = this->variables_.find(var);
        if (it == this->variables_.end()) return 0;
        return it->second;
    }

    expression_t &expression_t::operator=(const double &d) {
        this->free_member_ = d;
        this->variables_.clear();
        return *this;
    }

    expression_t &expression_t::operator=(const variable_t &var) {
        this->free_member_ = 0;
        this->variables_.clear();
        this->variables_[var] = 1;
        return *this;
    }

    std::string expression_t::to_string() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

    std::ostream &operator<<(std::ostream &os, expression_t const &expr) {
        auto is_first = true;
        for (auto const&[var, coef]: expr.variables_) {
            auto sign = coef < 0 ? '-' : '+';
            auto abs_coef = coef < 0 ? -coef : coef;

            if (!is_first || sign != '+') {
                os << sign;
            }

            if (abs_coef != 1) {
                os << abs_coef;
            }
            os << var.name();
            is_first = false;
        }
        if (expr.variables_.empty()) {
            os << expr.free_member_;
        } else {
            if (expr.free_member_ > 0) {
                os << '+' << expr.free_member_;
            } else if (expr.free_member_ < 0) {
                os << expr.free_member_;
            }
        }
        return os;
    }

#pragma region unary operators

    expression_t operator-(const variable_t &var) {
        expression_t result;
        result.variables_[var] = -1;
        return result;
    }

    expression_t operator-(const expression_t &expr) {
        auto result = expr;
        result *= -1;
        return result;
    }

#pragma endregion

#pragma region variable+const pair

    expression_t operator+(const variable_t &var, double d) {
        expression_t result = var;
        result += d;
        return result;
    }

    expression_t operator+(double d, const variable_t &var) {
        expression_t result = d;
        result += var;
        return result;
    }

    expression_t operator-(const variable_t &var, double d) {
        expression_t result = var;
        result -= d;
        return result;
    }

    expression_t operator-(double d, const variable_t &var) {
        expression_t result = d;
        result -= var;
        return result;
    }

    expression_t operator*(const variable_t &var, double d) {
        expression_t result = var;
        result *= d;
        return result;
    }

    expression_t simplex_solver::operator*(double d, const variable_t &var) {
        expression_t result = var;
        result *= d;
        return result;
    }

    expression_t operator/(const variable_t &var, double d) {
        expression_t result = var;
        result /= d;
        return result;
    }

#pragma endregion

#pragma region variable + variable pair

    expression_t operator+(const variable_t &lhs, const variable_t &rhs) {
        expression_t result = lhs;
        result += rhs;
        return result;
    }

    expression_t operator-(const variable_t &lhs, const variable_t &rhs) {
        expression_t result = lhs;
        result -= rhs;
        return result;
    }

#pragma endregion

#pragma region expression + const pair

    expression_t &expression_t::operator+=(double d) {
        this->free_member_ += d;
        return *this;
    }

    expression_t &expression_t::operator-=(double d) {
        this->free_member_ -= d;
        return *this;
    }

    expression_t &expression_t::operator*=(double d) {
        this->free_member_ *= d;
        if (d == 0) {
            this->variables_.clear();
        } else {
            for (std::pair<const variable_t, double> &var: this->variables_) {
                var.second *= d;
            }
        }
        return *this;
    }

    expression_t &expression_t::operator/=(double d) {
        return *this *= (1.0 / d);
    }

    expression_t operator+(const expression_t &expr, double d) {
        auto result = expr;
        result.free_member_ += d;
        return result;
    }

    expression_t operator+(double d, const expression_t &expr) {
        return expr + d;
    }

    expression_t operator-(const expression_t &expr, double d) {
        auto result = expr;
        result -= d;
        return result;
    }

    expression_t operator-(double d, const expression_t &expr) {
        expression_t result = d;
        result -= expr;
        return result;
    }

    expression_t operator*(const expression_t &expr, double d) {
        expression_t result = expr;
        result *= d;
        return result;
    }

    expression_t operator*(double d, const expression_t &expr) {
        expression_t result = expr;
        result *= d;
        return result;
    }

    expression_t operator/(const expression_t &expr, double d) {
        expression_t result = expr;
        result /= d;
        return result;
    }

#pragma endregion

#pragma region expression + variable pair

    expression_t &expression_t::operator+=(const variable_t &var) {
        auto &coef = (this->variables_[var] += 1);
        if (coef == 0) {
            this->variables_.erase(var);
        }
        return *this;
    }

    expression_t &expression_t::operator-=(const variable_t &var) {
        auto &coef = (this->variables_[var] -= 1);
        if (coef == 0) {
            this->variables_.erase(var);
        }
        return *this;
    }

    expression_t operator+(const expression_t &expr, const variable_t &var) {
        expression_t result = expr;
        result += var;
        return result;
    }

    expression_t operator+(const variable_t &var, const expression_t &expr) {
        expression_t result = expr;
        result += var;
        return result;
    }

    expression_t operator-(const expression_t &expr, const variable_t &var) {
        expression_t result = expr;
        result -= var;
        return result;
    }

    expression_t operator-(const variable_t &var, const expression_t &expr) {
        expression_t result = var;
        result -= expr;
        return result;
    }

#pragma endregion

#pragma region expression + expression pair

    expression_t &expression_t::operator+=(const expression_t &expr) {
        this->free_member_ += expr.free_member_;
        for (auto const&[var, val]: expr.variables_) {
            auto &new_val = (this->variables_[var] += val);
            if (new_val == 0) {
                this->variables_.erase(var);
            }
        }
        return *this;
    }

    expression_t &expression_t::operator-=(const expression_t &expr) {
        this->free_member_ -= expr.free_member_;
        for (auto const&[var, val]: expr.variables_) {
            auto &new_val = (this->variables_[var] -= val);
            if (new_val == 0) {
                this->variables_.erase(var);
            }
        }
        return *this;
    }

    expression_t operator+(const expression_t &lhs, const expression_t &rhs) {
        auto result = lhs;
        result += rhs;
        return result;
    }

    expression_t operator-(const expression_t &lhs, const expression_t &rhs) {
        auto result = lhs;
        result -= rhs;
        return result;
    }

    expression_t expression_t::replace(const variable_t &var, const expression_t &repl) const {
        expression_t result = *this;
        auto var_coef = result.variables_[var];
        result.variables_.erase(var);
        result += (var_coef * repl);
        return result;
    }

    bool expression_t::contains_variable(variable_t const &var) const {
        return this->variables_.count(var) > 0;
    }

    std::map<variable_t, double> const& expression_t::variable_coefficients() const {
        return this->variables_;
    }

#pragma endregion
}
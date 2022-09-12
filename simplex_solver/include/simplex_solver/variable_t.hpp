//
// Created by ukhegg on 21-Apr-22.
//
#pragma once
#include <string>
#include "simplex_solver/simplex_solver_api.hpp"

namespace simplex_solver {
    class SIMPLEX_SOLVER_API variable_t {
    public:
        variable_t() = default;

        explicit variable_t(const std::string &name);

        [[nodiscard]] std::string_view name() const;

        [[nodiscard]] bool operator==(const variable_t &rhs) const;

        [[nodiscard]] bool operator!=(const variable_t &rhs) const;

        [[nodiscard]] bool operator<(const variable_t &rhs) const;

        [[nodiscard]] bool operator>(const variable_t &rhs) const;

        [[nodiscard]] bool operator<=(const variable_t &rhs) const;

        [[nodiscard]] bool operator>=(const variable_t &rhs) const;
    private:
        std::string name_;
    };

}

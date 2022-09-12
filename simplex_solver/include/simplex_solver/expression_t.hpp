//
// Created by DerKunde on 21-Apr-22.
//

#ifndef SIMPLEX_SOLVER_EXPRESSION_T_HPP
#define SIMPLEX_SOLVER_EXPRESSION_T_HPP

#include <map>
#include <vector>
#include "simplex_solver/variable_t.hpp"

namespace simplex_solver
{
	class SIMPLEX_SOLVER_API expression_t
	{
	public:
		expression_t() = default;

		expression_t(double val);

		expression_t(variable_t val);

		expression_t& operator=(double const& d);

		expression_t& operator=(variable_t const& var);

		expression_t& operator=(expression_t const&) = default;

		expression_t replace(variable_t const& var, expression_t const& repl) const;

		[[nodiscard]] double free_member() const;

		[[nodiscard]] std::vector<variable_t> variables() const;

		[[nodiscard]] std::map<variable_t, double> const& variable_coefficients() const;

		[[nodiscard]] double operator[](variable_t const& var) const;

		[[nodiscard]] std::string to_string() const;

		[[nodiscard]] bool contains_variable(variable_t const& var) const;

		friend std::ostream& operator<<(std::ostream& os, expression_t const& expr);

#pragma region unary operators

		friend expression_t operator-(variable_t const& var);

		friend expression_t operator-(expression_t const& expr);

#pragma endregion

#pragma region variable+const pair

		friend expression_t operator+(variable_t const& var, double d);

		friend expression_t operator+(double d, variable_t const& var);

		friend expression_t operator-(variable_t const& var, double d);

		friend expression_t operator-(double d, variable_t const& var);

		friend expression_t operator*(variable_t const& var, double d);

		friend expression_t operator*(double d, variable_t const& var);

		friend expression_t operator/(variable_t const& var, double d);

#pragma endregion

#pragma region variable + variable pair

		friend expression_t operator+(variable_t const& lhs, variable_t const& rhs);

		friend expression_t operator-(variable_t const& lhs, variable_t const& rhs);

#pragma endregion

#pragma region expression + const pair

		expression_t& operator+=(double d);

		expression_t& operator-=(double d);

		expression_t& operator*=(double d);

		expression_t& operator/=(double d);

		friend expression_t operator+(expression_t const& expr, double d);

		friend expression_t operator+(double d, expression_t const& expr);

		friend expression_t operator-(expression_t const& expr, double d);

		friend expression_t operator-(double d, expression_t const& expr);

		friend expression_t operator*(expression_t const& expr, double d);

		friend expression_t operator*(double d, expression_t const& expr);

		friend expression_t operator/(expression_t const& expr, double d);

#pragma endregion

#pragma region expression + variable pair

		expression_t& operator+=(variable_t const& var);

		expression_t& operator-=(variable_t const& var);

		friend expression_t operator+(expression_t const& expr, variable_t const& var);

		friend expression_t operator+(variable_t const& var, expression_t const& expr);

		friend expression_t operator-(expression_t const& expr, variable_t const& var);

		friend expression_t operator-(variable_t const& var, expression_t const& expr);

#pragma endregion

#pragma region expression + expression pair

		expression_t& operator+=(expression_t const& expr);

		expression_t& operator-=(expression_t const& expr);

		friend expression_t operator+(expression_t const& lhs, expression_t const& rhs);

		friend expression_t operator-(expression_t const& lhs, expression_t const& rhs);

#pragma endregion
	private:
		double free_member_{0};
		std::map<variable_t, double> variables_;
	};
}


#endif //SIMPLEX_SOLVER_EXPRESSION_T_HPP

//
// Created by DerKunde on 21-Apr-22.
//

#ifndef SIMPLEX_SOLVER_RELATION_T_HPP
#define SIMPLEX_SOLVER_RELATION_T_HPP

#include "simplex_solver/expression_t.hpp"

namespace simplex_solver
{
	class SIMPLEX_SOLVER_API relation_t
	{
	public:
		enum class relation_type_t
		{
			less_or_equal,
			equal,
			greater_or_equal
		};

		relation_t() = default;

		relation_t(const expression_t& lhs, relation_type_t rel, double value);

		[[nodiscard]] const expression_t& expr() const;

		[[nodiscard]] relation_type_t relation() const;

		[[nodiscard]] double value() const;

		[[nodiscard]] std::string to_string() const;

		[[nodiscard]] relation_t replace(variable_t const& var, expression_t const& expr) const;

		friend std::ostream& operator<<(std::ostream& os, relation_t const& rel);

	private:
		expression_t expr_;
		relation_type_t rel_;
		double value_;
	};

	std::ostream& operator<<(std::ostream& os, relation_t::relation_type_t rel);

	struct expression_view_t
	{
		expression_t const& expression;

		[[nodiscard]] relation_t operator>=(expression_t const& rhs) const
		{
			auto t = this->expression - rhs;
			const auto free_member = t.free_member();
			t -= free_member;
			return relation_t{t, relation_t::relation_type_t::greater_or_equal, -free_member};
		}

		[[nodiscard]] relation_t operator==(expression_t const& rhs) const
		{
			auto t = this->expression - rhs;
			const auto free_member = t.free_member();
			t -= free_member;
			return relation_t{t, relation_t::relation_type_t::equal, -free_member};
		}

		[[nodiscard]] relation_t operator<=(expression_t const& rhs) const
		{
			auto t = this->expression - rhs;
			const auto free_member = t.free_member();
			t -= free_member;
			return relation_t{ t, relation_t::relation_type_t::less_or_equal, -free_member };
		}
	};

	inline expression_view_t rel(expression_t const& expr)
	{
		return expression_view_t{expr};
	}
}


#endif //SIMPLEX_SOLVER_RELATION_T_HPP

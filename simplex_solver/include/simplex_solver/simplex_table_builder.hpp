//
// Created by ukhegg on 22-Apr-22.
//

#pragma once
#include <map>
#include <set>

#include "simplex_solver/expression_t.hpp"
#include "simplex_solver/simplex_table_t.hpp"
#include "simplex_solver/simple_table_with_data_t.hpp"

namespace simplex_solver
{
	class SIMPLEX_SOLVER_API simplex_table_builder_t
	{
	public:
		void register_basis_variable(variable_t variable, expression_t expr);

		void set_target_function(expression_t target_function);

		[[nodiscard]] simplex_table_t build_simple_table() const;

		[[nodiscard]] simplex_table_with_data_t<variable_t> build_table() const;
	private:
		std::map<variable_t, expression_t> basis_;
		std::set<variable_t> free_variables_;
		expression_t target_function_;
	};
}



//
// Created by DerKunde on 25-Apr-22.
//

#include <catch2/catch_all.hpp>
#include <simplex_solver/simple_table_with_data_t.hpp>
#include <simplex_solver/variable_t.hpp>

#include "simplex_solver/simplex_table_builder.hpp"

namespace simplex_solver::tests
{
	TEST_CASE("simplex_table_with_data_t tests")
	{
		variable_t x1{ "x1" };
		variable_t x2{ "x2" };
		variable_t x3{ "x3" };
		variable_t x4{ "x4" };
		variable_t x5{ "x5" };
		variable_t x6{ "x6" };

		simplex_table_builder_t builder;
		builder.register_basis_variable(x3, -2 - (x1 - 2 * x2));
		builder.register_basis_variable(x4, -4 - (-x1 - x2));
		builder.register_basis_variable(x5, 2 - (x1 - x2));
		builder.register_basis_variable(x6, 6 - x2);
		builder.set_target_function(x1 + 2 * x2);


		auto table = builder.build_table();

		auto free_variables = table.free_variables();
	}
}

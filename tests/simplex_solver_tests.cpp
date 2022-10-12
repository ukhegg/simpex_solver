//
// Created by ukhegg on 22-Apr-22.
//

#include <simplex_solver/simplex_solver_t.hpp>
#include <catch2/catch_all.hpp>

namespace simplex_solver::tests
{
	TEST_CASE("simplex_solver_t tests")
	{
		variable_t x1{"x1"};
		variable_t x2{"x2"};

		simplex_solver_t solver;
		const auto solution = solver.find_max_f({
			                                        rel(-x1 + 2 * x2) >= 2,
			                                        rel(x1 + x2) >= 4,
			                                        rel(x1 - x2) <= 2,
			                                        rel(x2) <= 6,
		                                        },
		                                        x1 + 2 * x2);

		const std::map<variable_t, double> expected_solution = {
			{x1, 8},
			{x2, 6}
		};
		REQUIRE(solution == expected_solution);
	}
}

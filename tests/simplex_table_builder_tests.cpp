//
// Created by ukhegg on 22-Apr-22.
//

#include <catch2/catch_all.hpp>
#include <simplex_solver/simplex_table_builder.hpp>

namespace simplex_solver::tests
{
    using namespace simplex_solver;

	TEST_CASE("simplex_table_builder_t tests")
	{
		std::vector<std::string> expected_outputs =
		{
			//step 1
			"|basis|free member|       x1|       x2|helper coef|\n"
			"|   x3|  -2.000000| 1.000000|-2.000000|        ---|\n"
			"|   x4|  -4.000000|-1.000000|-1.000000|        ---|\n"
			"|   x5|   2.000000| 1.000000|-1.000000|        ---|\n"
			"|   x6|   6.000000| 0.000000| 1.000000|        ---|\n"
			"|    F|   0.000000|-1.000000|-2.000000|        ---|",
			//step 2
			"|basis|free member|       x1|       x3|helper coef|\n"
			"|   x2|   1.000000|-0.500000|-0.500000|        ---|\n"
			"|   x4|  -3.000000|-1.500000|-0.500000|   1.000000|\n"
			"|   x5|   3.000000| 0.500000|-0.500000|   1.000000|\n"
			"|   x6|   5.000000| 0.500000| 0.500000|  -1.000000|\n"
			"|    F|   2.000000|-2.000000|-1.000000|   2.000000|",
			//step 3
			"|basis|free member|       x4|       x3|helper coef|\n"
			"|   x2|   2.000000|-0.333333|-0.333333|   0.500000|\n"
			"|   x1|   2.000000|-0.666667| 0.333333|        ---|\n"
			"|   x5|   2.000000| 0.333333|-0.666667|  -0.500000|\n"
			"|   x6|   4.000000| 0.333333| 0.333333|  -0.500000|\n"
			"|    F|   6.000000|-1.333333|-0.333333|   2.000000|",
			// step 4
			"|basis|free member|       x5|       x3|helper coef|\n"
			"|   x2|   4.000000| 1.000000|-1.000000|   0.333333|\n"
			"|   x1|   6.000000| 2.000000|-1.000000|   0.666667|\n"
			"|   x4|   6.000000| 3.000000|-2.000000|        ---|\n"
			"|   x6|   2.000000|-1.000000| 1.000000|  -0.333333|\n"
			"|    F|  14.000000| 4.000000|-3.000000|   1.333333|",
			// step 5
			"|basis|free member|       x5|      x6|helper coef|\n"
			"|   x2|   6.000000| 0.000000|1.000000|   1.000000|\n"
			"|   x1|   8.000000| 1.000000|1.000000|   1.000000|\n"
			"|   x4|  10.000000| 1.000000|2.000000|   2.000000|\n"
			"|   x3|   2.000000|-1.000000|1.000000|        ---|\n"
			"|    F|  20.000000| 1.000000|3.000000|   3.000000|"
		};
		variable_t x1{"x1"};
		variable_t x2{"x2"};
		variable_t x3{"x3"};
		variable_t x4{"x4"};
		variable_t x5{"x5"};
		variable_t x6{"x6"};

		simplex_table_builder_t builder;
		builder.register_basis_variable(x3, -2 - (x1 - 2 * x2));
		builder.register_basis_variable(x4, -4 - (-x1 - x2));
		builder.register_basis_variable(x5, 2 - (x1 - x2));
		builder.register_basis_variable(x6, 6 - x2);
		builder.set_target_function(x1 + 2 * x2);

		auto table = builder.build_table();

		std::vector<std::string> history;

		for (auto i = 0; i < 5; ++i)
		{
			INFO("step " + std::to_string(i) + "\n\n");
			auto table_str = table.to_string([](variable_t const& var)
			{
				return std::string(var.name());
			});
			history.push_back(table_str);
			auto expected = expected_outputs[i];
			REQUIRE(table_str == expected);

			auto const reference_free_var = table.get_reference_free_variables()[0];

			auto const reference_basis_var = table.get_reference_basis_variable(reference_free_var);

			table = table.get_next(reference_basis_var, reference_free_var);
		}
	}
}

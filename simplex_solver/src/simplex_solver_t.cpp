//
// Created by DerKunde on 21-Apr-22.
//

#include "simplex_solver/simplex_solver_t.hpp"


#include <algorithm>
#include <stdexcept>

#include "simplex_solver/simplex_table_builder.hpp"

namespace simplex_solver
{
	namespace
	{
		simplex_table_with_data_t<variable_t> build_initial_table(std::vector<relation_t> const& relations,
		                                                          expression_t const& target_f)
		{
			simplex_table_builder_t table_builder;
			table_builder.set_target_function(target_f);
			std::vector<variable_t> additional_variables;

			auto add_additional_basis_var = [&](expression_t const& expr)
			{
				const variable_t addition_var = {"additional_var #" + std::to_string(additional_variables.size())};
				additional_variables.push_back(addition_var);
				table_builder.register_basis_variable(addition_var, expr);
			};

			for (auto const& relation : relations)
			{
				switch (relation.relation())
				{
				case relation_t::relation_type_t::less_or_equal:
					add_additional_basis_var(relation.value() - relation.expr());
					break;
				case relation_t::relation_type_t::equal:
					add_additional_basis_var(relation.expr() - relation.value());
					add_additional_basis_var(relation.value() - relation.expr());
					break;
				case relation_t::relation_type_t::greater_or_equal:
					add_additional_basis_var(relation.expr() - relation.value());
					break;
				default:
					throw std::runtime_error("unknown relation type");
				}
			}

			return table_builder.build_table();
		}

		bool is_optimized_table(simplex_table_t const& table)
		{
			return std::all_of(table.free_variables().begin(),
			                   table.free_variables().end(),
			                   [table](auto free_var)
			                   {
				                   return table.target_f_row().free_var_coef(free_var).value_or(-1) > 0;
			                   });
		}
	}

	std::map<variable_t, double> simplex_solver_t::find_max_f(std::vector<relation_t> const& relations,
	                                                          expression_t const& target_f)
	{
		auto table = build_initial_table(relations, target_f);

		while (!is_optimized_table(table.raw_table()))
		{
			auto reference_free_var = table.get_reference_free_variables()[0];
			auto reference_basis_var = table.get_reference_basis_variable(reference_free_var);

			table = table.get_next(reference_basis_var, reference_free_var);
		}

		std::map<variable_t, double> free_variable_values;
		for (auto const& b_var : table.basis_variables())
		{
			auto data_or_none = b_var.get_data();
			if (data_or_none == std::nullopt) continue;

			free_variable_values[data_or_none.value()] = table.raw_table()
			                                                  .free_member_column()
			                                                  .basis_coef(b_var.variable())
			                                                  .value();
		}

		std::map<variable_t, double> result;
		for (auto const& [var,_] : target_f.variable_coefficients())
		{
			result[var] = free_variable_values.at(var);
		}
		return result;
	}
}

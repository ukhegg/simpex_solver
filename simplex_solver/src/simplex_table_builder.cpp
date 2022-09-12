//
// Created by ukhegg on 22-Apr-22.
//

#include "simplex_solver/simplex_table_builder.hpp"

#include <set>
#include <stdexcept>

namespace simplex_solver
{
	void simplex_table_builder_t::register_basis_variable(variable_t variable,
	                                                      expression_t expr)
	{
		for (const auto& [var, expr] : this->basis_)
		{
			if (expr.contains_variable(variable))
			{
				throw std::runtime_error("basis variable used is expression");
			}

			if (expr.contains_variable(var))
			{
				throw std::runtime_error("basis variable used is expression");
			}
		}
		if (this->basis_.count(variable) != 0)
		{
			throw std::runtime_error("basis variable already registered");
		}

		this->basis_[variable] = expr;
		for (auto const& [var, _] : expr.variable_coefficients())
		{
			this->free_variables_.insert(var);
		}
	}

	simplex_table_t simplex_table_builder_t::build_simple_table() const
	{
		simplex_table_t result{this->basis_.size(), this->free_variables_.size()};

		std::map<variable_t, simplex_table_t::basis_variable_t> basis_variables;
		size_t basis_var_index = 0;
		for (auto const& [basis_var, _] : this->basis_)
		{
			basis_variables[basis_var] = result.basis_variables().at(basis_var_index++);
		}
		std::map<variable_t, simplex_table_t::free_variable_t> free_variables;
		size_t free_var_index = 0;
		for (auto const& free_var : this->free_variables_)
		{
			free_variables[free_var] = result.free_variables().at(free_var_index++);
		}

		for (auto const& [basis_var, expr] : this->basis_)
		{
			auto row = result.basis_var_row(basis_variables.at(basis_var));
			row.set_free_member(expr.free_member());
			row.set_helper_coef(0);

			for (auto const& [free_var, val] : expr.variable_coefficients())
			{
				row.set_free_var_coef(free_variables.at(free_var), -val);
			}
		}

		auto target_f_row = result.target_f_row();
		target_f_row.set_free_member(this->target_function_.free_member());
		target_f_row.set_helper_coef(0);
		for (auto const& [free_var, val] : this->target_function_.variable_coefficients())
		{
			target_f_row.set_free_var_coef(free_variables.at(free_var), -val);
		}

		return result;
	}

	simplex_table_with_data_t<variable_t> simplex_table_builder_t::build_table() const
	{
		std::map<simplex_table_t::basis_variable_t, variable_t> basis_var_mapping;
		std::map<simplex_table_t::free_variable_t, variable_t> free_var_mapping;

		simplex_table_t table = {this->basis_.size(), this->free_variables_.size()};

		size_t basis_var_index = 0;
		for (auto const& [basis_var, _] : this->basis_)
		{
			auto table_basis_var = table.basis_variables().at(basis_var_index++);
			basis_var_mapping[table_basis_var] = basis_var;
		}
		size_t free_var_index = 0;
		for (auto const& free_var : this->free_variables_)
		{
			auto table_free_var = table.free_variables().at(free_var_index++);
			free_var_mapping[table_free_var] = free_var;
		}

		auto fill_coefficients = [this, &table](simplex_table_t::row_t dst_row,
		                                         expression_t const& src_expr)
		{
			dst_row.set_free_member(src_expr.free_member());
			auto free_it1 = this->free_variables_.begin();
			auto free_it2 = table.free_variables().begin();

			for (; free_it1 != this->free_variables_.end(); ++free_it1, ++free_it2)
			{
				auto f_it = src_expr.variable_coefficients().find(*free_it1);
				if (f_it == src_expr.variable_coefficients().end())
				{
					dst_row.set_free_var_coef(*free_it2, 0);
				}
				else
				{
					dst_row.set_free_var_coef(*free_it2, -f_it->second);
				}
			}
		};

		auto basis_it1 = this->basis_.begin();
		auto basis_it2 = table.basis_variables().begin();
		for (; basis_it1 != this->basis_.end(); ++basis_it1, ++basis_it2)
		{
			auto const& expr = basis_it1->second;
			auto row = table.basis_var_row(*basis_it2);
			fill_coefficients(row, expr);
		}

		fill_coefficients(table.target_f_row(), this->target_function_);


		simplex_table_with_data_t<variable_t> result(std::move(table));
		for(auto const& [free_var, variable] : free_var_mapping)
		{
			result.set_data(free_var, variable);
		}
		for (auto const& [basis_var, variable] : basis_var_mapping)
		{
			result.set_data(basis_var, variable);
		}

		return result;
	}

	void simplex_table_builder_t::set_target_function(expression_t target_function)
	{
		for (const auto& [var, val] : this->basis_)
		{
			if (target_function.contains_variable(var))
			{
				throw std::runtime_error("target function contains basis variables!");
			}
		}
		this->target_function_ = std::move(target_function);
	}
}

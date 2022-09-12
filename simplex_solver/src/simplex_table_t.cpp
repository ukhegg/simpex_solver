//
// Created by ukhegg on 22-Apr-22.
//

#include "simplex_solver/simplex_table_t.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>
namespace simplex_solver
{
	using namespace std::string_literals;


	simplex_table_t::simplex_table_t() = default;

	std::vector<simplex_table_t::basis_variable_t> const& simplex_table_t::basis_variables() const
	{
		return this->basis_variables_;
	}

	std::vector<simplex_table_t::free_variable_t> const& simplex_table_t::free_variables() const
	{
		return this->free_variables_;
	}

#pragma region const_row_t
	simplex_table_t::const_row_t::const_row_t(simplex_table_t const* this_table, basis_variable_t const& b_var)
		: table_(this_table),
		  b_var_(b_var)
	{
	}

	std::optional<double> simplex_table_t::const_row_t::free_member() const
	{
		return this->table_->get_coef(this->b_var_, this->table_->free_member_);
	}

	std::optional<double> simplex_table_t::const_row_t::helper_coef() const
	{
		return this->table_->get_coef(this->b_var_, this->table_->helper_member_);
	}

	std::optional<double> simplex_table_t::const_row_t::free_var_coef(const free_variable_t& free_var) const
	{
		return this->table_->get_coef(this->b_var_, free_var);
	}
#pragma endregion

#pragma region row_t
	simplex_table_t::row_t::row_t(simplex_table_t* this_table, basis_variable_t const& b_var)
		: table_(this_table),
		  b_var_(b_var)
	{
	}

	std::optional<double> simplex_table_t::row_t::free_member() const
	{
		return this->table_->get_coef(this->b_var_, this->table_->free_member_);
	}

	void simplex_table_t::row_t::set_free_member(double val)
	{
		this->table_->set_coef(this->b_var_, this->table_->free_member_, val);
	}

	std::optional<double> simplex_table_t::row_t::helper_coef() const
	{
		return this->table_->get_coef(this->b_var_, this->table_->helper_member_);
	}

	void simplex_table_t::row_t::set_helper_coef(double val)
	{
		this->table_->set_coef(this->b_var_, this->table_->helper_member_, val);
	}

	std::optional<double> simplex_table_t::row_t::free_var_coef(const free_variable_t& free_var) const
	{
		return this->table_->get_coef(this->b_var_, free_var);
	}

	void simplex_table_t::row_t::set_free_var_coef(const free_variable_t& free_var, double coef)
	{
		this->table_->set_coef(this->b_var_, free_var, coef);
	}
#pragma endregion

#pragma region const column_t
	simplex_table_t::const_column_t::const_column_t(simplex_table_t const* table, const free_variable_t& f_var)
		: table_(table),
		  f_var_(f_var)
	{
	}

	std::optional<double> simplex_table_t::const_column_t::target_f_coef() const
	{
		return this->table_->get_coef(this->table_->target_f_, this->f_var_);
	}

	std::optional<double> simplex_table_t::const_column_t::basis_coef(basis_variable_t const& b_var) const
	{
		return this->table_->get_coef(b_var, this->f_var_);
	}
#pragma endregion

#pragma region column_t
	simplex_table_t::column_t::column_t(simplex_table_t* table, const free_variable_t& f_var)
		: table_(table),
		  f_var_(f_var)
	{
	}

	std::optional<double> simplex_table_t::column_t::target_f_coef() const
	{
		return this->table_->get_coef(this->table_->target_f_, this->f_var_);
	}

	void simplex_table_t::column_t::set_target_f_coef(double val)
	{
		this->table_->set_coef(this->table_->target_f_, this->f_var_, val);
	}

	std::optional<double> simplex_table_t::column_t::basis_var_coef(basis_variable_t const& b_var) const
	{
		return this->table_->get_coef(b_var, this->f_var_);
	}

	void simplex_table_t::column_t::set_basis_coef(basis_variable_t const& b_var, double val)
	{
		this->table_->set_coef(b_var, this->f_var_, val);
	}
#pragma endregion

	simplex_table_t::simplex_table_t(size_t num_basis_variables,
	                                 size_t num_free_variables)
	{
		this->basis_variables_.reserve(num_basis_variables);
		for (size_t basis_var_index = 0; basis_var_index < num_basis_variables; ++basis_var_index)
		{
			this->basis_variables_.push_back({basis_var_index});
		}
		this->target_f_ = {num_basis_variables};


		this->free_member_ = {0};
		this->free_variables_.reserve(num_free_variables);
		for (size_t free_var_index = 0; free_var_index < num_free_variables; ++free_var_index)
		{
			this->free_variables_.push_back({free_var_index + 1});
		}
		this->helper_member_ = {num_free_variables + 1};

		this->coefficients_.resize((num_basis_variables + 1) * (num_free_variables + 2));
	}

	simplex_table_t::row_t simplex_table_t::basis_var_row(basis_variable_t const& basis_var)
	{
		return row_t{this, basis_var};
	}

	simplex_table_t::const_row_t simplex_table_t::basis_var_row(basis_variable_t const& basis_var) const
	{
		return const_row_t{this, basis_var};
	}

	simplex_table_t::row_t simplex_table_t::target_f_row()
	{
		return row_t{this, this->target_f_};
	}

	simplex_table_t::const_row_t simplex_table_t::target_f_row() const
	{
		return const_row_t{this, this->target_f_};
	}

	simplex_table_t::column_t simplex_table_t::free_member_column()
	{
		return column_t{this, this->free_member_};
	}

	simplex_table_t::const_column_t simplex_table_t::free_member_column() const
	{
		return const_column_t{this, this->free_member_};
	}

	simplex_table_t::column_t simplex_table_t::free_var_column(free_variable_t const& free_var)
	{
		return column_t{this, free_var};
	}

	simplex_table_t::const_column_t simplex_table_t::free_var_column(free_variable_t const& free_var) const
	{
		return const_column_t{this, free_var};
	}

	simplex_table_t::column_t simplex_table_t::helper_member_column()
	{
		return column_t{this, this->helper_member_};
	}

	simplex_table_t::const_column_t simplex_table_t::helper_member_column() const
	{
		return const_column_t{this, this->helper_member_};
	}

	std::string simplex_table_t::to_string() const
	{
		return this->to_string({}, {});
	}

	std::string simplex_table_t::to_string(std::map<basis_variable_t, std::string> const& b_var_mapping,
	                                       std::map<free_variable_t, std::string> const& f_var_mapping) const
	{
		auto to_string = [](std::optional<double> const& val)
		{
			return val == std::nullopt ? "---" : std::to_string(val.value());
		};

		auto b_var_name = [this, &b_var_mapping](basis_variable_t const& var) -> std::string
		{
			if (b_var_mapping.count(var) > 0)
			{
				return b_var_mapping.at(var);
			}
			return "basis var#" + std::to_string(var.index);
		};

		auto f_var_name = [this, &f_var_mapping](free_variable_t const& var) -> std::string
		{
			if (f_var_mapping.count(var) > 0)
			{
				return f_var_mapping.at(var);
			}
			return "free var#" + std::to_string(var.index);
		};

		struct table_row_t
		{
			std::vector<std::string> cells;
		};


		std::vector<table_row_t> rows;

		auto& header = rows.emplace_back();
		header.cells.emplace_back("basis");
		header.cells.emplace_back("free member");
		for (auto const& free_var : this->free_variables_)
		{
			header.cells.emplace_back(f_var_name(free_var));
		}
		header.cells.emplace_back("helper coef");

		for (auto basis_var : this->basis_variables_)
		{
			auto& row = rows.emplace_back();
			row.cells.emplace_back(b_var_name(basis_var));

			auto table_row = this->basis_var_row(basis_var);
			row.cells.emplace_back(to_string(table_row.free_member()));
			for (auto const& free_var : this->free_variables_)
			{
				row.cells.emplace_back(to_string(table_row.free_var_coef(free_var)));
			}
			row.cells.emplace_back(to_string(table_row.helper_coef()));
		}
		auto& footer = rows.emplace_back();
		auto target_f_row = this->target_f_row();
		footer.cells.emplace_back("F");
		footer.cells.emplace_back(to_string(target_f_row.free_member()));
		for (auto const& free_var : this->free_variables_)
		{
			footer.cells.emplace_back(to_string(target_f_row.free_var_coef(free_var)));
		}
		footer.cells.emplace_back(to_string(target_f_row.helper_coef()));


		auto get_column_width = [&](size_t index)
		{
			size_t result = 0;
			for (auto const& row : rows)
			{
				result = std::max(result, row.cells[index].size());
			}
			return result;
		};
		std::ostringstream oss;
		bool is_first_row = true;

		for (auto const& row : rows)
		{
			if (is_first_row) is_first_row = false;
			else oss << std::endl;

			bool is_first_column = true;
			size_t table_width = 0;

			for (size_t column = 0; column < row.cells.size(); ++column)
			{
				auto width = get_column_width(column);

				if (is_first_column)
				{
					oss << '|';
					table_width += 1;
					is_first_column = false;
				}
				oss << std::setw(width) << row.cells[column] << '|';
				table_width += width;
				table_width += 1;
			}
		}


		return oss.str();
	}

	std::optional<double> simplex_table_t::get_coef(basis_variable_t const& b_var, free_variable_t const& f_var) const
	{
		return this->coefficients_.at(linear_index(b_var, f_var));
	}

	void simplex_table_t::set_coef(basis_variable_t const& b_var, free_variable_t const& f_var, double val)
	{
		this->coefficients_.at(linear_index(b_var, f_var)) = val;
	}

	size_t simplex_table_t::linear_index(basis_variable_t const& b_var, free_variable_t const& f_var) const
	{
		const auto row_size = this->free_variables_.size() + 2;
		return row_size * b_var.index + f_var.index;
	}

	void simplex_table_t::fill_first_row(simplex_table_t const& prev,
	                                     basis_variable_t const& reference_basis_var,
	                                     free_variable_t const& reference_free_var)
	{
		const auto reference_src_row = prev.basis_var_row(reference_basis_var);
		const auto reference_coef = reference_src_row.free_var_coef(reference_free_var).value();
		auto first_dst_row = this->basis_var_row(reference_basis_var);
		first_dst_row.set_free_member(reference_src_row.free_member().value() / reference_coef);
		first_dst_row.set_free_var_coef(reference_free_var, 1 / reference_coef);
		for (auto const& free_var : prev.free_variables())
		{
			if (free_var == reference_free_var) continue;
			first_dst_row.set_free_var_coef(
				free_var, reference_src_row.free_var_coef(free_var).value() / reference_coef);
		}
	}

	void simplex_table_t::fill_helper_column(simplex_table_t const& prev,
	                                         basis_variable_t const& reference_basis_var,
	                                         free_variable_t const& reference_free_var)
	{
		for (auto const& b_var : prev.basis_variables_)
		{
			if (b_var == reference_basis_var) continue;
			auto dst_row = this->basis_var_row(b_var);
			auto src_row = prev.basis_var_row(b_var);
			dst_row.set_helper_coef(-src_row.free_var_coef(reference_free_var).value());
		}
		this->target_f_row().set_helper_coef(-prev.target_f_row().free_var_coef(reference_free_var).value());
	}

	void simplex_table_t::fill_remainder_coefficients(simplex_table_t const& prev,
	                                                  basis_variable_t const& reference_basis_var,
	                                                  free_variable_t const& reference_free_var)
	{
		auto const dst_first_row = this->basis_var_row(reference_basis_var);
		auto const helper_column = this->helper_member_column();

		auto fill_row = [&](basis_variable_t b_var)
		{
			auto dst_row = this->basis_var_row(b_var);
			const auto helper_coef = helper_column.basis_var_coef(b_var).value();
			auto const src_row = prev.basis_var_row(b_var);

			dst_row.set_free_member(dst_first_row.free_member().value() * helper_coef + src_row.free_member().value());
			for (const auto f_var : this->free_variables_)
			{
				const auto first_row_value = dst_first_row.free_var_coef(f_var).value();


				if (f_var == reference_free_var)
				{
					dst_row.set_free_var_coef(f_var, first_row_value * helper_coef);
				}
				else
				{
					const auto src_coef = src_row.free_var_coef(f_var).value();
					dst_row.set_free_var_coef(f_var, first_row_value * helper_coef + src_coef);
				}
			}
		};


		for (const auto b_var : this->basis_variables_)
		{
			if (b_var == reference_basis_var) continue;
			fill_row(b_var);
		}
		fill_row(this->target_f_);
	}

	simplex_table_t::free_variable_t simplex_table_t::get_reference_free_variable() const
	{
		return this->get_reference_free_variables().front();
		//free_variable_t reference_free_variable;
		//double min_index_coef{0};
		//for (auto const& free_var : this->free_variables_)
		//{
		//	const auto abs_coef = std::abs(this->target_f_row().free_var_coef(free_var).value_or(0));
		//	if (abs_coef < min_index_coef) continue;
		//	reference_free_variable = free_var;
		//	min_index_coef = abs_coef;
		//}
		//return reference_free_variable;
	}

	std::vector<simplex_table_t::free_variable_t> simplex_table_t::get_reference_free_variables() const
	{
		auto result = this->free_variables_;
		std::sort(result.begin(), result.end(), [this](free_variable_t const& lhs, free_variable_t const& rhs)
		{
			auto lhs_coef = this->target_f_row().free_var_coef(lhs).value_or(0);
			auto rhs_coef = this->target_f_row().free_var_coef(rhs).value_or(0);
			return lhs_coef < rhs_coef;
		});
		return result;
	}

	auto simplex_table_t::get_reference_basis_variable(free_variable_t const& reference_free_var) const
	-> basis_variable_t
	{
		double min_relation = std::numeric_limits<double>::max();
		basis_variable_t result;

		const auto ref_column = this->free_var_column(reference_free_var);
		const auto free_member_column = this->free_member_column();

		for (auto const& basis_var : this->basis_variables_)
		{
			const auto free_member = free_member_column.basis_coef(basis_var).value();
			const auto ref_column_value = ref_column.basis_coef(basis_var).value();

			if (free_member > 0 && ref_column_value < 0) continue;

			auto relation = free_member / ref_column_value;
			if (relation >= min_relation) continue;

			min_relation = relation;
			result = basis_var;
		}
		return result;
	}

	simplex_table_t simplex_table_t::get_next(basis_variable_t const& reference_basis_var,
	                                          free_variable_t const& reference_free_var) const
	{
		simplex_table_t next(this->basis_variables_.size(), this->free_variables_.size());

		next.fill_first_row(*this, reference_basis_var, reference_free_var);
		next.fill_helper_column(*this, reference_basis_var, reference_free_var);
		next.fill_remainder_coefficients(*this, reference_basis_var, reference_free_var);

		return next;
	}
}

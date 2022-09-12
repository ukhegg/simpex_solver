//
// Created by ukhegg on 22-Apr-22.
//

#pragma once
#include <vector>
#include <map>
#include <optional>
#include <string>
#include "simplex_solver/simplex_solver_api.hpp"

namespace simplex_solver
{
	class SIMPLEX_SOLVER_API simplex_table_t
	{
	public:
		struct basis_variable_tag_t
		{
		};

		struct free_variable_tag_t
		{
		};

		template <class TTag>
		struct simplex_variable_t
		{
			size_t index{0};

			friend bool operator==(const simplex_variable_t& lhs, const simplex_variable_t& rhs)
			{
				return lhs.index == rhs.index;
			}

			friend bool operator!=(const simplex_variable_t& lhs, const simplex_variable_t& rhs)
			{
				return !(lhs == rhs);
			}

			friend bool operator<(const simplex_variable_t& lhs, const simplex_variable_t& rhs)
			{
				return lhs.index < rhs.index;
			}

			friend bool operator<=(const simplex_variable_t& lhs, const simplex_variable_t& rhs)
			{
				return !(rhs < lhs);
			}

			friend bool operator>(const simplex_variable_t& lhs, const simplex_variable_t& rhs)
			{
				return rhs < lhs;
			}

			friend bool operator>=(const simplex_variable_t& lhs, const simplex_variable_t& rhs)
			{
				return !(lhs < rhs);
			}
		};

		using basis_variable_t = simplex_variable_t<basis_variable_tag_t>;
		using free_variable_t = simplex_variable_t<free_variable_tag_t>;

		simplex_table_t();

		simplex_table_t(size_t num_basis_variables,
		                size_t num_free_variables);

		[[nodiscard]] std::vector<basis_variable_t> const& basis_variables() const;
		[[nodiscard]] std::vector<free_variable_t> const& free_variables() const;

		class const_row_t
		{
		public:
			const_row_t(simplex_table_t const* this_table, basis_variable_t const& b_var);

			[[nodiscard]] std::optional<double> free_member() const;

			[[nodiscard]] std::optional<double> helper_coef() const;

			[[nodiscard]] std::optional<double> free_var_coef(free_variable_t const& free_var) const;
		private:
			simplex_table_t const* table_{nullptr};
			basis_variable_t b_var_;
		};

		class row_t
		{
		public:
			row_t(simplex_table_t* this_table, basis_variable_t const& b_var);

			[[nodiscard]] std::optional<double> free_member() const;
			void set_free_member(double val);

			[[nodiscard]] std::optional<double> helper_coef() const;
			void set_helper_coef(double val);

			[[nodiscard]] std::optional<double> free_var_coef(free_variable_t const& free_var) const;
			void set_free_var_coef(free_variable_t const& free_var, double coef);

			[[nodiscard]] inline operator const_row_t() const
			{
				return const_row_t{this->table_, this->b_var_};
			}

		private:
			simplex_table_t* table_{nullptr};
			basis_variable_t b_var_;
		};

		class const_column_t
		{
		public:

			const_column_t(simplex_table_t const* table, const free_variable_t& f_var);

			[[nodiscard]] std::optional<double> target_f_coef() const;

			[[nodiscard]] std::optional<double> basis_coef(basis_variable_t const& b_var) const;

		private:
			simplex_table_t const* table_{nullptr};
			free_variable_t f_var_;
		};

		class column_t
		{
		public:

			column_t(simplex_table_t* table, const free_variable_t& f_var);

			[[nodiscard]] std::optional<double> target_f_coef() const;
			void set_target_f_coef(double val);

			[[nodiscard]] std::optional<double> basis_var_coef(basis_variable_t const& b_var) const;
			void set_basis_coef(basis_variable_t const& b_var, double val);

			[[nodiscard]] inline operator const_column_t() const
			{
				return const_column_t{this->table_, this->f_var_};
			}

		private:
			simplex_table_t* table_{nullptr};
			free_variable_t f_var_;
		};

		[[nodiscard]] row_t basis_var_row(basis_variable_t const& basis_var);
		[[nodiscard]] const_row_t basis_var_row(basis_variable_t const& basis_var) const;

		[[nodiscard]] row_t target_f_row();
		[[nodiscard]] const_row_t target_f_row() const;

		[[nodiscard]] column_t free_member_column();
		[[nodiscard]] const_column_t free_member_column() const;

		[[nodiscard]] column_t free_var_column(free_variable_t const& free_var);
		[[nodiscard]] const_column_t free_var_column(free_variable_t const& free_var) const;

		[[nodiscard]] column_t helper_member_column();
		[[nodiscard]] const_column_t helper_member_column() const;

		[[nodiscard]] std::string to_string() const;

		[[nodiscard]] std::string to_string(std::map<basis_variable_t, std::string> const& b_var_mapping,
		                                    std::map<free_variable_t, std::string> const& f_var_mapping) const;

		[[nodiscard]] free_variable_t get_reference_free_variable() const;

		[[nodiscard]] std::vector<free_variable_t> get_reference_free_variables() const;
		
		[[nodiscard]] basis_variable_t get_reference_basis_variable(free_variable_t const& reference_free_var) const;
		
		[[nodiscard]] simplex_table_t get_next(basis_variable_t const& reference_basis_var,
		                                       free_variable_t const& reference_free_var) const;
	private:
		basis_variable_t target_f_;
		free_variable_t free_member_;
		free_variable_t helper_member_;
		std::vector<basis_variable_t> basis_variables_;
		std::vector<free_variable_t> free_variables_;
		std::vector<std::optional<double>> coefficients_;

		[[nodiscard]] std::optional<double> get_coef(basis_variable_t const& b_var, free_variable_t const& f_var) const;
		void set_coef(basis_variable_t const& b_var, free_variable_t const& f_var, double val);
		[[nodiscard]] size_t linear_index(basis_variable_t const& b_var, free_variable_t const& f_var) const;


		void fill_first_row(simplex_table_t const& prev,
		                    basis_variable_t const& reference_basis_var,
		                    free_variable_t const& reference_free_var);

		void fill_helper_column(simplex_table_t const& prev,
		                        basis_variable_t const& reference_basis_var,
		                        free_variable_t const& reference_free_var);

		void fill_remainder_coefficients(simplex_table_t const& prev,
		                                 basis_variable_t const& reference_basis_var,
		                                 free_variable_t const& reference_free_var);

	};
}
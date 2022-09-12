//
// Created by ukhegg on 25-Apr-22.
//

#pragma once
#include <stdexcept>
#include <functional>
#include "simplex_solver/simplex_table_t.hpp"

namespace simplex_solver
{
	template <class TData>
	class simplex_table_with_data_t
	{
	public:
		template <class TTag>
		class variable_with_data_t
		{
		public:
			variable_with_data_t()
			{
			}

			explicit variable_with_data_t(simplex_table_t::simplex_variable_t<TTag> var,
			                              simplex_table_with_data_t* this_table)
				: this_table_(this_table),
				  variable_(var)
			{
			}

			[[nodiscard]] simplex_table_t::simplex_variable_t<TTag> variable() const
			{
				return this->variable_;
			}

			[[nodiscard]] std::optional<TData> get_data() const
			{
				if (this->this_table_ == nullptr)
				{
					throw std::runtime_error("nullptr this_table_");
				}

				auto const& data_map = this->get_data_map();
				auto it = data_map.find(this->variable_);
				if (it == data_map.end())
				{
					return std::nullopt;
				}
				return it->second;
			}

			void set_data(TData const& data)
			{
				this->get_data_map()[this->variable_] = data;
			}

		private:
			simplex_table_with_data_t* this_table_{nullptr};
			simplex_table_t::simplex_variable_t<TTag> variable_;

			[[nodiscard]] std::map<simplex_table_t::simplex_variable_t<TTag>, TData> const& get_data_map() const
			{
				if constexpr (std::is_same_v<TTag, simplex_table_t::basis_variable_tag_t>)
				{
					return this->this_table_->basis_vars_data_;
				}
				else
				{
					return this->this_table_->free_vars_data_;
				}
			}

			[[nodiscard]] std::map<simplex_table_t::simplex_variable_t<TTag>, TData>& get_data_map()
			{
				if constexpr (std::is_same_v<TTag, simplex_table_t::basis_variable_tag_t>)
				{
					return this->this_table_->basis_vars_data_;
				}
				else
				{
					return this->this_table_->free_vars_data_;
				}
			}
		};

		template <class TTag>
		class const_variable_with_data_t
		{
		public:
			const_variable_with_data_t()
			{
			}

			explicit const_variable_with_data_t(simplex_table_t::simplex_variable_t<TTag> var,
			                                    simplex_table_with_data_t const* this_table)
				: this_table_(this_table),
				  variable_(var)
			{
			}

			[[nodiscard]] simplex_table_t::simplex_variable_t<TTag> variable() const
			{
				return this->variable_;
			}

			[[nodiscard]] std::optional<TData> get_data() const
			{
				if (this->this_table_ == nullptr)
				{
					throw std::runtime_error("nullptr this_table_");
				}

				auto const& data_map = this->get_data_map();
				auto it = data_map.find(this->variable_);
				if (it == data_map.end())
				{
					return std::nullopt;
				}
				return it->second;
			}

		private:
			simplex_table_with_data_t const* this_table_{nullptr};
			simplex_table_t::simplex_variable_t<TTag> variable_;

			[[nodiscard]] std::map<simplex_table_t::simplex_variable_t<TTag>, TData> const& get_data_map() const
			{
				if constexpr (std::is_same_v<TTag, simplex_table_t::basis_variable_tag_t>)
				{
					return this->this_table_->basis_vars_data_;
				}
				else
				{
					return this->this_table_->free_vars_data_;
				}
			}

			[[nodiscard]] std::map<simplex_table_t::simplex_variable_t<TTag>, TData>& get_data_map()
			{
				if constexpr (std::is_same_v<TTag, simplex_table_t::basis_variable_tag_t>)
				{
					return this->this_table_->basis_vars_data_;
				}
				else
				{
					return this->this_table_->free_vars_data_;
				}
			}
		};

		using basis_variable_t = variable_with_data_t<simplex_table_t::basis_variable_tag_t>;
		using const_basis_variable_t = const_variable_with_data_t<simplex_table_t::basis_variable_tag_t>;
		using free_variable_t = variable_with_data_t<simplex_table_t::free_variable_tag_t>;
		using const_free_variable_t = const_variable_with_data_t<simplex_table_t::free_variable_tag_t>;

		simplex_table_with_data_t() = default;

		explicit simplex_table_with_data_t(simplex_table_t table)
			: simplex_table_(std::move(table))
		{
		}

		[[nodiscard]] simplex_table_t const& raw_table() const { return this->simplex_table_; }

		[[nodiscard]] std::vector<basis_variable_t> basis_variables()
		{
			std::vector<basis_variable_t> result;
			result.reserve(this->simplex_table_.basis_variables().size());
			for (auto var : this->simplex_table_.basis_variables())
			{
				result.emplace_back(var, this);
			}
			return result;
		}

		[[nodiscard]] std::vector<const_basis_variable_t> basis_variables() const
		{
			std::vector<const_basis_variable_t> result;
			result.reserve(this->simplex_table_.basis_variables().size());
			for (auto var : this->simplex_table_.basis_variables())
			{
				result.emplace_back(var, this);
			}
			return result;
		}

		[[nodiscard]] std::vector<free_variable_t> free_variables()
		{
			std::vector<free_variable_t> result;
			result.reserve(this->simplex_table_.free_variables().size());
			for (auto var : this->simplex_table_.free_variables())
			{
				result.emplace_back(var, this);
			}
			return result;
		}

		[[nodiscard]] std::vector<const_free_variable_t> free_variables() const
		{
			std::vector<const_free_variable_t> result;
			result.reserve(this->simplex_table_.free_variables().size());
			for (auto var : this->simplex_table_.free_variables())
			{
				result.emplace_back(var, this);
			}
			return result;
		}

		[[maybe_unused]] free_variable_t set_data(simplex_table_t::free_variable_t var, TData const& data)
		{
			this->free_vars_data_[var] = data;
			return free_variable_t{var, this};
		}

		[[maybe_unused]] basis_variable_t set_data(simplex_table_t::basis_variable_t var, TData const& data)
		{
			this->basis_vars_data_[var] = data;
			return basis_variable_t{var, this};
		}

		[[nodiscard]] std::string to_string(std::function<std::string(TData const&)> to_str)
		{
			const std::map<simplex_table_t::basis_variable_t, std::string> basis_var_mapping =
				build_mapping(this->simplex_table_.basis_variables(),
				              this->basis_vars_data_,
				              to_str);
			const std::map<simplex_table_t::free_variable_t, std::string> free_var_mapping =
				build_mapping(this->simplex_table_.free_variables(),
				              this->free_vars_data_,
				              to_str);
			return this->simplex_table_.to_string(basis_var_mapping, free_var_mapping);
		}

		[[nodiscard]] std::vector<const_free_variable_t> get_reference_free_variables() const
		{
			std::vector<const_free_variable_t> result;
			result.reserve(this->simplex_table_.free_variables().size());
			for (auto const& free_var : this->simplex_table_.get_reference_free_variables())
			{
				result.emplace_back(free_var, this);
			}
			return result;
		}

		[[nodiscard]] const_basis_variable_t get_reference_basis_variable(const_free_variable_t const& free_var) const
		{
			return const_basis_variable_t(this->simplex_table_.get_reference_basis_variable(free_var.variable()),
			                              this);
		}

		[[nodiscard]] simplex_table_with_data_t get_next(const_basis_variable_t const& reference_basis_var,
		                                                 const_free_variable_t const& reference_free_var) const
		{
			auto next_table = this->simplex_table_.get_next(reference_basis_var.variable(),
			                                                reference_free_var.variable());
			simplex_table_with_data_t result(std::move(next_table));
			result.basis_vars_data_ = this->basis_vars_data_;
			result.basis_vars_data_.erase(reference_basis_var.variable());
			if (reference_free_var.get_data() != std::nullopt)
			{
				result.basis_vars_data_[reference_basis_var.variable()] = reference_free_var.get_data().value();
			}


			result.free_vars_data_ = this->free_vars_data_;
			result.free_vars_data_.erase(reference_free_var.variable());
			if (reference_basis_var.get_data() != std::nullopt)
			{
				result.free_vars_data_[reference_free_var.variable()] = reference_basis_var.get_data().value();
			}


			return result;
		}

	private:
		simplex_table_t simplex_table_;
		std::map<simplex_table_t::basis_variable_t, TData> basis_vars_data_;
		std::map<simplex_table_t::free_variable_t, TData> free_vars_data_;

		template <class TTag>
		static std::map<simplex_table_t::simplex_variable_t<TTag>, std::string> build_mapping(
			std::vector<simplex_table_t::simplex_variable_t<TTag>> const& variables,
			std::map<simplex_table_t::simplex_variable_t<TTag>, TData> const& variables_data,
			std::function<std::string(TData const&)> to_str)
		{
			std::map<simplex_table_t::simplex_variable_t<TTag>, std::string> result;
			for (auto const& var : variables)
			{
				auto it = variables_data.find(var);
				if (it != variables_data.end())
				{
					result[var] = to_str(it->second);
				}
				else
				{
					result[var] = "var #" + std::to_string(var.index);
				}
			}
			return result;
		}
	};
}
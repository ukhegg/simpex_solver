//
// Created by ukhegg on 21-Apr-22.
//

#pragma once
#include "simplex_solver/relation_t.hpp"

namespace simplex_solver
{
	class SIMPLEX_SOLVER_API simplex_solver_t
	{
	public:
		std::map<variable_t, double> find_max_f(std::vector<relation_t> const& relations,
		                                        expression_t const& target_f);
	};
}



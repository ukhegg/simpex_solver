//
// Created by DerKunde on 21-Apr-22.
//

#ifndef SIMPLEX_SOLVER_SIMPLEX_SOLVER_T_HPP
#define SIMPLEX_SOLVER_SIMPLEX_SOLVER_T_HPP

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


#endif //SIMPLEX_SOLVER_SIMPLEX_SOLVER_T_HPP

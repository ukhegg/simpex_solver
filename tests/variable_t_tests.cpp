//
// Created by ukhegg on 21-Apr-22.
//

#include <simplex_solver/variable_t.hpp>
#include <catch2/catch_all.hpp>


namespace simplex_solver::tests
{
    TEST_CASE("variable_t tests")
    {
        SECTION("ctor tests")
        {
            SECTION("can default-ctor variable"){
                variable_t var("a");
                REQUIRE(var.name() == "a");
            }
        }

    }
}

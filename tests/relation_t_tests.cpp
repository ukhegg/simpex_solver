//
// Created by DerKunde on 21-Apr-22.
//

#include <catch2/catch_all.hpp>
#include <simplex_solver/relation_t.hpp>

namespace simplex_solver::tests {
    TEST_CASE("relation tests") {
        auto x1 = variable_t{"x1"};
        auto x2 = variable_t{"x2"};

        GIVEN("some relation 1") {
            relation_t rel = {x1 + x2, relation_t::relation_type_t::equal, 10};

            WHEN("formatting to string"){
                auto str = rel.to_string();
                THEN("it is properly formatted"){
                    REQUIRE(str == "[x1+x2] = 10");
                }
            }
        }
        GIVEN("some relation 2") {
            relation_t rel = {-2*x1 + 3*x2, relation_t::relation_type_t::less_or_equal, -5};

            WHEN("formatting to string"){
                auto str = rel.to_string();
                THEN("it is properly formatted"){
                    REQUIRE(str == "[-2x1+3x2] <= -5");
                }
            }
        }

        GIVEN("some relation 3") {
            relation_t rel = {7*x1 -4*x2, relation_t::relation_type_t::greater_or_equal, 6};

            WHEN("formatting to string"){
                auto str = rel.to_string();
                THEN("it is properly formatted"){
                    REQUIRE(str == "[7x1-4x2] >= 6");
                }
            }
        }

        GIVEN("some relation 4") {
            relation_t rel = {x2, relation_t::relation_type_t::greater_or_equal, 0};

            WHEN("formatting to string"){
                auto str = rel.to_string();
                THEN("it is properly formatted"){
                    REQUIRE(str == "[x2] >= 0");
                }
            }
        }
    }
}
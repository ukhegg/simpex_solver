//
// Created by DerKunde on 21-Apr-22.
//

#include <catch2/catch_all.hpp>
#include <simplex_solver/expression_t.hpp>


namespace simplex_solver::tests {
    TEST_CASE("expression_t tests") {
        SECTION("operations tests") {
            GIVEN("default constructed expression") {
                expression_t expr;
                THEN("free member is 0") {
                    REQUIRE(expr.free_member() == 0);
                }AND_THEN("no variables are present in expression") {
                    REQUIRE(expr.variables().empty());
                }
            }

            GIVEN("expression constructed from value") {
                expression_t expr = 5;
                THEN("free member is that value") {
                    REQUIRE(expr.free_member() == 5);
                }AND_THEN("no variables are present in expression") {
                    REQUIRE(expr.variables().empty());
                }
            }

            GIVEN("expression constructed from variable") {
                expression_t expr = variable_t{"a"};
                THEN("free member is null") {
                    REQUIRE(expr.free_member() == 0);
                }AND_THEN("that variable is only variable in expression")
                {
                    REQUIRE(expr.variables().size() == 1);
                    REQUIRE(expr.variables()[0].name() == "a");
                }
            }

            GIVEN("some variable") {
                variable_t a{"a_variable"};
                WHEN("using unary minus on variable") {
                    auto expr = -a;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is zero") {
                        REQUIRE(expr.free_member() == 0);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == -1);
                    }
                }WHEN("adding const value to it") {
                    auto expr = a + 5;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is value") {
                        REQUIRE(expr.free_member() == 5);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == 1);
                    }
                }WHEN("adding it to const value") {
                    auto expr = 5 + a;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is value") {
                        REQUIRE(expr.free_member() == 5);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == 1);
                    }
                }WHEN("subtracting value from it") {
                    auto expr = a - 5;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is value") {
                        REQUIRE(expr.free_member() == -5);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == 1);
                    }
                }WHEN("subtracting it from const value") {
                    auto expr = 5 - a;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is value") {
                        REQUIRE(expr.free_member() == 5);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == -1);
                    }
                }WHEN("multiplying it by value") {
                    auto expr = a * 5;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is zero") {
                        REQUIRE(expr.free_member() == 0);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == 5);
                    }
                }WHEN("dividing variable by const value") {
                    auto expr = a / 5;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(expr), expression_t>);
                    }AND_THEN("free member is zero") {
                        REQUIRE(expr.free_member() == 0);
                    }AND_THEN("variables contains only given variable") {
                        REQUIRE(expr.variables().size() == 1);
                        REQUIRE(expr.variables()[0] == a);
                        REQUIRE(expr[a] == 1.0 / 5);
                    }
                }
            }

            GIVEN("some expression") {
                variable_t a_var = "a";
                expression_t expr = a_var + 10;
                REQUIRE(expr[a_var] == 1);
                REQUIRE(expr.free_member() == 10);
                WHEN("using unary minus on it") {
                    auto res = -expr;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is negated") {
                        REQUIRE(res.free_member() == -10);
                    }AND_THEN("all variables are negated") {
                        REQUIRE(res[a_var] == -1);
                    }
                }WHEN("adding const value to it") {
                    auto res = expr + 5;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == 15);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == 1);
                    }
                }WHEN("adding it to const value") {
                    auto res = 5 + expr;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == 15);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == 1);
                    }
                }WHEN("subtracting const value from it") {
                    auto res = expr - 5;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == 5);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == 1);
                    }
                }WHEN("subtracting it from const value") {
                    auto res = 5 - expr;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == -5);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == -1);
                    }
                }WHEN("multiplying it by const") {
                    auto res = expr * 5;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == 50);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == 5);
                    }
                }WHEN("multiplying const by it") {
                    auto res = 5 * expr;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == 50);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == 5);
                    }
                }WHEN("dividing it by const") {
                    auto res = expr / 5;
                    THEN("result is also expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is changed") {
                        REQUIRE(res.free_member() == 2);
                    }AND_THEN("variables are not changed") {
                        REQUIRE(res[a_var] == 1.0 / 5);
                    }
                }
            }

            GIVEN("some expression and variable") {
                variable_t a_var = "a";
                expression_t expr = a_var + 10;

                variable_t b_var = "b";
                WHEN("adding new variable to expression") {
                    auto res = expr + b_var;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is not affected") {
                        REQUIRE(res.free_member() == 10);
                    }AND_THEN("new variable is added with coef 1 to expression") {
                        REQUIRE(res[b_var] == 1);
                    }AND_THEN("other expression variables are not affected") {
                        REQUIRE(res[a_var] == 1);
                    }
                }WHEN("adding expression to new variable") {
                    auto res = b_var + expr;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is not affected") {
                        REQUIRE(res.free_member() == 10);
                    }AND_THEN("new variable is added with coef 1 to expression") {
                        REQUIRE(res[b_var] == 1);
                    }AND_THEN("other expression variables are not affected") {
                        REQUIRE(res[a_var] == 1);
                    }
                }WHEN("subtracting new variable from expression") {
                    auto res = expr - b_var;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is not affected") {
                        REQUIRE(res.free_member() == 10);
                    }AND_THEN("new variable is added with coef -1 to expression") {
                        REQUIRE(res[b_var] == -1);
                    }AND_THEN("other expression variables are not affected") {
                        REQUIRE(res[a_var] == 1);
                    }
                }WHEN("subtracting expression from new variable") {
                    auto res = b_var - expr;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free member is invered") {
                        REQUIRE(res.free_member() == -10);
                    }AND_THEN("new variable is added with coef 1 to expression") {
                        REQUIRE(res[b_var] == 1);
                    }AND_THEN("other expression variables are inverted") {
                        REQUIRE(res[a_var] == -1);
                    }
                }
            }

            GIVEN("lhs and rhs expressions") {
                variable_t a_var = "a";
                variable_t b_var = "b";
                variable_t c_var = "c";

                auto lhs = a_var + 5 * b_var + 5;
                auto rhs = b_var - 4 * c_var - 1;

                WHEN("adding lhs and rhs") {
                    auto res = lhs + rhs;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free members are added") {
                        REQUIRE(res.free_member() == 4);
                    }AND_THEN("common variables are added") {
                        REQUIRE(res[b_var] == 6);
                    }AND_THEN("distinct variables stay the same") {
                        REQUIRE(res[a_var] == 1);
                        REQUIRE(res[c_var] == -4);
                    }
                }WHEN("subtracting rhs from lhs") {
                    auto res = lhs - rhs;
                    THEN("result is expression") {
                        REQUIRE(std::is_same_v<decltype(res), expression_t>);
                    }AND_THEN("free members are subtracted") {
                        REQUIRE(res.free_member() == 6);
                    }AND_THEN("common variables are subtracted") {
                        REQUIRE(res[b_var] == 4);
                    }AND_THEN("distinct variables stay the same") {
                        REQUIRE(res[a_var] == 1);
                        REQUIRE(res[c_var] == 4);
                    }
                }
            }
        }

        SECTION("to string test") {
            GIVEN("default constructed expression") {
                expression_t expr;
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a zero") {
                        REQUIRE(str == "0");
                    }
                }
            }GIVEN("expression constructed from const value") {
                expression_t expr = 5;
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a number") {
                        REQUIRE(str == "5");
                    }
                }
            }GIVEN("expression constructed from negative const value") {
                expression_t expr = -5;
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a number") {
                        REQUIRE(str == "-5");
                    }
                }
            }GIVEN("expression constructed from variable") {
                expression_t expr = variable_t{"a"};
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a variable name") {
                        REQUIRE(str == "a");
                    }
                }
            }GIVEN("expression constructed from negated variable") {
                expression_t expr = -variable_t{"a"};
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a variable name") {
                        REQUIRE(str == "-a");
                    }
                }
            }

            GIVEN("complex expression with no free member") {
                expression_t expr = variable_t{"a"} - 5 * variable_t{"b"};
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a variable name") {
                        REQUIRE(str == "a-5b");
                    }
                }
            }GIVEN("complex expression with no free member and negative first coef") {
                expression_t expr = -variable_t{"a"} + 5 * variable_t{"b"};
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a variable name") {
                        REQUIRE(str == "-a+5b");
                    }
                }
            }GIVEN("complex expression with ree member") {
                expression_t expr = variable_t{"a"} - 5 * variable_t{"b"} + 10;
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a variable name") {
                        REQUIRE(str == "a-5b+10");
                    }
                }
            }GIVEN("complex expression with free member and negative first coef") {
                expression_t expr = -variable_t{"a"} + 5 * variable_t{"b"} - 11;
                WHEN("to_string") {
                    auto str = expr.to_string();
                    THEN("it is just a variable name") {
                        REQUIRE(str == "-a+5b-11");
                    }
                }
            }
        }
    }
}

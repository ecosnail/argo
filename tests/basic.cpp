#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ecosnail/argo.hpp>

namespace argo = ecosnail::argo;

// TODO:
// * Implement '=' notation
// * Maybe use 'argument' instead of 'option' for options without flags

TEST_CASE("Command line description mistakes", "[checks]")
{
    std::ostringstream output;
    argo::output(output);

    SECTION("option flags overlap")
    {
        argo::message(argo::Message::OptionFlagOverlaps, "overlap");
        argo::option("-x");
        argo::option("-x");
        REQUIRE(output.str() == "overlap: -x\n");
    }

    SECTION("option flag overlaps with help flag")
    {
    }

    SECTION("all arguments have either flags or expected values")
    {
    }

    SECTION("warn about no-value arguments after a multi no-value argument")
    {
    }

    SECTION("required values have default value or are default-constructible")
    {
    }

    SECTION("subsequent parse calls")
    {
    }

    SECTION("all option flags can be converted to strings")
    {
    }
}

TEST_CASE("Command line argument mistakes")
{
    SECTION("non-multi argument used multiple times")
    {
        // which argument?
    }
}

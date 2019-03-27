#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ecosnail/argo.hpp>

namespace argo = ecosnail::argo;
namespace ex = argo::exception;

// TODO:
// * Implement '=' notation
// * Implement '-abc' notation
// * Maybe use 'argument' instead of 'option' for options without flags
// * Check option values with spaces (they are likely read with streams)
// * Check for bad values (the string provided as a value cannot be converted to
//   the option type)
//
// Static checks:
// * Option type is convertible to/from text ('cast' can be called)

TEST_CASE("Command line description mistakes", "[checks]")
{
    std::ostringstream output;
    argo::output(output);

    SECTION("an option flag is duplicated")
    {
        REQUIRE_THROWS_AS(argo::option("-x", "-x"), ex::FlagDuplicated);
    }

    //SECTION("option flags overlap")
    //{
    //    argo::option("-x");
    //    REQUIRE_THROWS_AS(argo::option("-x"), ex::FlagDuplicated);
    //}

    //SECTION("help option overlaps with previously set option")
    //{
    //    argo::helpOption("-h");
    //    REQUIRE_THROWS_AS(argo::option("-h"), ex::FlagDuplicated);
    //}

    //SECTION("option overlaps with previously set help option")
    //{
    //    argo::option("-h");
    //    REQUIRE_THROWS_AS(argo::helpOption("-h"), ex::FlagDuplicated);
    //}

    //SECTION("flag options must have keys at parse time")
    //{
    //    argo::option().flag();
    //    REQUIRE_THROWS_AS(argo::parse({}), ex::FlagDoesNotHaveKeys);
    //}

    //SECTION("no argument allowed after a multi argument")
    //{
    //    argo::option().multi();
    //    argo::option();
    //    REQUIRE_THROWS_AS(argo::parse({}), ex::ArgumentsAfterMultiArgument);
    //}

    //SECTION("required values have default value or are default-constructible")
    //{
    //    struct Data {
    //        Data(int) {}
    //    };

    //    argo::message(
    //        argo::Message::RequiredNotConstructible, "not constructible");
    //    argo::option<Data>("-x").required();
    //    argo::parse({});
    //    REQUIRE(output.str() == "not constructible: -x\n");
    //}

    //SECTION("subsequent parse calls")
    //{
    //    argo::parse({});
    //    REQUIRE_THROWS_AS(argo::parse({}), ex::SubsequentParseCall);
    //}

    //SECTION("all option flags can be converted to strings")
    //{
    //    struct Data {};

    //    argo::message(
    //        argo::Message::ValueNotConvertibleToString, "not convertible");
    //    argo::option<Data>("-x");
    //    REQUIRE(output.str() == "not convertible: -x\n");
    //}

    // Command line errors

    //SECTION("non-multi argument used multiple times")
    //{
    //    argo::message(argo::Message::NonMultiUsedMultipleTimes, "non multi");
    //    argo::option("-x").flag();
    //    bool success = argo::parse({"-x", "-x"});
    //    REQUIRE(output.str() == "non multi: -x\n");
    //    REQUIRE(!success);
    //}
}


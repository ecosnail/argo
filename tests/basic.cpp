#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ecosnail/argo.hpp>

namespace argo = ecosnail::argo;

// TODO:
// * Implement '=' notation
// * Maybe use 'argument' instead of 'option' for options without flags
//
// Static checks:
// * Option type is convertible to/from text ('cast' can be called)

TEST_CASE("Command line description mistakes", "[checks]")
{
    std::ostringstream output;
    argo::output(output);

    SECTION("an option flag is duplicated")
    {
        argo::message(argo::Message::OptionFlagDuplicated, "duplicated");
        argo::option("-x", "-x");
        REQUIRE(output.str() == "duplicated: -x\n");
    }

    SECTION("option flags overlap")
    {
        argo::message(argo::Message::OptionFlagsOverlap, "overlap");
        argo::option("-x");
        argo::option("-x");
        REQUIRE(output.str() == "overlap: -x\n");
    }

    SECTION("help option overlaps with previously set option")
    {
        argo::message(argo::Message::OptionFlagOverlapsHelp, "overlaps help");
        argo::helpOption("-h");
        argo::option("-h");
        REQUIRE(output.str() == "overlaps help: -h\n");
    }

    SECTION("option overlaps with previously set help option")
    {
        argo::message(argo::Message::OptionFlagOverlapsHelp, "overlaps help");
        argo::option("-h");
        argo::helpOption("-h");
        REQUIRE(output.str() == "overlaps help: -h\n");
    }

    SECTION("flag must have keys")
    {
        argo::message(argo::Message::FlagDoesNotHaveKeys, "no keys");
        argo::option().flag();
        argo::parse({});
        REQUIRE(output.str() == "no keys\n");
    }

    SECTION("warn about no-key arguments after a multi argument")
    {
        argo::message(argo::Message::MultiArgumentNotLast, "not last");
        argo::option().multi();
        argo::option();
        argo::parse({});
        REQUIRE(output.str() == "not last\n");
    }

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

    SECTION("subsequent parse calls")
    {
        argo::message(argo::Message::SubsequentParseCall, "subsequent");
        argo::parse({});
        argo::parse({});
        REQUIRE(output.str() == "subsequent\n");
    }

    //SECTION("all option flags can be converted to strings")
    //{
    //    struct Data {};

    //    argo::message(
    //        argo::Message::ValueNotConvertibleToString, "not convertible");
    //    argo::option<Data>("-x");
    //    REQUIRE(output.str() == "not convertible: -x\n");
    //}

    // Command line errors

    SECTION("non-multi argument used multiple times")
    {
        argo::message(argo::Message::NonMultiUsedMultipleTimes, "non multi");
        argo::option("-x").flag();
        bool success = argo::parse({"-x", "-x"});
        REQUIRE(output.str() == "non multi: -x\n");
        REQUIRE(!success);
    }
}


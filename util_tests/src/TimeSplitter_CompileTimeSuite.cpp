// TODO: use module instead of include
#include <TestUtilities.hpp>
#include <util/TimeSplitter.hpp>

namespace
{
    // Uses C++20 chrono features
    boost::ut::suite compileTimeSuite = []
    {
        // I personally think using namespace is a bad practice, however in boost::ut encourages it
        // It is also the accepted way to use C++ user-defined literals such as min, s, etc...
        using namespace boost::ut;
        using namespace util::test;
        using namespace std::chrono_literals;
        namespace sc = std::chrono;

        "ts_compile_time_simple_1"_test = []
        {
            static constexpr auto time = util::TimeSplitter<sc::seconds, sc::milliseconds>::split(2500ms);
            expect_eq(std::get<0>(time), 2s);
            expect_eq(std::get<1>(time), 500ms);
        };

        "ts_compile_time_simple_2"_test = []
        {
            static constexpr auto time = util::TimeSplitter<sc::hours, sc::minutes>::split(sc::days(2) + 125min);
            expect_eq(std::get<0>(time), 50h);
            expect_eq(std::get<1>(time), 5min);
        };

#if __cpp_lib_chrono >= 201907L
        "ts_3_days_passed"_test = []
        {
            static constexpr auto elapsed =
                    util::TimeSplitter<sc::days>::split(sc::sys_days(1993y / sc::October / 5d) - sc::sys_days(1993y / sc::October / 2d));
            expect_eq(std::get<0>(elapsed), sc::days{3});
        };

        "ts_compile_time_no_information_lost"_test = []
        {
            // Even though 66 seconds is 0 years, we can still extract minutes + seconds + milliseconds without losing information
            static constexpr auto time =
                    util::TimeSplitter<sc::years, sc::months, sc::weeks, sc::days, sc::hours, sc::minutes, sc::seconds, sc::milliseconds>::split(
                            66.4s);
            expect(std::get<sc::years>(time).count() == 0_l);
            expect_eq(std::get<sc::months>(time), sc::months{0});
            expect_eq(std::get<sc::weeks>(time), sc::weeks{0});
            expect_eq(std::get<sc::days>(time), sc::days{0});
            expect_eq(std::get<sc::minutes>(time), 1min);
            expect_eq(std::get<sc::seconds>(time), 6s);
            expect_eq(std::get<sc::milliseconds>(time), 400ms);
        };

        "ts_compile_time_ymd"_test = []
        {
            static constexpr auto hl2ReleaseDate{2004y / sc::November / 16d};
            static constexpr auto timeInDay = sc::sys_days(hl2ReleaseDate) + sc::hh_mm_ss(5h + 30min + 2s).to_duration();
            static constexpr auto time = util::TimeSplitter<sc::hours, sc::minutes, sc::seconds>::split(timeInDay - sc::sys_days(hl2ReleaseDate));
            // Test multiple representations of same time
            // 5 Hours
            expect_eq(std::get<0>(time), 5h);
            expect_eq(std::get<0>(time), 5 * 3600s);
            expect_eq(std::get<0>(time), 5 * 60min);
            // 30 Min
            expect_eq(std::get<1>(time), 1800s);
            expect_eq(std::get<1>(time), 30min);
            expect_eq(std::get<1>(time), 0.5h);
            // 2 Seconds
            expect_neq(std::get<2>(time), 0s);
            expect_neq(std::get<2>(time), 5min);
            expect_eq(std::get<2>(time), 2s);
            expect_eq(std::get<2>(time), 2'000ms);
            expect_eq(std::get<2>(time), 2'000'000us);
            expect_eq(std::get<2>(time), 2'000'000'000ns);
        };

        "ts_compile_time_leap_day"_test = []
        {
            static constexpr auto time =
                    util::TimeSplitter<sc::years, sc::hours>::split(sc::sys_days(2020y / sc::March / 1d) - sc::sys_days(2020y / sc::February / 28d));

            expect_eq(std::get<0>(time), sc::years(0));
            // 2 days passed, since Feburary 29 2020 occurred
            expect_eq(std::get<1>(time), 48h);
        };

        "ts_compile_time_no_leap_day"_test = []
        {
            static constexpr auto time =
                    util::TimeSplitter<sc::years, sc::hours>::split(sc::sys_days(2021y / sc::March / 1d) - sc::sys_days(2021y / sc::February / 28d));

            expect_eq(std::get<0>(time), sc::years(0));
            // 1 days passed, since Feburary 29 2020 did not occur
            expect_eq(std::get<1>(time), 24h);
        };

        "ts_compile_time_leap_day_bdd"_test = []
        {
            using namespace boost::ut::bdd;
            given("Feb 28 2020") = []
            {
                static constexpr auto feb28{2020y / sc::February / 28d};
                when("I add 2 days") = []
                {
                    static constexpr auto shouldBeMarch1st = sc::sys_days(feb28) + sc::days{2};
                    then("We get March 1st") = []
                    {
                        expect_eq(2020y / sc::March / 1d, shouldBeMarch1st);
                    };
                    then("2 days elapsed") = []
                    {
                        expect_eq(std::get<0>(util::TimeSplitter<sc::days>::split(shouldBeMarch1st - sc::sys_days(feb28))), sc::days{2});
                    };
                };
            };
        };

        "ts_compile_time_no_leap_day_bdd"_test = []
        {
            using namespace boost::ut::bdd;
            given("Feb 28 2021") = []
            {
                static constexpr auto feb28{2021y / sc::February / 28d};
                when("I add 2 days") = []
                {
                    static constexpr auto shouldBeMarch2nd = sc::sys_days(feb28) + sc::days{2};
                    then("We get March 2nd") = []
                    {
                        expect_eq(2021y / sc::March / 2d, shouldBeMarch2nd);
                    };
                    then("2 days elapsed") = []
                    {
                        expect_eq(std::get<0>(util::TimeSplitter<sc::days>::split(shouldBeMarch2nd - sc::sys_days(feb28))), sc::days{2});
                    };
                };
            };
        };

        "ts_compile_time_birthday_age_bdd"_test = []
        {
            using namespace boost::ut::bdd;

            given("You were born on October 5th 1993") = []
            {
                static constexpr auto birthday1993{1993y / sc::October / 5d};
                when("it was your 2nd birthday") = []
                {
                    static constexpr auto birthday1995{1995y / sc::October / 5d};
                    then("you were 2 years old on your birthday (no leap year in between)") = []
                    {
                        static constexpr auto twoYearsLater = sc::year_month_day(sc::floor<sc::days>(sc::sys_days(birthday1993) + sc::years{2}));
                        expect_eq(birthday1995, twoYearsLater);
                    };
                    then("exactly 1 year, 11months, 29 days elapsed") = []
                    {
                        static constexpr auto twoYearsElapsedInDays =
                                util::TimeSplitter<sc::days>::split(sc::sys_days(birthday1995) - sc::sys_days(birthday1993));
                        expect_eq(std::get<sc::days>(twoYearsElapsedInDays), sc::days{730});
                    };
                };
            };
        };
#endif// __cpp_lib_chrono >= 201907L
    };
}// namespace

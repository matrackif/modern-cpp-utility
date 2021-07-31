// TODO: use module instead of include
#include <boost/ut.hpp>
#include <util/TimeSplitter.hpp>

namespace
{
    boost::ut::suite compileTimeSuite = []
    {
        using namespace boost::ut;
        using namespace std::chrono_literals;
        namespace sc = std::chrono;

        "ts_compile_time_1"_test = []
        {
            static constexpr auto time = util::TimeSplitter<sc::seconds, sc::milliseconds>::split(2500ms);
            expect(constant<std::get<0>(time) == 2s>);
            expect(constant<std::get<1>(time) == 500ms>);
        };

        "ts_compile_time_2"_test = []
        {
            // No literal for std::chrono::days, but there is one for std::chrono::day, which is not a std::chrono::duration. Unfortunate
            static constexpr auto time = util::TimeSplitter<sc::hours, sc::minutes>::split(sc::days(2) + 125min);
            expect(constant<std::get<0>(time) == 50h>);
            expect(constant<std::get<1>(time) == 5min>);
        };

        "ts_compile_time_3_ymd"_test = []
        {
            // Use C++20 chrono
            static constexpr auto hl2ReleaseDate{2004y / sc::November / 16d};
            static constexpr auto timeInDay = sc::sys_days(hl2ReleaseDate) + sc::hh_mm_ss(5h + 30min + 2s).to_duration();
            static constexpr auto time = util::TimeSplitter<sc::hours, sc::minutes, sc::seconds>::split(timeInDay - sc::sys_days(hl2ReleaseDate));
            expect(constant<std::get<0>(time) == 5h>);
            expect(constant<std::get<1>(time) == 30min>);
            expect(constant<std::get<2>(time) == 2s>);
        };
    };
}

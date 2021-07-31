//
// Created by Filip Matracki
//

#ifndef TIMESPLITTER_HPP
#define TIMESPLITTER_HPP

#include <tuple>
#include <chrono>

namespace util
{
    namespace detail
    {
        template<typename Duration>
        struct is_duration : std::false_type
        {
        };

        template<typename Rep, typename Period>
        struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type
        {
        };

        template<typename Duration1, typename Duration2>
        constexpr inline bool is_less_precise_v = std::ratio_greater<typename Duration1::period, typename Duration2::period>::value;

    }// namespace detail

    template<typename... Durations>
    class TimeSplitter
    {
        static_assert(std::conjunction_v<detail::is_duration<Durations>...>, "All types must be of type std::chrono::duration");

        using SplitTimeType = std::tuple<Durations...>;
        using MaxPrecisionType = std::chrono::nanoseconds;

        static constexpr std::size_t N = std::tuple_size<SplitTimeType>::value;
        static_assert(N > 0, "Must provide at least one template argument to TimeSplitter");

        template<std::size_t I = 0, typename Rep, typename Period>
        static constexpr void splitTimeImpl(std::chrono::duration<Rep, Period> dur, SplitTimeType &t)
        {
            using Duration1 = typename std::tuple_element<I, SplitTimeType>::type;
            std::get<I>(t) = std::chrono::duration_cast<Duration1>(dur);

            if constexpr(I + 1 < N)
            {
                using Duration2 = typename std::tuple_element<I + 1, SplitTimeType>::type;
                static_assert(detail::is_less_precise_v<Duration1, Duration2>, "Values must be in order of least precise to most precise");
                dur -= std::get<I>(t);
                splitTimeImpl<I + 1>(dur, t);
            }
        }

    public:
        /// Splits a std::chrono::duration into smaller (less precise) duration types
        /// Maximum supported precision is TimeSplitter::MaxPrecisionType
        /// \tparam Rep
        /// \tparam Period
        /// \param dur duration of time to be split into smaller durations
        /// \return std::tuple<Durations...>, where each element in the tuple is filled with the duration after splitting
        /// Example usage:
        /// <pre>
        ///    namespace sc = std::chrono;
        ///    using namespace std::chrono_literals;
        ///    static constexpr auto ret = TimeSplitter<sc::hours, sc::seconds, sc::milliseconds>::split(1h + 5min + 2s + 100ms);
        ///    // ret is of type std::tuple<sc::hours, sc::seconds, sc::milliseconds>
        ///    // ret contains the following values: [1, 302, 100]
        /// </pre>
        template<typename Rep, typename Period>
        static constexpr SplitTimeType split(std::chrono::duration<Rep, Period> dur)
        {
            SplitTimeType ret;
            splitTimeImpl(std::chrono::duration_cast<MaxPrecisionType>(dur), ret);
            return ret;
        }
    };
}// namespace util

#endif//TIMESPLITTER_HPP

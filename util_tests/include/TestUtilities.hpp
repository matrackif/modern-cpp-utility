#ifndef TEST_UTILITIES_HPP
#define TEST_UTILITIES_HPP

#include <boost/ut.hpp>

namespace util::test
{
    template<class TLhs, class TRhs>
    constexpr auto expect_eq(const TLhs &lhs, const TRhs &rhs,
                             const boost::ut::reflection::source_location &sl = boost::ut::reflection::source_location::current())
    {
        return boost::ut::expect(boost::ut::eq(lhs, rhs), sl);
    }

    template<class TLhs, class TRhs>
    constexpr auto expect_neq(const TLhs &lhs, const TRhs &rhs,
                              const boost::ut::reflection::source_location &sl = boost::ut::reflection::source_location::current())
    {
        return boost::ut::expect(boost::ut::neq(lhs, rhs), sl);
    }
}// namespace util::test

#endif// #ifndef TEST_UTILITIES_HPP

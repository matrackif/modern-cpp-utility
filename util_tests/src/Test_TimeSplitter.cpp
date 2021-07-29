// TODO: use module instead of include
#include <boost/ut.hpp>
#include <util/TimeSplitter.hpp>


int main()
{
	using namespace boost::ut;
	using namespace std::chrono_literals;
	namespace sc = std::chrono;
	static constexpr auto time = util::TimeSplitter<sc::seconds, sc::milliseconds>::split(2500ms);
	static_assert(std::get<0>(time) == 2s);
	static_assert(std::get<1>(time) == 500ms);

	"split_hours_minutes"_test = []
	{
		// No literal for std::chrono::days, but there is one for std::chrono::day, which is not a std::chrono::duration. Unfortunate
		const auto [hours, minutes] = util::TimeSplitter<sc::hours, sc::minutes>::split(std::chrono::days(2) + 125min);
		expect(hours.count() == 50_l && minutes.count() == 5_l);
	};

	return 0;
}
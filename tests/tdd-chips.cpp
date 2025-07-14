#include <q1x/test/test.h>
#include <q1x/runtime/config.h>
#include <q1x/proto/data.h>

TEST_CASE("load-trans", "[chips]") {

}

TEST_CASE("load-chips", "[chips]") {
    std::string securityCode = "sh510050";
    std::string factor_date = "2025-07-11";
    auto ofn = config::get_chip_distribution_filename(securityCode, factor_date);
    std::cout << ofn << std::endl;
    data::Chips chips{};
    std::ifstream is(ofn, std::ios::binary);
    bool ok = chips.ParseFromIstream(&is);
    std::cout << ok << std::endl;
    auto date = chips.date();
    std::cout << date << std::endl;
    std::cout << chips.dist().size() << std::endl;
    f64 vol = 0;
    for (auto & pl : chips.dist()) {
        std::cout << pl.DebugString() << std::endl;
        vol+= (pl.sell() + pl.buy());
    }
    std::cout << vol << std::endl;
}
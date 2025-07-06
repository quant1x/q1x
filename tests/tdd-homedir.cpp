#include <q1x/test/test.h>
#include <q1x/std/util.h>

TEST_CASE("homedir", "[io]") {
    auto homedir = util::homedir();
    std::cout << homedir << std::endl;
}
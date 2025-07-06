#include <api/test/test.h>
#include "api/util.h"

TEST_CASE("homedir", "[io]") {
    auto homedir = util::homedir();
    std::cout << homedir << std::endl;
}
#include "gtest-1.8.1/gtest.h"
#include "../src/tablero.h"

TEST(tablero_test, nuevo_tablero_test) {
    tablero t(10);
    EXPECT_EQ(t.tamano(), 10);
    }
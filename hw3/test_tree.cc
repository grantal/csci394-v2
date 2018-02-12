#define CATCH_CONFIG_MAIN
/*
 * Tree class unit tests
 * Written by Eitan Frachtenberg.
 */

#include "catch.hpp"
#include "tree.hh"
#include "vectree.hh"

using TreeType = tree::VecTree;  // Change this to test other subclasses of Tree

TEST_CASE("size is computed correctly", "[vectree]") {
    const auto tree1 = new TreeType(15);

    SECTION("leaf case") {
        REQUIRE(tree1->size() == 1);
    }

    SECTION("compound tree") {
        const auto tree2 = new TreeType(10);
        const auto tree3 = new TreeType(7, *tree1, *tree2);
        REQUIRE(tree2->size() == 1);
        REQUIRE(tree3->size() == 3);

        delete tree3;
        delete tree2;
    }

    delete tree1;
}


TEST_CASE("Path is computed correctly", "[vectree]") {
    const auto tree1 = new TreeType(15);
    const auto tree2 = new TreeType(10);
    const auto tree3 = new TreeType(7, *tree1, *tree2);

    REQUIRE(tree3->pathTo(15) == "L");
    REQUIRE_THROWS(tree3->pathTo(2));
    REQUIRE(tree3->pathTo(10) == "R");
    REQUIRE(tree3->pathTo(7) == "");

    delete tree3;
    delete tree2;
    delete tree1;
}

TEST_CASE("Correct Node is found for given Path", "[vectree]") {
    const auto tree1 = new TreeType(21);
    const auto tree2 = new TreeType(13);
    const auto tree3 = new TreeType(22, *tree1, *tree2);

    SECTION("tree2 should be on the right of tree3") {
        REQUIRE(tree3->getByPath("R") == 13);
    }
    SECTION("there are no paths coming from a tree with no leaves") {
        REQUIRE_THROWS(tree1->getByPath("L"));
        REQUIRE_THROWS(tree1->getByPath("R"));
    }
    SECTION("No path should return the root") {
        REQUIRE(tree1->getByPath("") == 21);
    }
    SECTION("can't go too deep") {
        REQUIRE_THROWS(tree3->getByPath("RR"));
    }

    delete tree3;
    delete tree2;
    delete tree1;
}

TEST_CASE("getByPath works for more complex paths", "[vectree]") {
    const auto tree1 = new TreeType(1);
    const auto tree2 = new TreeType(2);
    const auto tree3 = new TreeType(3, *tree1, *tree2);
    const auto tree4 = new TreeType(4);
    const auto tree5 = new TreeType(5);
    const auto tree6 = new TreeType(6, *tree4, *tree5);
    const auto tree7 = new TreeType(7, *tree3, *tree6);

    REQUIRE(tree7->getByPath("LL") == 1);
    REQUIRE(tree7->getByPath("LR") == 2);
    REQUIRE(tree7->getByPath("L")  == 3);
    REQUIRE(tree7->getByPath("RL") == 4);
    REQUIRE(tree7->getByPath("RR") == 5);
    REQUIRE(tree7->getByPath("R")  == 6);
    REQUIRE(tree7->getByPath("")   == 7);

    delete tree7;
    delete tree6;
    delete tree5;
    delete tree4;
    delete tree3;
    delete tree2;
    delete tree1;
}

#define CATCH_CONFIG_MAIN
/*
 * Tree class unit tests
 * Written by Eitan Frachtenberg.
 */

#include "catch.hpp"
#include "tree.hh"
#include "ptrtree.hh"

using TreeType = tree::PtrTree;  // Change this to test other subclasses of Tree

TEST_CASE("size is computed correctly", "[ptrtree]") {
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


TEST_CASE("Path is computed correctly", "[ptrtree]") {
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

TEST_CASE("Correct Node is found for given Path", "[ptrtree]") {
    const auto tree1 = new TreeType(21);
    const auto tree2 = new TreeType(13);
    const auto tree3 = new TreeType(22, *tree1, *tree2);

    SECTION("tree2 should be on the right of tree3") {
        REQUIRE(tree3->getByPath("R") == 13);
    }
    SECTION("there are no paths coming from a tree with no leaves") {
        REQUIRE(tree1->getByPath("L") == 21);
        REQUIRE(tree1->getByPath("R") == 21);
        REQUIRE(tree1->getByPath("LRLRL") == 21);
    }
    SECTION("No path should return the root") {
        REQUIRE(tree1->getByPath("") == 21);
    }
    SECTION("When given a path to long, it returns the last valid node") {
        REQUIRE(tree3->getByPath("RR") == 13);
        REQUIRE(tree3->getByPath("RL") == 13);
        REQUIRE(tree3->getByPath("RRRLRLLRL") == 13);
        REQUIRE(tree3->getByPath("LL") == 21);
        REQUIRE(tree3->getByPath("LR") == 21);
        REQUIRE(tree3->getByPath("LR") == 21);
    }

    delete tree3;
    delete tree2;
    delete tree1;
}

TEST_CASE("getByPath works for more complex paths", "[ptrtree]") {
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


TEST_CASE("unbalanced trees work", "[ptrtree]") {
    const auto tree1 = new TreeType(1);
    const auto tree2 = new TreeType(2);
    tree2->setLeft(*tree1);
    const auto tree3 = new TreeType(3);
    tree3->setRight(*tree2);

    SECTION("tree1 should be on the left of tree2") {
        REQUIRE(tree2->getByPath("L") == 1);
        REQUIRE(tree2->pathTo(1) == "L");
    }
    SECTION("tree1 should be left then right from tree3") {
        REQUIRE(tree3->getByPath("RL") == 1);
        REQUIRE(tree3->pathTo(1) == "RL");
    }
    SECTION("error messages are thrown on invalid input"){
        REQUIRE_THROWS(tree3->pathTo(12));
    }
    SECTION("getbypath returns last valid node in path"){
        REQUIRE(tree3->getByPath("L") == 3);
        REQUIRE(tree3->getByPath("RR") == 2);
        REQUIRE(tree2->getByPath("R") == 2);
    }
    delete tree3;
    delete tree2;
    delete tree1;
}

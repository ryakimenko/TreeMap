#include <algorithm>
#include <gtest/gtest.h>
#include <initializer_list>
#include <treemap/treemap.h>

TEST(TreeMap, insertTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    tree.insert({2, 2});
    tree.insert({1, 1});
    for (size_t i = 0; i < tree.size(); i++) {
        ASSERT_EQ(i + 1, tree[i + 1]); // NOLINT
    }
}

TEST(TreeMap, eraseTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    tree.insert({2, 2});
    tree.insert({1, 1});
    tree.erase(1);
    tree.erase(2);
    tree.erase(3);
    ASSERT_EQ(true, tree.empty()); // NOLINT
}

TEST(TreeMap, countTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    tree.insert({2, 2});
    tree.insert({1, 1});
    ASSERT_EQ(1, tree.count(3)); // NOLINT
}

TEST(TreeMap, containsTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    ASSERT_EQ(true, tree.contains(3)); // NOLINT
}

TEST(TreeMap, sizeTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    ASSERT_EQ(1, tree.size()); // NOLINT
}

TEST(TreeMap, equalTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    tree.insert({2, 2});
    tree.insert({1, 1});
    libcsc::TreeMap<int, int> tree2(tree);
    ASSERT_EQ(true, tree == tree2); // NOLINT
}

TEST(TreeMap, moveTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    tree.insert({2, 2});
    tree.insert({1, 1});
    libcsc::TreeMap<int, int> tree2(std::move(tree));
    ASSERT_EQ(false, tree == tree2); // NOLINT
    ASSERT_EQ(3, tree2.size());      // NOLINT
    ASSERT_EQ(0, tree.size());       // NOLINT
}

TEST(TreeMap, stlTest)
{
    libcsc::TreeMap<int, int> tree;
    tree.insert({3, 3});
    tree.insert({2, 2});
    tree.insert({1, 1});
    std::pair<const int, int> pair = std::make_pair(3, 3);
    auto it = std::find(tree.begin(), tree.end(), pair);
    ASSERT_EQ(3, it->first);  // NOLINT
    ASSERT_EQ(3, it->second); // NOLINT
}

TEST(TreeMap, operatorTest)
{
    libcsc::TreeMap<int, int> tree;

    tree[4] = 4;

    std::pair<const int, int> pair = std::make_pair(4, 4);
    auto it = std::find(tree.begin(), tree.end(), pair);
    ASSERT_EQ(4, it->first);  // NOLINT
    ASSERT_EQ(4, it->second); // NOLINT
}

TEST(TreeMap, atTest)
{
    libcsc::TreeMap<int, int> tree;

    tree[4] = 4;

    ASSERT_EQ(4, tree.at(4)); // NOLINT
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

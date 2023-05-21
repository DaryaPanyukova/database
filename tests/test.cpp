#include <gtest/gtest.h>
#include <lib/database.h>

TEST(TestSuite, Test1) {
    ASSERT_EQ(Sum(3, 5), 8);

    /*
    Database a;
    a.AddBase("students", "students.tsv");
    a.Request("какой-то код на sql");
    std::cout << a;
    a.Save("filename.tsv");
     */
}

#include "shared_ptr.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>
#include <array>

class MyClass
{
  public:
    MyClass(std::string name) :
        m_name(name)
    {
    }

    void updateName(std::string name)
    {
        m_name = name;
    }

    std::string m_name;
};

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

template <typename T>
T testCopyMoveConstructors(T p)
{
    EXPECT_GT(p.use_count(), 1);
    return p;
}

template <typename T>
void testPrimitives()
{
    std::shared_ptr<T> p1 = std::make_shared<T>();
    usu::shared_ptr<T> p2 = usu::make_shared<T>();

    EXPECT_EQ(p1.use_count(), p2.use_count());

    // Create a new scope to validate incrementing, decrementing reference counts
    {
        auto p1b = p1;
        auto p2b = p2;

        EXPECT_EQ(p1.use_count(), p2.use_count());
    }

    EXPECT_EQ(p1.use_count(), p2.use_count());
}

TEST(Primitives, DefaultConstruction)
{
    testPrimitives<int>();
    testPrimitives<unsigned int>();
    testPrimitives<bool>();
    testPrimitives<double>();
}

TEST(Primitives, OverloadedConstructors)
{
    std::shared_ptr<int> p1 = std::make_shared<int>(10);
    usu::shared_ptr<int> p2 = usu::make_shared<int>(10);

    EXPECT_EQ(p1.use_count(), p2.use_count());
    EXPECT_EQ(*p1, *p2);
}

TEST(Primitives, CanReuse)
{
    std::shared_ptr<int> p1 = std::make_shared<int>();
    usu::shared_ptr<int> p2 = usu::make_shared<int>();

    p1 = std::make_shared<int>(10);
    p2 = usu::make_shared<int>(10);

    EXPECT_EQ(p1.use_count(), p2.use_count());
    EXPECT_EQ(*p1, *p2);
}

TEST(Primitives, CopyMoveOperations)
{
    std::shared_ptr<int> p1 = std::make_shared<int>();
    usu::shared_ptr<int> p2 = usu::make_shared<int>();

    p1 = testCopyMoveConstructors(p1);
    p2 = testCopyMoveConstructors(p2);

    EXPECT_EQ(p1.use_count(), p2.use_count());
}

TEST(ComplexTypes, CanCreateComplexTypes)
{
    std::shared_ptr<std::string> p1 = std::make_shared<std::string>();
    usu::shared_ptr<std::string> p2 = usu::make_shared<std::string>();

    EXPECT_EQ(p1.use_count(), p2.use_count());
    EXPECT_EQ(*p1, *p2);
}

TEST(ComplexTypes, OverloadedConstructors)
{
    std::shared_ptr<std::string> p1 = std::make_shared<std::string>("This is a test");
    usu::shared_ptr<std::string> p2 = usu::make_shared<std::string>("This is a test");

    EXPECT_EQ(p1.use_count(), p2.use_count());
    EXPECT_EQ(*p1, *p2);
}

TEST(ComplexTypes, PointerOperator)
{
    auto p1 = std::make_shared<MyClass>("Some Name");
    auto p2 = usu::make_shared<MyClass>("Some Name");

    EXPECT_EQ(p1.use_count(), p2.use_count());
    EXPECT_EQ(p1->m_name, p2->m_name);

    p1->updateName("Updated Name");
    p2->updateName("Updated Name");
    EXPECT_EQ(p1->m_name, p2->m_name);
}

TEST(Array, CanCreateArray)
{
    std::array<int, 4> truth{2, 3, 5, 7};
    usu::shared_ptr<int[]> primes = usu::make_shared_array<int, 4>();
    primes[0] = truth[0];
    primes[1] = truth[1];
    primes[2] = truth[2];
    primes[3] = truth[3];

    for (unsigned int i = 0; i < primes.size(); i++)
    {
        EXPECT_EQ(primes[i], truth[i]);
    }
}

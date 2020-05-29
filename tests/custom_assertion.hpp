#ifndef CUSTOM_ASSERTION_HPP
#define CUSTOM_ASSERTION_HPP

#include <cassert>

using namespace std;

template <class T>
void ASSERT_EQUAL(T a, T b)
{
    assert(a == b);
}

template <class T>
void ASSERT_NOT_EQUAL(T a, T b)
{
    assert(a != b);
}

#endif //CUSTOM_ASSERTION_HPP
//
// Created by Glyhux on 2021/7/26.
//
#define BOOST_TEST_MODULE boost_unit_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( my_test )
{
    BOOST_CHECK_EQUAL(1, 1);
}
//
// Created by Glyhux on 2021/7/26.
//
#define BOOST_TEST_MODULE boost_unit_test
#include <boost/test/included/unit_test.hpp>


#include <iostream>
using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(free_test_function)
{
    BOOST_CHECK( true /* test assertion */ );
}
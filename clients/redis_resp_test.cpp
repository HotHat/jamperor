//
// Created by admin on 2021/7/28.
//

#define BOOST_TEST_MODULE redis_resp_test
#include <boost/test/included/unit_test.hpp>
#include "redis_resp.h"

using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(test_simple_string)
{
    std::shared_ptr<RespSimpleString> simple_string(new RespSimpleString("OK"));
    std::shared_ptr<RespSimpleString> simple_string2(new RespSimpleString("Operation against a key holding the wrong kind of value"));

    std::cout << simple_string->ToString() << std::endl;
    std::cout << simple_string2->Error("WRONGTYPE") << std::endl;

    RespBulkString bulk_string("foobar");
    std::cout << bulk_string.ToString() << std::endl;

    std::cout << RespBulkString::Empty().ToString() << std::endl;
    std::cout << RespBulkString::Nil().ToString() << std::endl;

    RespInteger integer("123456");
    RespInteger integer2(123456);
    std::cout << integer.ToString() << std::endl;
    std::cout << integer2.ToString() << std::endl;


    std::cout << "array test" << std::endl;
    RespArray resp_array;

    resp_array.Add(simple_string);
    resp_array.Add(simple_string2);

    std::cout << resp_array.ToString() << std::endl;

    BOOST_CHECK(true);
}
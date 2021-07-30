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

   // std::cout << simple_string->ToString() << std::endl;
   // std::cout << simple_string2->Error("WRONGTYPE") << std::endl;

    RespBulkString bulk_string("foobar");
    std::cout << bulk_string.ToString() << std::endl;

    // std::cout << RespBulkString::Empty().ToString() << std::endl;
    // std::cout << RespBulkString::Null().ToString() << std::endl;

    RespInteger integer("123456");
    RespInteger integer2(123456);
    std::cout << integer.ToString() << std::endl;
    std::cout << integer2.ToString() << std::endl;

//    std::shared_ptr<RespArray> resp_array_inner(new RespArray());
//    resp_array_inner->Add(simple_string);
//    resp_array_inner->Add(simple_string2);

//    std::cout << "array test" << std::endl;
//    RespArray resp_array;

//    resp_array.Add(simple_string);
//    resp_array.Add(simple_string2);
//    resp_array.Add(resp_array_inner);

//    std::cout << resp_array.ToString() << std::endl;
//    std::cout << RespArray::Null().ToString() << std::endl;


    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_parse_string)
{
    // auto opt = redis_resp_parse(std::string("+OK\r\n-ERR unknown command 'foobar'\r\n:1000\r\n$"));
    // if (opt) {
    //     std::cout << opt.value()->ToString() << std::endl;
    // }


    auto opt = redis_resp_parse(std::string("+This is simple string\r\n-Error Message\r\n:12345\r\n$-1\r\n$5\r\n12345\r\n"));
    if (opt) {
        std::cout << opt.value()->ToString() << std::endl;
    }

    // auto opt1 = redis_resp_parse(std::string("-ERR unknown command 'foobar'\r\n"));
    // if (opt) {
    //     std::cout << opt1.value()->ToString() << std::endl;
    // }
    //
    // auto opt2 = redis_resp_parse(std::string(":1000\r\n"));
    // if (opt) {
    //     std::cout << opt2.value()->ToString() << std::endl;
    // }

    BOOST_CHECK(true);
}

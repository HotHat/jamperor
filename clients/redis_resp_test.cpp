//
// Created by admin on 2021/7/28.
//

#define BOOST_TEST_MODULE redis_resp_test
#include <boost/test/included/unit_test.hpp>
#include "redis_resp.h"

using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(test_simple_string)
{
    RespArray arr;
    auto auth = new RespBulkString("auth");
    auto password = new RespBulkString("NtoceahobN8287281Nre2");
    arr.Add(auth);
    arr.Add(password);

    std::cout << arr.ToString() << std::endl;

    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_parse_string)
{
    RedisResp  resp, resp1, resp2;
    auto simple_str = resp.parse(std::string("*2\r\n"
                                             "$4\r\n"
                                             "4096\r\n"
                                             "*6\r\n"
                                             "$41\r\n"
                                             "qytrelease_cache:manufacturer_data.id=632\r\n"
                                             "$39\r\n"
                                             "qytrelease_cache:goods_region_id_776345\r\n"
                                             "$50\r\n"
                                             "qytceshi_cache:api_oauth_cache_keyzsyt_sync_real_1\r\n"
                                             "$39\r\n"
                                             "qytrelease_cache:dosage_form_data.id=67\r\n"
                                             "$39\r\n"
                                             "qytrelease_cache:goods_region_id_515018\r\n"
                                             "$38\r\n"
                                             "qytrelease_cache:goods_region_id_59574\r\n"));
    if (simple_str) {
        std::cout << simple_str.value()->ToString() << std::endl;
    } else {
        std::cout << "simple string parse error" << std::endl;
    }

    // auto simple_str2 = resp.parse(std::string("\r\n+PONG\r\n"));
    // if (simple_str2) {
    //     std::cout << simple_str2.value()->ToString() << std::endl;
    // } else {
    //     std::cout << "simple string parse error" << std::endl;
    // }




    // auto error_str = resp1.parse(std::string("-ERR unknown command 'foobar'\r\n"));
    // if (error_str) {
    //     std::cout << error_str.value()->ToString() << std::endl;
    // } else {
    //     std::cout << "error string parse error" << std::endl;
    // }
    //
    // auto integer_str = resp2.parse(std::string(":888123\r\n"));
    // if (integer_str) {
    //     std::cout << integer_str.value()->ToString() << std::endl;
    // } else {
    //     std::cout << "integer parse error" << std::endl;
    // }
    //
    // std::cout << "run the end" << std::endl;
    // auto bulk_str = resp.parse(std::string("$8\r\n12345678\r\n"));
    // if (bulk_str) {
    //     std::cout << bulk_str.value()->ToString() << std::endl;
    // } else {
    //     std::cout << "bulk string parse error" << std::endl;
    // }

    // auto opt = redis_resp_parse(std::string("+OK\r\n-ERR unknown command 'foobar'\r\n:1000\r\n$"));
    // if (opt) {
    //     std::cout << opt.value()->ToString() << std::endl;
    // }


    // auto opt = redis_resp_parse(std::string("+This is simple string\r\n-Error Message\r\n:12345\r\n$-1\r\n$5\r\n12345\r\n"));
    // if (opt) {
    //     std::cout << opt.value()->ToString() << std::endl;
    // }

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

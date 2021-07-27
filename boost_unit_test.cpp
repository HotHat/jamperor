//
// Created by Glyhux on 2021/7/26.
//
#define BOOST_TEST_MODULE boost_unit_test
// import for log
#define BOOST_LOG_DYN_LINK 1
#include <boost/test/included/unit_test.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <iostream>
using namespace boost::unit_test;
namespace logging = boost::log;

void init()
{
    logging::add_file_log("sample.log");

    logging::core::get()->set_filter
            (
                    logging::trivial::severity >= logging::trivial::info
            );
}

BOOST_AUTO_TEST_CASE(free_test_function)
{
    init();
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(trace) << "An informational severity message";
    BOOST_LOG_TRIVIAL(fatal) << "An informational severity message";
    BOOST_CHECK( true /* test assertion */ );
}
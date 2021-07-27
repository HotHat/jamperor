//
// Created by admin on 2021/7/27.
//
#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


class Logger {
public:
    Logger() {
        namespace logging = boost::log;
        namespace keywords = boost::log::keywords;
        namespace attrs = boost::log::attributes;

        logging::add_file_log(
                keywords::file_name = "epoll-sample.log",
                keywords::auto_flush = true,
                keywords::open_mode = (std::ios::out | std::ios::app),
                keywords::format = "%TimeStamp% [%Uptime%] (%LineID%) <%Severity%>: %Message%"
                );

        logging::add_common_attributes();
//        logging::core::get()->add_global_attribute("Uptime", attrs::timer());

        logging::core::get()->set_filter
                (
                        logging::trivial::severity >= logging::trivial::info
                );
    }

    void info(std::string &message) {
        BOOST_LOG_TRIVIAL(info) << message;
    }

    void error(std::string &message) {
        BOOST_LOG_TRIVIAL(fatal) << message;
    }

    void debug(std::string &message) {
        BOOST_LOG_TRIVIAL(debug) << message;
    }
};
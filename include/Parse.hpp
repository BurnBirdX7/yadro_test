#pragma once
#include <stdexcept>

#include "Time.hpp"
#include "Model.hpp"


namespace club {

    struct PrintLineError : public std::runtime_error {
        explicit PrintLineError(std::string const& msg) : std::runtime_error(msg) {}
    };

    class Parse {
    public:
        using queue_t = Model::queue_t;

        /**
         * Parses header of the file
         * <table_count>
         * <start_time> <end_time>
         * <hourly_price>
         * @param in stream to parse
         * @return tuple: (table_count, start_time, end_time, hourly_price)
         * @throws PrintLineError thrown when error occurs during parsing, content must be printed
         */
        static std::tuple<int, Time, Time, int> header(std::istream& in);

        /**
         * Parses event queue
         * @param in stream to parse
         * @return queue
         * @throws PrintLineError thrown when error occurs during parsing, content must be printed
         */
        static queue_t events(std::istream& in);

    private:
        virtual ~Parse() = 0;
    };

} // club

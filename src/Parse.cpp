#include "Parse.hpp"
#include <string>
#include <iostream>
#include <sstream>

using namespace club;


std::tuple<int, Time, Time, int> Parse::header(std::istream& in) {
    std::string count_str, time_str, price_str;
    std::getline(in, count_str);
    std::getline(in, time_str);
    std::getline(in, price_str);

    int table_count, price;
    Time start_time{}, end_time{};

    std::string current_line;
    // Count and price
    try {
        // Table count
        current_line = count_str;
        table_count = std::stoi(count_str);

        // Start and End time
        current_line = time_str;
        std::istringstream ss(time_str);
        std::string start_str, end_str;
        ss >> start_str >> end_str;

        start_time = Time(start_str);
        end_time = Time(end_str);

        // Price
        current_line = price_str;
        price = std::stoi(price_str);
    } catch (std::runtime_error const&) {
        throw PrintLineError(current_line);
    } catch (std::invalid_argument const&) {
        throw PrintLineError(current_line);
    }

    return {table_count, start_time, end_time, price};
}


Parse::queue_t Parse::events(std::istream& in) {
    queue_t events{};
    Time lastTime{}; // Mark to track that time of incoming events is in ascending order

    while (!in.eof()) {
        std::string line;
        std::getline(in, line);
        try { // Catches low(er)-level parsing errors, system_error
            Event event = Event::from_string(line);
            if (event.time() < lastTime) {
                throw PrintLineError(line);
            }
            events.push_back(event);

            lastTime = event.time();
        } catch (std::system_error const& error) {
            throw PrintLineError(line);
        }
    }

    return events;
}

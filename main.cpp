#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <sstream>
#include <tuple>
#include "Event.hpp"
#include "Model.hpp"


void printHelp() {
    std::cout << "Usage: task <input_file>";
}

using namespace club;

struct PrintLineError : public std::runtime_error {
    explicit PrintLineError(std::string const& msg) : std::runtime_error(msg) {}
};

/**
 * Parses header of the file
 * <table_count>
 * <start_time> <end_time>
 * <hourly_price>
 * @param in stream to parse
 * @return tuple: (table_count, start_time, end_time, hourly_price)
 * @throws PrintLineError thrown when error occurs during parsing, content must be printed
 */
std::tuple<int, Time, Time, int> parse_header(std::istream& in) {
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
    }

    return {table_count, start_time, end_time, price};
}

/**
 * Parses event queue
 * @param in stream to parse
 * @return queue
 * @throws PrintLineError thrown when error occurs during parsing, content must be printed
 */
Model::queue_t parse_events(std::istream& in) {
    Model::queue_t events{};
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

int main(int argc, char** argv) {
    if (argc != 2) {
        printHelp();
        return 0;
    }

    auto fin = std::ifstream(argv[1]);

    if (!fin) {
        std::cerr << "Cannot open file";
        return 0;
    }

    try { // Catches parsing errors, PrintLineError
        auto [table_count, start_time, end_time, price] = parse_header(fin);
        auto incoming_events = parse_events(fin);

        auto model = Model(table_count, start_time, end_time, price, std::move(incoming_events));
        model.run();

    } catch (PrintLineError& error) {
        std::cout << error.what() << '\n';
        return 0;
    } catch (std::runtime_error& error) {
        // ignore ...
    }

}

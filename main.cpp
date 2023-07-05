#include <iostream>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <deque>
#include <queue>
#include <fstream>
#include <sstream>
#include "Event.hpp"
#include "Model.hpp"


void printHelp() {
    std::cout << "Usage: task <input_file>";
}

using namespace club;

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

    std::string count_str, time_str, price_str;

    std::getline(fin, count_str);
    std::getline(fin, time_str);
    std::getline(fin, price_str);

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
        std::cout << current_line;
        return 0;
    }

    Model::queue_t incoming_events{};
    Time lastTime{};
    while (!fin.eof()) {
        std::string line;
        std::getline(fin, line);
        try {
            Event event = Event::from_string(line);
            if (event.time() < lastTime) {
                throw std::runtime_error("Wrong time order");
            }
            incoming_events.push_back(event);

            lastTime = event.time();
        } catch (std::runtime_error const& error) {
            std::cout << line << ": " << error.what();
            return 0;
        }
    }

    auto model = Model(table_count, start_time, end_time, price, std::move(incoming_events));
    model.run();
}

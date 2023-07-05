#include <iostream>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <deque>
#include <queue>
#include <fstream>
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

    int table_count, price;
    std::string start_str, end_str;

    // TODO: Format check for this data
    fin >> table_count >> start_str >> end_str >> price;
    club::Time start_time(start_str), end_time(end_str);

    Model::queue_t incoming_events{};

    Time lastTime{};
    while (!fin.eof()) {
        std::string line;
        std::getline(fin, line);
        try {
            Event event = Event::from_string(line);
            if (event.time() < lastTime) {
                throw std::runtime_error("Wrong order");
            }
            incoming_events.push_back(event);

            lastTime = event.time();
        } catch (std::runtime_error const& error) {
            std::cout << line;
            return 0;
        }
    }

    auto model = Model(table_count, start_time, end_time, std::move(incoming_events));
    model.run();
}

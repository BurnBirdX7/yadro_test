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

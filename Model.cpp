#include <functional>
#include <ranges>
#include "Model.hpp"

using namespace club;

Model::Model(int table_count, Time start, Time end, Model::queue_t queue)
    : start_(start)
    , end_(end)
    , event_queue_(std::move(queue))
    , table_(table_count + 1, std::nullopt)
    , table_revenue_(table_count + 1, 0)
    , open_tables_(table_count)
{}

void Model::run() {
    while (!event_queue_.empty()) {
        current_time_ = event_queue_.front().time();

        // Opening / Closing
        if (!open_ && current_time_ > start_) {
            open_ = true;
        } else if (open_ && current_time_ >= end_) {
            open_ = false;

            // Force everybody outside
            for (auto const& [client, _] : clients_inside_ | std::views::reverse) {
                event_queue_.emplace_back(end_, EventType::FORCED_WALKED_OUT, client);
            }
            continue;
        }

        auto event = event_queue_.front();
        event_queue_.pop_front();

        std::cout << event.toString() << '\n';

        auto handler = handlers_.at(event.type());
        if (handler) {
            (this->*handler)(event);
        }
    }

}

// Auxiliary:
void club::Model::produce_error(const std::string &msg) {
    event_queue_.emplace_front(current_time_, EventType::ERROR, msg);
}


void Model::table_freed(int table_id) {
    // TODO: Implement
}

// Handlers
void Model::client_walked_in(const Event &event) {
    if (clients_inside_.contains(event.client())) {
        produce_error("YouShallNotPass&quot");
    } else if (!open_) {
        produce_error("NotOpenYet");
    } else {
        clients_inside_.emplace(event.client(), false);
    }
}

void Model::client_sat(const club::Event &event) {
    int target_table = event.table();
    if (table_[target_table].has_value()) {
        produce_error("PlaceIsBusy");
        return;
    } else if (!clients_inside_.contains(event.client())) {
        produce_error("ClientUnknown");
        return;
    }

    auto const &client = event.client();
    int current_table = clients_inside_[client];
    table_[target_table] = client;

    if (current_table != 0) {
        table_freed(current_table);
    }

}

void Model::client_waits(const Event &event) {
    if (open_tables_ > 0) {
        event_queue_.emplace_front(current_time_, EventType::FORCED_SAT, event.client());
        produce_error("ICanWaitNoLonger!");
        return;
    }

    client_queue_.push_back(event.client());
}

void Model::client_walked_out(const Event &event) {
    int table = clients_inside_[event.client()];
    if (table == 0) {
        return; // No table -> no revenue to update
    }



}

#include <functional>
#include <ranges>
#include "Model.hpp"

using namespace club;

Model::Model(int table_count, Time start, Time end, int hour_price, Model::queue_t queue)
    : start_(start)
    , end_(end)
    , hour_price_(hour_price)
    , event_queue_(std::move(queue))
    , table_status_(table_count + 1, std::nullopt)
    , table_revenue_(table_count + 1, 0)
    , open_tables_(table_count)
{}

void Model::run() {
    std::cout << start_.toString() << '\n';

    while (!event_queue_.empty()) {
        current_time_ = event_queue_.front().time();

        // Opening / Closing
        if (!open_ && current_time_ > start_) {
            open_ = true;
        } else if (open_ && current_time_ >= end_) {
            open_ = false;

            // Force everybody outside
            for (auto const& [client, _] : client_status | std::views::reverse) {
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

    std::cout << end_.toString() << '\n';

}

// Auxiliary:
void Model::produce_error(const std::string &msg) {
    event_queue_.emplace_front(current_time_, EventType::ERROR, msg);
}

void Model::update_revenue(int table_id, Time start_time) {
    auto passed_time = (current_time_ - start_time).roundUp().hours();
    table_revenue_[table_id] += passed_time * hour_price_;
    table_status_[table_id] = std::nullopt;
}

void Model::open_table(int table_id) {
    if (table_status_[table_id].has_value()) {
        update_revenue(table_id, table_status_[table_id]->second);
    }

    if (!client_queue_.empty()) {
        return;
    }

    event_queue_.emplace_front(current_time_,
                               EventType::FORCED_SAT,
                               client_queue_.front(),
                               table_id);
    client_queue_.pop_front();
}

// Handlers
void Model::client_walked_in(const Event &event) {
    if (client_status.contains(event.client())) {
        produce_error("YouShallNotPass&quot");
    } else if (!open_) {
        produce_error("NotOpenYet");
    } else {
        client_status.emplace(event.client(), false);
    }
}

void Model::client_sat(const Event &event) {
    int target_table = event.table();
    if (table_status_[target_table].has_value()) {
        produce_error("PlaceIsBusy");
        return;
    } else if (!client_status.contains(event.client())) {
        produce_error("ClientUnknown");
        return;
    }

    auto const &client = event.client();
    int current_table = client_status[client];
    if (current_table == 0) {
        table_status_[target_table] = {client, current_time_};
        return;
    }

    table_status_[target_table] = table_status_[current_table];
    table_status_[current_table] = std::nullopt;
    open_table(current_table);
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
    int table = client_status[event.client()];
    if (table == 0) {
        return; // No table -> no revenue to update
    }

    open_table(table);
    client_status.erase(event.client());
}

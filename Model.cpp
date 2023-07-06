#include <functional>
#include <ranges>
#include <format>
#include "Model.hpp"

using namespace club;

Model::Model(int table_count, Time start, Time end, int hour_price, Model::queue_t queue)
    : start_(start)
    , end_(end)
    , hour_price_(hour_price)
    , event_queue_(std::move(queue))
    , table_statistics_(table_count + 1)
    , open_tables_(table_count)
{}

void Model::run() {
    std::cout << start_.toString() << '\n';

    if (event_queue_.back().time() < end_) {
        event_queue_.emplace_back(end_, EventType::DUMMY);
    }

    while (!event_queue_.empty()) {
        current_time_ = event_queue_.front().time();

        // Opening / Closing
        if (!open_ && current_time_ > start_ && current_time_ < end_) {
            open_ = true;
        } else if (open_ && current_time_ >= end_) {
            open_ = false;

            if (event_queue_.front().type() == EventType::DUMMY) {
                event_queue_.pop_front();
            }

            // Force everybody outside
            for (auto const& [client, _] : client_status_ | std::views::reverse) {
                event_queue_.emplace_front(end_, EventType::FORCED_WALKED_OUT, client);
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
    for (size_t i = 1; i < table_statistics_.size(); ++i) {
        auto [_, revenue, usage] = table_statistics_[i];
        std::cout << std::format("{} {} {}\n", i, revenue, usage.toString());
    }
}

// Auxiliary:
void Model::produce_error(const std::string &msg) {
    event_queue_.emplace_front(current_time_, EventType::ERROR, msg);
}

void Model::update_revenue(int table_id) {
    auto& [busy, revenue, usage] = table_statistics_[table_id];
    auto passed_time = (current_time_ - busy->since);
    revenue += passed_time.roundUp().hours() * hour_price_;
    usage += passed_time;

    busy = {};
}

void Model::open_table(int table_id) {
    if (table_statistics_[table_id].occupied()) {
        update_revenue(table_id);
    }

    if (client_queue_.empty()) {
        return;
    }

    event_queue_.emplace_front(current_time_,
                               EventType::FORCED_SAT,
                               client_queue_.front(),
                               table_id);
    client_queue_.pop_front();
    open_tables_++;
}

// Handlers
void Model::client_walked_in(const Event &event) {
    if (client_status_.contains(event.client())) {
        produce_error("YouShallNotPass");
    } else if (!open_) {
        produce_error("NotOpenYet");
    } else {
        client_status_.emplace(event.client(), 0);
    }
}

void Model::client_sat(const Event &event) {
    int target_table = event.table();
    if (table_statistics_[target_table].occupied()) {
        produce_error("PlaceIsBusy");
        return;
    } else if (!client_status_.contains(event.client())) {
        produce_error("ClientUnknown");
        return;
    }

    auto const &client = event.client();
    int client_status = client_status_[client];
    if (client_status > 0) {
        open_table(client_status);
        return;
    } else if (client_status < 0) {
        std::erase(client_queue_, client);
    }

    table_statistics_[target_table].busy = {client, current_time_};
    client_status_[client] = target_table;
    open_tables_--;
}

void Model::client_waits(const Event &event) {
    if (open_tables_ > 0) {
        produce_error("ICanWaitNoLonger!");
    }
    if (client_queue_.size() >= table_statistics_.size()) {
        event_queue_.emplace_front(current_time_, EventType::FORCED_WALKED_OUT, event.client());
    }
    client_status_[event.client()] = -1;
    client_queue_.push_back(event.client());
}

void Model::client_walked_out(const Event &event) {
    int client_status = client_status_[event.client()];
    if (client_status <= 0) {
        return;
    }

    open_table(client_status);
    client_status_.erase(event.client());
    std::erase(client_queue_, event.client());
}

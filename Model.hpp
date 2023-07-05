#pragma once

#include <deque>
#include <queue>
#include <optional>
#include <unordered_map>
#include <set>
#include <map>

#include "Event.hpp"
#include "Time.hpp"

namespace club {

    class Model {
    public:
        using queue_t = std::deque<Event>;

        Model(int table_count, Time start, Time end, queue_t queue);

        void run();

    private: // auxiliary
        void produce_error(std::string const& msg);
        void table_freed(int table_id);

    private:
        // Essential
        Time start_;
        Time end_;
        queue_t event_queue_;

        // State
        bool open_ = false;
        Time current_time_ = {};

        // Clients:
        std::map<std::string /* name */, int /* table number */> clients_inside_ = {};
        std::deque<std::string> client_queue_;

        // Table statistics:
        using table_info = std::pair<std::string /* who */, Time /* since when */>;
        std::vector<std::optional<table_info>> table_;
        std::vector<int> table_revenue_;
        int open_tables_;

    public: // handlers
        using handler_f = void (Model::*)(Event const&);
        std::unordered_map<EventType, handler_f> const handlers_ = {
                {EventType::CLIENT_WALKED_IN, &Model::client_walked_in},
                {EventType::CLIENT_SAT, &Model::client_sat},
                {EventType::CLIENT_WAITS, &Model::client_waits},
                {EventType::CLIENT_WALKED_OUT, &Model::client_walked_out},

                {EventType::FORCED_WALKED_OUT, &Model::client_walked_out}, // same as default
                {EventType::FORCED_SAT, &Model::client_sat},               // same as default
                {EventType::ERROR, nullptr}, // noop
        };

        void client_walked_in(Event const& event);
        void client_sat(Event const& event);
        void client_waits(Event const& event);
        void client_walked_out(Event const& event);
        void error(Event const& event);
    };

}

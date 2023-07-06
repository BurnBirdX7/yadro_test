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

    struct TableStatistics {
        struct Busy {
            std::string client;
            Time since;
        };

        std::optional<Busy> busy  = {};
        int revenue = 0;
        Time usage  = {};

        [[nodiscard]] inline bool occupied() const {
            return busy.has_value();
        }
    };

    class Model {
    public:
        using queue_t = std::deque<Event>;

        Model(int table_count, Time start, Time end, int hour_price, queue_t queue);

        void run();

    private: // auxiliary
        void produce_error(std::string const& msg);
        void update_revenue(int table_id);
        void open_table(int table_id);

    private:
        // Essential:
        Time start_;
        Time end_;
        int hour_price_;

        // State:
        bool open_ = false;
        queue_t event_queue_;
        Time current_time_ = {};

        // Clients:
        /**
         * Client Status: int
         * > 0 - table ID
         * = 0 - inside the club
         * < 0 - waits in the queue
         */
        std::map<std::string, int> client_status_ = {};
        std::deque<std::string> client_queue_;

        // Table statistics:
        std::vector<TableStatistics> table_statistics_;
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
    };

}

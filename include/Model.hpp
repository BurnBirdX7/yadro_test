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

        /**
         * Constructor
         * @param table_count how many tables are in the club
         * @param start when club opens
         * @param end when club closes
         * @param hourly_price hourly rate of every table in the club
         * @param queue initial state of event queue
         */
        Model(int table_count, Time start, Time end, int hourly_price, queue_t queue);

        /**
         * Runs model simulation
         * @param out output stream
         */
        void run(std::ostream& out = std::cout);

    private: // auxiliary
        /**
         * Adds error event to the event queue
         * @param msg error message
         */
        void produce_error(std::string const& msg);

        /**
         * Updates table statistics
         * @param table_id table to update
         */
        void update_table_statistics(int table_id);

        /**
         * Open table to new clients, updates statistics and corrects state
         * May add FORCED_SAT event to the queue
         * @param table_id table to open
         */
        void open_table(int table_id);

        int table_count() const;

    private:
        // Essential:
        Time opening_time_;
        Time closing_time_;
        int hourly_price_;

        // State:
        bool is_open_ = false;
        queue_t event_queue_;
        Time current_time_ = {};

        // Client statistics:
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

    public: // Event handlers
        using handler_f = void (Model::*)(Event const&);
        std::unordered_map<EventType, handler_f> const handlers_ = {
                // Incoming events
                {EventType::CLIENT_WALKED_IN, &Model::client_walked_in},
                {EventType::CLIENT_SAT, &Model::client_sat},
                {EventType::CLIENT_WAITS, &Model::client_waits},
                {EventType::CLIENT_WALKED_OUT, &Model::client_walked_out},

                // Generated events
                {EventType::FORCED_WALKED_OUT, &Model::client_walked_out},
                {EventType::FORCED_SAT, &Model::client_sat},
                {EventType::ERROR, nullptr /* noop */ },
                {EventType::DUMMY, nullptr /* noop */ },
        };

        void client_walked_in(Event const& event);
        void client_sat(Event const& event);
        void client_waits(Event const& event);
        void client_walked_out(Event const& event);
    };

}

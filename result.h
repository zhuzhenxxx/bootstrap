#ifndef BOOTSTRAP_RESULT_H
#define BOOTSTRAP_RESULT_H
#include <string>
#include <vector>
#include <utility>
#include "result_message.h"

namespace basecode {

    class result {
    public:
        result() = default;

        inline void fail()
        {
            _success = false;
        }

        inline void succeed()
        {
            _success = true;
        }

        inline void add_message(
                const std::string& code,
                const std::string& message)
        {
            _message.emplace_back(code, message, std::string(), result_message::types::info);
        }

        inline void add_message(
                const std::string& code,
                const std::string& message,
                bool error)
        {
            _message.emplace_back(code, message,std::string(), error ? result_message::types::error : result_message::types::info);
            if (error)
                fail();
        }

        inline bool is_failed() const {
            return !_success;
        }

        inline const result_message_list& messages() const {
            return _message;
        }

        inline bool has_code(const std::string& code) const {
            for (const auto& msg : _message)
                if (msg.code() == code)
                    return true;
            return false;
        }

        inline const result_message* find_code(const std::string& code) const {
            for (auto it = _message.begin(); it != _message.end(); ++it)
            {
                if ((*it).code() == code)
                    return &(*it);
            }
            return nullptr;
        }

    private:
        bool _success = true;
        result_message_list _message {};
    };
}

#endif //BOOTSTRAP_RESULT_H

#ifndef BOOTSTRAP_HEX_FORMATTER_H
#define BOOTSTRAP_HEX_FORMATTER_H

#include <cstddef>
#include <cstdio>
#include <string>

namespace basecode {
    class hex_formatter {
    public:
        static std::string dump_to_string(const void* data, size_t size);
    };
}
#endif //BOOTSTRAP_HEX_FORMATTER_H

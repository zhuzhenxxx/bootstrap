#include <iostream>
#include <fmt/printf.h>
#include "terp.h"

int main()
{
    basecode::terp terp((1024 * 1024) * 32);

    basecode::result r;
    if (!terp.initialize(r))
    {
        std::cerr << "oh, fuck.......";
        return 1;
    }

    fmt::print("heap size : {} bytes, {} qwords\n", terp.heap_size(), terp.heap_size_in_qwords());

    const auto& regs = terp.register_file();
    auto stack_op = regs.sp;
    fmt::print("regs.sp = ${:08x}\n", regs.sp);
//    std::cout << "regs.sp " << regs.sp << std::endl;
    terp.push(0x08);
    terp.push(0x04);
    terp.push(0x02);

//    std::cout << "regs.sp " << regs.sp << " number of entries " << stack_op - regs.sp / sizeof(uint64_t)<< std::endl;
    fmt::print(
            "regs.sp = ${:08x}, number of entries: {} \n",
            regs.sp,
            stack_op - regs.sp / sizeof(uint64_t)
            );
    auto t1 = terp.pop();
    auto t2 = terp.pop();
    auto t3 = terp.pop();

    fmt::print("t1 = ${:08x}\n", t1);
    fmt::print("t2 = ${:08x}\n", t2);
    fmt::print("t3 = ${:08x}\n", t3);

    return 0;
}
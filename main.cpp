#include <iostream>
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

    std::cout << "heap size : " << terp.heap_size() << " qwords " << terp.heap_size_in_qwords() << std::endl;

    auto regs = terp.register_file();
    auto stack_op = regs.sp;
    std::cout << "regs.sp " << regs.sp << std::endl;
    terp.push(0x08);
    terp.push(0x04);
    terp.push(0x02);

    std::cout << "regs.sp " << regs.sp << " number of entries " << stack_op - regs.sp / sizeof(uint64_t)<< std::endl;

    auto t1 = terp.pop();
    auto t2 = terp.pop();
    auto t3 = terp.pop();
    std::cout << "t1: " << t1 << " t2 " << t2 << " t3 " << t3 << std::endl;

    return 0;
}
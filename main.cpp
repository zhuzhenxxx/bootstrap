#include <iostream>
#include <fmt/printf.h>
#include "terp.h"

void print_result(basecode::result& r)
{
    fmt::print("result success: {}\n", !r.is_failed());
    for (const auto& msg : r.messages())
    {
        fmt::print("\r{} code: {} | message: {}\n",
                   msg.is_error() ? "ERROR" : "",
                   msg.code(),
                   msg.message()
                   );
    }
}
int main()
{
    basecode::terp terp((1024 * 1024) * 32);

    basecode::result r;
    if (!terp.initialize(r))
    {
        std::cerr << "oh, fuck.......";
        print_result(r);
        return 1;
    }

    fmt::print("heap size : {} bytes, {} qwords\n", terp.heap_size(), terp.heap_size_in_qwords());

    const auto& regs = terp.register_file();
    auto stack_op = regs.sp;
    fmt::print("regs.sp = ${:08x}\n", regs.sp);

    terp.push(0x08);
    terp.push(0x04);
    terp.push(0x02);

    fmt::print(
            "regs.sp = ${:08x}, number of entries: {} \n",
            regs.sp,
            (stack_op - regs.sp) / sizeof(uint64_t)
            );
    auto t1 = terp.pop();
    auto t2 = terp.pop();
    auto t3 = terp.pop();

    fmt::print("t1 = ${:08x}\n", t1);
    fmt::print("t2 = ${:08x}\n", t2);
    fmt::print("t3 = ${:08x}\n", t3);

    size_t inst_size = 0;
    uint64_t location_counter = 0;

    basecode::instruction_t nop;
    nop.op = basecode::opcode::nop;
    nop.operand_count = 0;

    inst_size = terp.encode_instruction(r, location_counter, nop);
    if (inst_size == 0)
    {
        print_result(r);
        return 1;
    }
    location_counter += inst_size;

    basecode::instruction_t move_constant_to_reg3;
    move_constant_to_reg3.op = basecode::opcode::move;
    move_constant_to_reg3.size = basecode::op_size::byte;
    move_constant_to_reg3.operand_count = 2;
    move_constant_to_reg3.oprands[0].value = 0x7f;
    move_constant_to_reg3.oprands[0].type = basecode::operand_types::constant;
    move_constant_to_reg3.oprands[1].index = 3;
    move_constant_to_reg3.oprands[1].type = basecode::operand_types::register_integer;
    inst_size = terp.encode_instruction(r, location_counter, move_constant_to_reg3);
    if (inst_size == 0)
    {
        print_result(r);
        return 1;
    }
    location_counter += inst_size;

    basecode::instruction_t move_constant_to_reg4;
    move_constant_to_reg4.op = basecode::opcode::move;
    move_constant_to_reg4.size = basecode::op_size::byte;
    move_constant_to_reg4.operand_count = 2;
    move_constant_to_reg4.oprands[0].value = 0x7f;
    move_constant_to_reg4.oprands[0].type = basecode::operand_types::constant;
    move_constant_to_reg4.oprands[1].index = 4;
    move_constant_to_reg4.oprands[1].type = basecode::operand_types::register_integer;
    inst_size = terp.encode_instruction(r, location_counter, move_constant_to_reg4);
    if (inst_size == 0)
    {
        print_result(r);
        return 1;
    }

    location_counter += inst_size;

    basecode::instruction_t and_i3_to_i4;
    and_i3_to_i4.op = basecode::opcode::add;
    and_i3_to_i4.size = basecode::op_size::byte;
    and_i3_to_i4.operand_count = 3;
    and_i3_to_i4.oprands[0].index = 2;
    and_i3_to_i4.oprands[0].type = basecode::operand_types::register_integer;
    and_i3_to_i4.oprands[1].index = 3;
    and_i3_to_i4.oprands[1].type = basecode::operand_types::register_integer;
    and_i3_to_i4.oprands[2].index = 4;
    and_i3_to_i4.oprands[2].type = basecode::operand_types::register_integer;
    inst_size = terp.encode_instruction(r, location_counter, and_i3_to_i4);
    if (inst_size == 0)
    {
        print_result(r);
        return 1;
    }

    terp.dump_heap(0);

    for (size_t steps = 0; steps < 4; steps++)
    {
        if (!terp.step(r))
        {
            print_result(r);
            return 1;
        }
        terp.dump_status();
    }
    return 0;
}
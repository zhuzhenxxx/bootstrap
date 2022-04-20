#include "terp.h"
#include "hex_formatter.h"
#include <fmt/printf.h>

namespace basecode{
    terp::terp(uint32_t heap_size) : _heap_size(heap_size)
    {

    }

    terp::~terp()
    {
        delete _heap;
        _heap = nullptr;
    }

    bool terp::initialize(result& r)
    {
        _heap = new uint64_t[heap_size_in_qwords()];
        _registers.pc = 0;
        _registers.fr = 0;
        _registers.sr = 0;
        _registers.sp = heap_size_in_qwords();

        for (size_t i = 0; i < 64; i++)
        {
            _registers.i[i] = 0;
            _registers.f[i] = 0.0;
        }
        return !r.is_failed();
    }

    uint64_t terp::pop()
    {
        uint64_t value = _heap[_registers.sp];
        _registers.sp += sizeof(uint64_t);
        return value;
    }

    void terp::push(uint64_t value) {
        _registers.sp -= sizeof(uint64_t);
        _heap[_registers.sp] = value;
        return;
    }

    const register_file_t &terp::register_file() const
    {
        return _registers;
    }

    size_t terp::heap_size_in_qwords() const
    {
        return _heap_size / sizeof(uint64_t);
    }

    size_t terp::heap_size() const
    {
        return _heap_size;
    }

    bool terp::step(result &r)
    {
        instruction_t inst;
        auto inst_size = decode_instruction(r,inst);
        if (inst_size == 0)
            return false;

        switch (inst.op) {
            case opcode::nop:
                fmt::print("nop\n");
                break;
            case opcode::load:
                break;
            case opcode::store:
                break;
            case opcode::move:
            {
                fmt::print("move\n");
                uint64_t  source_value;
                if (!get_operand_value(r, inst, 0, source_value))
                {
                    return false;
                }
                if (!set_target_operand_value(r, inst, 1, source_value))
                {
                    return false;
                }
                break;
            }
            case opcode::push:
                fmt::print("push\n");
                break;
            case opcode::pop:
                fmt::print("pop\n");
                break;
            case opcode::add:
                uint64_t  lhs_value, rhs_value;
                if (!get_operand_value(r, inst, 1, lhs_value))
                {
                    return false;
                }
                if (!get_operand_value(r, inst, 2, rhs_value))
                {
                    return false;
                }
                if (!set_target_operand_value(r, inst, 0, lhs_value + rhs_value))
                {
                    return false;
                }
                fmt::print("add\n");
                break;
            case opcode::sub:
                break;
            case opcode::mul:
                break;
            case opcode::div:
                break;
            case opcode::mod:
                break;
            case opcode::neg:
                break;
            case opcode::shr:
                break;
            case opcode::shl:
                break;
            case opcode::ror:
                break;
            case opcode::rol:
                break;
            case opcode::and_op:
                break;
            case opcode::or_op:
                break;
            case opcode::xor_op:
                break;
            case opcode::not_op:
                break;
            case opcode::bis:
                break;
            case opcode::bic:
                break;
            case opcode::test:
                break;
            case opcode::cmp:
                break;
            case opcode::bz:
                break;
            case opcode::bnz:
                break;
            case opcode::tbnz:
                break;
            case opcode::bne:
                break;
            case opcode::beq:
                break;
            case opcode::bea:
                break;
            case opcode::ba:
                break;
            case opcode::bcc:
                break;
            case opcode::bcs:
                break;
            case opcode::jsr:
                break;
            case opcode::ret:
                break;
            case opcode::jmp:
                break;
            case opcode::mate:
                break;
            case opcode::debug:
                break;
        }

        return !r.is_failed();
    }

    size_t terp::decode_instruction(result& r, instruction_t &instruction)
    {
        if (_registers.pc % 8 != 0)
        {
            r.add_message("B003", "Instructions must be endcoded at 8-byte boundaries.", true);
            return 0;
        }
        uint8_t* encoding_ptr = reinterpret_cast<uint8_t *>(&_heap[_registers.pc / sizeof(uint64_t)]);
        uint8_t size = *encoding_ptr;

        uint16_t* op_ptr = reinterpret_cast<uint16_t*>(encoding_ptr + 1);
        instruction.op = static_cast<basecode::opcode>(*op_ptr);
        instruction.size = static_cast<op_size>(static_cast<uint8_t>(*(encoding_ptr + 3)));
        instruction.operand_count = static_cast<uint8_t >(*(encoding_ptr + 4));

        size_t offset = 5;
        for (size_t i = 0; i < instruction.operand_count; i++)
        {
            instruction.oprands[i].type = static_cast<operand_types>(*(encoding_ptr + offset));
            ++offset;

            instruction.oprands[i].index = *(encoding_ptr + offset);
            ++offset;

            instruction.oprands[i].value = 0;
            if (instruction.oprands[i].type == operand_types::constant)
            {
                uint64_t* constant_value_ptr = reinterpret_cast<uint64_t*>(encoding_ptr + offset);
                instruction.oprands[i].value = *constant_value_ptr;
                offset += sizeof(uint64_t);
            }
        }
        _registers.pc += size;

        return size;
    }

    size_t terp::encode_instruction(result& r, uint64_t address, instruction_t instruction)
    {
        if (address % 8 != 0)
        {
            r.add_message("B003", "Instructions must be endcoded at 8-byte boundaries.", true);
            return 0;
        }

        auto qword_address = address / sizeof(uint64_t);
        uint8_t size = 4;

        auto encoding_ptr = reinterpret_cast<uint8_t *>(_heap + qword_address);
        auto op_ptr = reinterpret_cast<uint16_t *>(encoding_ptr + 1);
        *op_ptr = static_cast<uint16_t >(instruction.op);

        *(encoding_ptr + 3) = static_cast<uint8_t>(instruction.size);
        *(encoding_ptr + 4) = instruction.operand_count;

        size_t offset = 5;
        for (size_t i = 0; i < instruction.operand_count; i++)
        {
            *(encoding_ptr + offset) = static_cast<uint8_t>(instruction.oprands[i].type);
            ++offset;
            ++size;

            *(encoding_ptr + offset) = instruction.oprands[i].index;
            ++offset;
            ++size;

            if (instruction.oprands[i].type == operand_types::constant)
            {
                uint64_t* constant_value_ptr = reinterpret_cast<uint64_t*>(encoding_ptr + offset);
                *constant_value_ptr = instruction.oprands[i].value;
                offset += sizeof(uint64_t);
                size += sizeof(uint64_t);
            }
        }

        if (instruction.operand_count > 0)
        {
            ++size;
        }

        if (size < 8)
        {
            size = 8;
        }

        size = static_cast<uint8_t >(align(size, sizeof(uint64_t)));
        *encoding_ptr = size;
        return size;
    }

    void terp::dump_heap(uint64_t address, size_t size)
    {
        auto program_memory = basecode::hex_formatter::dump_to_string(
                reinterpret_cast<const void*>(_heap), size);

        fmt::print("{}\n", program_memory);
    }

    size_t terp::align(uint64_t value, size_t size) const
    {
        auto offset = value % size;

        return offset ? value + (size - offset) : value;
    }

    bool
    terp::set_target_operand_value(result &r, const instruction_t &instruction, uint8_t operand_index, uint64_t value) {
        switch (instruction.oprands[operand_index].type) {
            case operand_types::register_integer:
            {
                _registers.i[instruction.oprands[operand_index].index] = value;
                return true;
            }
            case operand_types::register_floating_point:
                r.add_message("B009", "floating point registers connot be a target  for interger type.", true);
                break;
            case operand_types::register_sp:
                break;
            case operand_types::register_pc:
                break;
            case operand_types::register_flags:
                break;
            case operand_types::register_status:
                break;
            case operand_types::constant:
                r.add_message("B006", "constant connot be a target operand type.", true);
                break;
        }

        return false;
    }

    bool
    terp::set_target_operand_value(result &r, const instruction_t &instruction, uint8_t operand_index, double value) {
        switch (instruction.oprands[operand_index].type) {
            case operand_types::register_integer:
            {
                r.add_message("B009", "interger point registers connot be a target  for interger type.", true);
                break;
            }
            case operand_types::register_floating_point:
                _registers.i[instruction.oprands[operand_index].index] = value;
                break;
            case operand_types::register_sp:
                break;
            case operand_types::register_pc:
                break;
            case operand_types::register_flags:
                break;
            case operand_types::register_status:
                break;
            case operand_types::constant:
                r.add_message("B006", "constant connot be a target operand type.", true);
                break;
        }
        return false;
    }


    bool terp::get_operand_value(result& r, const instruction_t& instruction, uint8_t operand_index, uint64_t& value) const
    {
        switch (instruction.oprands[operand_index].type)
        {
            case operand_types::register_integer:
                value = _registers.i[instruction.oprands[operand_index].index];
                break;
            case operand_types::register_floating_point:

                break;
            case operand_types::register_sp:
                value = _registers.sp;
                break;
            case operand_types::register_pc:
                value = _registers.pc;
                break;
            case operand_types::register_flags:
                value = _registers.fr;
                break;
            case operand_types::register_status:
                value = _registers.sr;
                break;
            case operand_types::constant:
                value = instruction.oprands[operand_index].value;
                break;

        }

        // XXX: need to implement zero extend
        switch (instruction.size) {
            case op_size::byte:
                break;
            case op_size::word:
                break;
            case op_size::dword:
                break;
            case op_size::qword:
                break;
            default: {
                r.add_message("B005", "unsupported size of 'none' for operand", true);
                return false;
            }
        }
        return true;
    }


    bool terp::get_operand_value(result &r, const instruction_t &instruction, uint8_t operand_index, double &value) const
    {
        switch (instruction.oprands[operand_index].type)
        {
            case operand_types::register_integer:
                break;
            case operand_types::register_floating_point:
                value = _registers.i[instruction.oprands[operand_index].index];
                break;
            case operand_types::register_sp:
                break;
            case operand_types::register_pc:
                break;
            case operand_types::register_flags:
                break;
            case operand_types::register_status:
                r.add_message("B005", "integer registers cannot be uesd for floating point operands", true);
                break;
            case operand_types::constant:
                value = instruction.oprands[operand_index].value;
                break;
        }

        return false;
    }

    void terp::dump_status()
    {
        fmt::print("BaseCode Interpreter State\n");
        fmt::print("---------------------------------------\n");
        fmt::print("IO={:08x} | I1={:08x} | I2={:08x} | I3={:08x}\n",
                   _registers.i[0],
                   _registers.i[1],
                   _registers.i[2],
                   _registers.i[3]);
        fmt::print("I4={:08x} | I5={:08x} | I6={:08x} | I7={:08x}\n",
                   _registers.i[4],
                   _registers.i[5],
                   _registers.i[6],
                   _registers.i[7]);
        fmt::print("\n");
        fmt::print("PC={:08x} | SP={:08x} | FR={:08x} | SR={:08x}\n",
                   _registers.pc,
                   _registers.sp,
                   _registers.fr,
                   _registers.sr);
    }
}


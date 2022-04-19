#include "terp.h"

namespace basecode{
    terp::terp(uint32_t heap_size) : _heap_size(heap_size){

    }

    terp::~terp() {
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
        if (!decode_instruction(r,inst))
            return false;

        switch (inst.op) {
            case opcode::nop:
                break;
            case opcode::load:
                break;
            case opcode::store:
                break;
            case opcode::move:
                break;
            case opcode::push:
                break;
            case opcode::pop:
                break;
            case opcode::add:
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

    bool terp::decode_instruction(result& r, instruction_t &instruction)
    {
        if (_registers.pc % 8 != 0)
        {
            r.add_message("B003", "Instructions must be endcoded at 8-byte boundaries.", true);
            return false;
        }
        uint8_t* encoding_ptr = reinterpret_cast<uint8_t *>(&_heap[_registers.pc]);

        instruction.op = static_cast<basecode::opcode>(static_cast<uint16_t>(*encoding_ptr));
        instruction.operand_count = static_cast<uint8_t >(*(encoding_ptr + 2));
        _registers.pc += 64;

        return true;
    }

    bool terp::encode_instruction(result& r, uint64_t address, instruction_t instruction)
    {
        if (address % 8 != 0)
        {
            r.add_message("B003", "Instructions must be endcoded at 8-byte boundaries.", true);
            return false;
        }

        auto encoding_ptr = reinterpret_cast<uint8_t *>(_heap + address);

        auto op_ptr = reinterpret_cast<uint16_t *>(encoding_ptr);
        *op_ptr = static_cast<uint16_t >(instruction.op);

        *(encoding_ptr + 2) = instruction.operand_count;

        return true;
    }
}


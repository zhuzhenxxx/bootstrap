#ifndef BOOTSTRAP_TERP_H
#define BOOTSTRAP_TERP_H

#include <cstdint>
#include <string>
#include "result.h"

namespace basecode {
    // basecode interpreter which consumes base IR
    //
    // register-based machine with a generic stack
    // register file:
    // general purpose: data or address
    // I0-I63: integer registers 64-bit
    // data only：
    // F0-F63L floating point register(double precision)
    // stack pointer: sp
    // program counter: pc
    // flags : fr
    // status: sr
    // instructions:
    // load{.b|.w|.dw|.qw}
    //                 ^ default
    // .b = 8-bit
    // .w = 16-bit
    // .dw = 32-bit
    // .qw = 64-bit
    // non-used bits are zero-extended
    // store{.b|.w|.dw|.qw}
    // non-used bits are zero-extended
    // addressing modes(loads & store):
    // {target-register}, [{source-register}]
    //     ”       “    ，[{source-register}. offset constant]
    //
    // copy {source-register}, {target-register}, {length constant}
    // copy {source-register}, {target-register}, {length-register}
    //
    // fill {constant}, {target-register}, {length constant}
    // fill {constant}, {target-register}, {length-register}
    //
    // fill {constant}, {target-register}, {length constant}
    // fill {constant}, {target-register}, {length-register}
    //
    // fill {source-register}, {target-register}, {length constant}
    // fill {source-register}, {target-register}, {length-register}
    //
    // register/constant
    // move{.b|.w|.dw|.qw}
    // move constant to register -- or -- register to register
    // move.b #$06, I3
    // mov I3, I16
    // stack point: sp (like an IXX register)
    // flags: fr (definitely read; maybe write)
    // status: sr (definitely read; maybe write)
    // --------------
    // push{.b|.w|.dw|.qw}
    // pop{.b|.w|.dw|.qw}
    // sp register behaves like IXX register
    // ALU
    // -----------
    //
    // add{.b|.w|.dw|.qw}
    // sub{.b|.w|.dw|.qw}
    // mul{.b|.w|.dw|.qw}
    // div{.b|.w|.dw|.qw}
    // mod{.b|.w|.dw|.qw}
    //
    // size applicable to all: {.b|.w|.dw|.qw}
    //
    // shr
    // shl
    // ror
    // rol
    //
    // and
    // or
    // xor
    //
    // bis (bit set)
    // bic (bit clear)
    // test
    // tbz (test & branch if not set)
    // tbnz (test & branch if set)
    //
    // cmp (compare register to register, or register to constant
    // branch/conditional execution
    // ------------------------------------
    // bz (branch if zero)
    // bz (branch if not zero)
    // bne
    // beq
    // bae
    // ba
    // ble
    // bl
    // bo
    // bcc
    // bcs
    //
    // jsr
    //      - equivalent to call (endcode tail flag?)
    //          push current PC + sizeof(instruction)
    //          jmp to address
    // ret - jump to address on stack
    // jmp
    //      - absolute constant: jmp #$fffffff0
    //      - indirect register: jmp [14]
    //      - direct: jmp I4
    // nop
    //

    struct register_file_t {
        uint64_t i[64];
        double f[64];
        uint64_t pc;
        uint64_t sp;
        uint64_t fr;
        uint64_t sr;
    };

    enum class opcode : uint16_t {
        nop = 1,
        load,
        store,
        move,
        push,
        pop,
        add,
        sub,
        mul,
        div,
        mod,
        neg,
        shr,
        shl,
        ror,
        rol,
        and_op,
        or_op,
        xor_op,
        not_op,
        bis,
        bic,
        test,
        cmp,
        bz,
        bnz,
        tbnz,
        bne,
        beq,
        bea,
        ba,
        ble,
        bl,
        bo,
        bcc,
        bcs,
        jsr,
        ret,
        jmp,
        mate,
        debug
    };

    enum class operand_types {
        register_integer = 1,
        register_floating_point,
        register_pc,
        register_sp,
        register_flags,
        register_status,
        constant
    };

    enum class op_size : uint8_t {
        none,
        byte,
        word,
        dword,
        qword
    };

    struct operand_enconding_t{
        operand_types type;
        uint8_t index;
        uint64_t value;
    };

    struct instruction_t {
        opcode op;
        op_size size = op_size::none;
        uint8_t operand_count;
        operand_enconding_t oprands[4];
    };

    struct debug_information_t {
        uint32_t line_number;
        uint16_t colum_number;
        std::string symbol;
        std::string source_file;
    };

    class terp {
    public:
        explicit terp(uint32_t heap_size);
        virtual ~terp();
        bool initialize(result& r);

        uint64_t pop();
        void push(uint64_t value);

        void dump_heap(uint64_t address, size_t size = 256);
        bool step(result& r);

        size_t heap_size() const;
        size_t heap_size_in_qwords() const;
        const register_file_t& register_file() const;

        size_t encode_instruction(result& r, uint64_t address, instruction_t instruction);

    protected:
        size_t decode_instruction(result& r,instruction_t& instruction);

    private:
        uint32_t _heap_size = 0;
        uint64_t* _heap = nullptr;
        register_file_t _registers {};
    };
}
#endif //BOOTSTRAP_TERP_H

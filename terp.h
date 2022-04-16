#ifndef BOOTSTRAP_TERP_H
#define BOOTSTRAP_TERP_H

namespace basecode {
    // basecode interpreter which consumes base IR
    //
    //register-based machine with a generic stack
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
    // addressing modes:
    // {target-register}, [{source-register}]
    //     ”       “    ，[{source-register}. offset constant]
    class terp {
    public:

    private:

    };
}
#endif //BOOTSTRAP_TERP_H

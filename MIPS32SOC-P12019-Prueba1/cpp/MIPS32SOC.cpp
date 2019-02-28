#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <verilated.h>
#include "VMIPS32SOC_AsyncROM.h"
#include "VMIPS32SOC_RAMDualPort.h"
#include "VMIPS32SOC_RegisterFile.h"
#include "VMIPS32SOC_MIPS32SOC.h"
#include "VMIPS32SOC.h"
#include "mips32.h"
#include "Mips32Opcode.h"

using namespace std;

Mips32Opcode mips32;

const char *regNames[] = {
    "$zero",
    "$at",
    "$v0",
    "$v1",
    "$a0",
    "$a1",
    "$a2",
    "$a3",
    "$t0",
    "$t1",
    "$t2",
    "$t3",
    "$t4",
    "$t5",
    "$t6",
    "$t7",
    "$s0",
    "$s1",
    "$s2",
    "$s3",
    "$s4",
    "$s5",
    "$s6",
    "$s7",
    "$t8",
    "$t9",
    "$k0",
    "$k1",
    "$gp",
    "$sp",
    "$fp",
    "$ra"
};

void initRegisters(VMIPS32SOC *m) {
    m->MIPS32SOC->regFile->memory[A0_INDEX] = 10;  
    m->MIPS32SOC->regFile->memory[A1_INDEX] = 20;
}

void dumpDataMemory(VMIPS32SOC *m) {
    for (int i = 0; i < 16; i++) {
        printf("%04X ", m->MIPS32SOC->dataMem->memory[i]);
    }
    std::cout << dec << endl;
}

void dumpRegs(VMIPS32SOC *m) {
    for (int i = 0; i < 32; i++) {
        std::cout << regNames[i] << " = " 
                  << m->MIPS32SOC->regFile->memory[i]
                  << '\n';
    }
}

void dumpSignals(VMIPS32SOC *m) {
    uint32_t pc = m->MIPS32SOC->PC;
    uint32_t inst = m->MIPS32SOC->inst;

    std::cout << std::hex << pc << ": " << mips32.disassemble(pc, inst) << std::dec << '\n';
        /* << " "
         << "iszero = " << (int)m->MIPS32SOC->isZero << " "
         << "beq = " << (int)m->MIPS32SOC->isBEQ  << " " 
         << "rs = " << (int)m->MIPS32SOC->rs  << " " 
         << "rt = " << (int)m->MIPS32SOC->rt  << " " 
         << "rd = " << (int)m->MIPS32SOC->rd  << " " 
         << "RFData1 = " << m->MIPS32SOC->RFData1  << " " 
         << "RFData2 = " << m->MIPS32SOC->RFData2  << " " 
         << '\n';*/
}

int main(int argc, char** argv)
{
    VMIPS32SOC *msoc = new VMIPS32SOC;
    
    initRegisters(msoc);

    msoc->rst = 1;
    msoc->clk = 0;
    msoc->eval();
    msoc->clk = 1;
    msoc->eval();
    msoc->clk = 0;
    msoc->rst = 0;
    msoc->eval();
 
    for (int i = 0; i < 10; i++) {
        msoc->clk = !msoc->clk;
        if (msoc->clk == 1) {
            dumpSignals(msoc);
            if (msoc->MIPS32SOC->invalidOpcode) {
                std::cerr << "Invalid opcode. Aborting ...\n";
                break;
            }
        }
        msoc->eval();
    }
    //dumpDataMemory(msoc);
    dumpRegs(msoc);
    msoc->final();               // Done simulating
    delete msoc;

    return 0;
}

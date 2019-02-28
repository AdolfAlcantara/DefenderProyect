#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <iostream>
#include <chrono>
#include <random>
#include "doctest.h"
#include "VMIPS32SOC_AsyncROM.h"
#include "VMIPS32SOC_RAMDualPort.h"
#include "VMIPS32SOC_RegisterFile.h"
#include "VMIPS32SOC_MIPS32SOC.h"
#include "VMIPS32SOC.h"
#include "mips32.h"

#define GLOBAL_BASEADDR 0x10010000
#define STACK_BASEADDR  0x7FFFEFFC
#define CODE_BASEADDR   0x400000
#define REG(m, r) m->MIPS32SOC->regFile->memory[r##_INDEX]

#define CHECK_ERROR_SIGNALS(m) \
        do { \
            REQUIRE(m->MIPS32SOC->invalidOpcode == 0); \
            REQUIRE(m->MIPS32SOC->invalidPC == 0); \
        } while (0)

#include "tests-code.cpp"

void reset(VMIPS32SOC *m) {
    m->rst = 1;
    m->clk = 0;
    m->eval();
    m->clk = 1;
    m->eval();
    m->clk = 0;
    m->rst = 0;
    m->eval();
}

void clockPulse(VMIPS32SOC *m) {
    m->clk = 1;
    m->eval();
    m->clk = 0;
    m->eval();
}

void setProgramCode(VMIPS32SOC *m, const uint32_t code[], int size) {
    for (int i = 0; i < size; i++) {
        m->MIPS32SOC->instMem->memory[i] = code[i];
    }
}

void randomizeRegisterFile(VMIPS32SOC* m) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine re(seed);
    std::uniform_int_distribution<> dist(0, 65535);

    for (int i = 1; i < 32; i++) {
        m->MIPS32SOC->regFile->memory[i] = dist(re);
    }
}

TEST_CASE("'add' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 10500;  
    REG(m, A1) = 3500;
    REG(m, A2) = -3500;
    REG(m, A3) = 500;

    setProgramCode(m, test_add_code, TEST_ADD_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T0) == 14000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(static_cast<int32_t>(REG(m, T1)) == -3000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T2) == 0);
}

TEST_CASE("'addu' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 10500;  
    REG(m, A1) = 3500;
    REG(m, A2) = -3500;
    REG(m, A3) = 500;

    setProgramCode(m, test_addu_code, TEST_ADDU_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T0) == 14000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(static_cast<int32_t>(REG(m, T1)) == -3000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T2) == 0);
}

TEST_CASE("'sub' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 10500;  
    REG(m, A1) = 3500;
    REG(m, A2) = -3500;
    REG(m, A3) = 500;

    setProgramCode(m, test_sub_code, TEST_SUB_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T0) == 7000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(static_cast<int32_t>(REG(m, T1)) == -4000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T2) == 7000);
}

TEST_CASE("'subu' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 10500;  
    REG(m, A1) = 3500;
    REG(m, A2) = -3500;
    REG(m, A3) = 500;

    setProgramCode(m, test_subu_code, TEST_SUBU_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T0) == 7000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(static_cast<int32_t>(REG(m, T1)) == -4000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(REG(m, T2) == 7000);
}

TEST_CASE("'and' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);

    REG(m, A0) = 0xaabbccdd;  
    REG(m, A1) = 0x00ff00ff;
    REG(m, A2) = 0x11223344;
    REG(m, A3) = 0xff0000ff;

    setProgramCode(m, test_and_code, TEST_AND_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T0) == 0x00bb00dd);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T1) == 0x11000044);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T2) == 0x00220044);
}

TEST_CASE("'or' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);

    REG(m, A0) = 0xaa00cc00;  
    REG(m, A1) = 0x00bb00dd;
    REG(m, A2) = 0x00223300;
    REG(m, A3) = 0x11000044;

    setProgramCode(m, test_or_code, TEST_OR_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T0) == 0xaabbccdd);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T1) == 0x11223344);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T2) == 0x00BB33DD);
}

TEST_CASE("'slt' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);

    REG(m, A0) = 10500;  
    REG(m, A1) = 3500;
    REG(m, A2) = -3500;
    REG(m, A3) = 500;

    setProgramCode(m, test_slt_code, TEST_SLT_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T0) == 0);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T1) == 1);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(REG(m, T2) == 0);
}

TEST_CASE("'lw' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);

    REG(m, A0) = GLOBAL_BASEADDR;
    m->MIPS32SOC->dataMem->memory[0] = 0xaabbccdd;
    m->MIPS32SOC->dataMem->memory[4] = 0x11223344;
    m->MIPS32SOC->dataMem->memory[16] = 0xdeadbeef;

    setProgramCode(m, test_lw_code, TEST_LW_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);
    
    CHECK(REG(m, T0) == 0xaabbccdd);
    CHECK_ERROR_SIGNALS(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);

    CHECK(REG(m, T1) == 0x11223344);
    CHECK_ERROR_SIGNALS(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);

    CHECK(REG(m, T2) == 0xdeadbeef);
}

TEST_CASE("'sw' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    REG(m, A0) = GLOBAL_BASEADDR;
    REG(m, T0) = 0xaabbccdd;
    REG(m, T1) = 0x11223344;
    REG(m, T2) = 0xdeadbeef;

    setProgramCode(m, test_sw_code, TEST_SW_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);

    CHECK(m->MIPS32SOC->dataMem->memory[0] == 0xaabbccdd);
    CHECK_ERROR_SIGNALS(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);

    CHECK(m->MIPS32SOC->dataMem->memory[4] == 0x11223344);
    CHECK_ERROR_SIGNALS(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);
    
    CHECK(m->MIPS32SOC->dataMem->memory[16] == 0xdeadbeef);
}

TEST_CASE("'jmp' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    setProgramCode(m, test_jump_code, TEST_JUMP_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 16));
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 20));
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 4));
}

TEST_CASE("'beq' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    REG(m, A0) = 10500;
    REG(m, A1) = -3500;
    REG(m, A2) = 10500;
    REG(m, A3) = -3500;

    setProgramCode(m, test_beq_code, TEST_BEQ_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 4));
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 20));
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 24));
    clockPulse(m);
    CHECK_ERROR_SIGNALS(m);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 8));
}

TEST_CASE("'bne' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    REG(m, A0) = 10500;
    REG(m, A1) = 10500;
    REG(m, A2) = -3500;
    REG(m, A3) = -3500;

    setProgramCode(m, test_bne_code, TEST_BNE_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 4));
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 20));
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 24));
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);
    
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 8));
}

TEST_CASE("'lui' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    
    setProgramCode(m, test_lui_code, TEST_LUI_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T0) == 0xaabb0000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T1) == 0xccdd0000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T2) == 0x11220000);
}

TEST_CASE("'addi' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 3500;
    REG(m, A1) = 3500;
    REG(m, A2) = 3500;

    setProgramCode(m, test_addi_code, TEST_ADDI_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T0) == 4000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T1) == 3000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T2) == 0);
}

TEST_CASE("'addiu' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 3500;
    REG(m, A1) = 3500;
    REG(m, A2) = 3500;

    setProgramCode(m, test_addiu_code, TEST_ADDIU_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T0) == 4000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T1) == 3000);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T2) == 0);
}

TEST_CASE("'andi' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 0xaabbccdd;
    REG(m, A2) = 0x11223344;

    setProgramCode(m, test_andi_code, TEST_ANDI_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T0) == 0xdd);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T1) == 0x3344);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T2) == 0xc0d0);
}

TEST_CASE("'ori' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    randomizeRegisterFile(m);
    REG(m, A0) = 0xaabbcc00;
    REG(m, A1) = 0x11223300;
    REG(m, A2) = 0x11223300;

    setProgramCode(m, test_ori_code, TEST_ORI_CODE_SIZE);

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T0) == 0xaabbccdd);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T1) == 0x11223344);
    CHECK_ERROR_SIGNALS(m);
    clockPulse(m);

    CHECK(REG(m, T2) == 0x1122f3cd);
}

TEST_CASE("'invalid PC' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    // Special program
    m->MIPS32SOC->instMem->memory[0] = 0x08100200; //j	0x400800
    m->MIPS32SOC->instMem->memory[0x800] = 0x08100400; //j	0x401000

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    clockPulse(m);
    CHECK(m->MIPS32SOC->invalidPC == 0);
    CHECK(m->MIPS32SOC->PC == (CODE_BASEADDR + 0x800));
    clockPulse(m);
    CHECK(m->MIPS32SOC->invalidPC == 0);
}

TEST_CASE("'invalid global address' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    setProgramCode(m, test_invalid_gbladdr_code, TEST_INVALID_GBLADDR_CODE_SIZE);
    REG(m, A0) = GLOBAL_BASEADDR;

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    
    clockPulse(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);

    clockPulse(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 1);

    clockPulse(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR + 0xc);

    REG(m, A0) = GLOBAL_BASEADDR - 4;
    reset(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 1);
}

TEST_CASE("'invalid stack address' test") {
    VMIPS32SOC *m = new VMIPS32SOC;

    setProgramCode(m, test_invalid_stkaddr_code, TEST_INVALID_STKADDR_CODE_SIZE);
    REG(m, A1) = STACK_BASEADDR;

    reset(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    clockPulse(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);
    
    clockPulse(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 0);

    clockPulse(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 1);

    clockPulse(m);
    REQUIRE(m->MIPS32SOC->PC == CODE_BASEADDR + 0xc);

    REG(m, A1) = STACK_BASEADDR - 4;
    reset(m);
    REQUIRE(m->MIPS32SOC->invalidAddr == 1);
}
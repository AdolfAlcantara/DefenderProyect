/* verilator lint_off UNUSED */
module MIPS32SOC (
    input clk, // Clock signal
    input rst  // Reset signal
);
    wire [31:0] inst /*verilator public*/;
    reg  [31:0] nextPC; // Should be 'reg' because it used in a always block
    reg  [31:0] PC /*verilator public*/; // The PC (Program Counter) register
    wire [31:0] pcPlus4;
    wire [5:0] func;
    wire [4:0] rd /*verilator public*/;
    wire [4:0] rt /*verilator public*/;
    wire [4:0] rs /*verilator public*/;
    wire [5:0] opcode;
    wire aluSrc;
    wire rfWriteEnable; // Register File Write Enable
    wire rfWriteAddrSel; // Register File Write Address Select
    wire [1:0] rfWriteDataSel; // Register File Write Data Select
    wire [4:0] rfWriteAddr; // Register File Write Address
    reg  [31:0] rfWriteData; // Register File Write Data
    wire [31:0] rfData1 /*verilator public*/;
    wire [31:0] rfData2 /*verilator public*/;
    wire [31:0] imm32;
    wire [15:0] imm16;
    // wire [7:0] memAddr;
    wire memWrite;
    wire memRead;
    wire [31:0] memData;
    wire [2:0] aluFunc;
    wire [31:0] aluOperand2;
    wire [31:0] aluResult;
    wire [31:0] branchTargetAddr;
    wire [31:0] jmpTarget32;
    wire isJmp /*verilator public*/;
    wire isBEQ /*verilator public*/;
    wire isBNE /*verilator public*/;
    wire isZero /*verilator public*/;
    wire bitXtend;
    wire [11:0] physicalPC;
    wire [12:0] physicalAddr;
    wire invalidPC /*verilator public*/;
    wire invalidAddr /*verilator public*/;
    wire invalidOpcode /*verilator public*/;

    //tercerEntrega new wires
    wire [2:0] memEnabler;      //0 for datamem, 2 for vgamem, 4 for i/omem
    wire [1:0] memBankSel;      //selects the bank, 0 for data, 1 vga, 2 i/o
    wire [1:0] memDataSize;     //size of data yo be read/write 0 word, 1 half, 2byte
    wire memBitExt;             //in case of read, 0 = signext, 1 = zeroext
    wire [31:0] memWriteOutData;
    wire [31:0] memReadOutData;

    wire [3:0] encMW;            //each bit refers to the byte to be write

    wire [31:0] memReadInData;  //cable de salida del MUX para decidir la entrada del MemReadDataDecoder

    //vga wires
    wire [31:0] readVGAData;
    wire [2:0] red;
    wire [2:0] green;
    wire [1:0] blue;
    wire vsync,hsync;    


    assign func = inst[5:0];
    assign rd = inst[15:11];
    assign rt = inst[20:16];
    assign rs = inst[25:21];
    assign opcode = inst[31:26];
    assign imm16 = inst[15:0];
    // assign memAddr = aluResult; //antes aluResult[9:2]

    assign pcPlus4 = PC + 32'd4;
    assign jmpTarget32 = {pcPlus4[31:28], inst[25:0], 2'b00};
    assign branchTargetAddr = pcPlus4 + {imm32[29:0], 2'b00};

    assign rfWriteAddr = rfWriteAddrSel? rd : rt; // MUX
    assign aluOperand2 = aluSrc? imm32 : rfData2; // MUX
    assign rfWriteData = rfWriteDataSel[0]? memReadOutData : aluResult; // MUX

    //MemReadDataDecoderInData
    always @(*) begin
        case(memBankSel)
            2'd0: memReadInData = memData;
            2'd1: memReadInData = 32'd0; //vga
            2'd2: memReadInData = 32'd0; //io
            2'd3: memReadInData = 32'd0; //0
            default: memReadInData = 32'd0;
        endcase
    end

    // Next PC value
    always @ (*) begin
        if (isJmp)
            nextPC = jmpTarget32;
        else begin
            if (isBEQ & isZero)
                nextPC = branchTargetAddr;
            else if (isBNE & !isZero)
                nextPC = branchTargetAddr;
            else
                nextPC = pcPlus4;
        end
    end
  
    // PC
    always @ (posedge clk) begin
        if (rst)
            PC <= 32'h400000;
        else if(invalidPC | invalidOpcode | invalidAddr)
            PC <= PC;
        else
            PC <= nextPC;
    end
  
    // Instruction Memory
    AsyncROM instMem (
        .addr(physicalPC[11:2]),
        .en( 1'b1 ),
        .dout( inst )
    );

    // // Data Memory
    // RAMDualPort dataMem (
    //     .A( physicalAddr[12:2] ),
    //     .D_in( rfData2 ),
    //     .str( memWrite ),
    //     .C( clk ),
    //     .ld ( memRead ),
    //     .D ( memData )
    // );

    //Register File
    RegisterFile regFile (
        .ra1( rs ),
        .ra2( rt ),
        .wa( rfWriteAddr ),
        .wd( rfWriteData ),
        .we( rfWriteEnable ),
        .clk( clk ),
        .rd1( rfData1 ),
        .rd2( rfData2 )
    );

  // ALU
  ALU ALU_i11 (
    .a( rfData1 ),
    .b( aluOperand2 ),
    .func( aluFunc ),
    .res( aluResult ),
    .isZero( isZero )
  );

  // BitExtender
  BitExtender BitExtender_i12 (
    .x( bitXtend ),
    .in( imm16 ),
    .out( imm32 )
  );

  // Control Unit
  ControlUnit ControlUnit_i13 (
    .opc( opcode ),
    .func( func ),
    .jmp( isJmp ),
    .beq( isBEQ ),
    .bne( isBNE ),
    .rfWriteDataSel( rfWriteDataSel ),
    .rfWriteAddrSel( rfWriteAddrSel ),
    .rfWriteEnable( rfWriteEnable ),
    .memWrite( memWrite ),
    .memRead( memRead ),
    .aluSrc( aluSrc ),    
    .aluFunc( aluFunc ),
    .bitXtend( bitXtend ),
    .invOpcode( invalidOpcode ),
    .memDataSize(memDataSize),
    .memBitExt(memBitExt)
  );

  //PCDecoder
  PCDecoder PCDecoder_i14 (
    .virtPC(PC),
    .physPC(physicalPC),
    .invPC(invalidPC)
  );

  //MemDecoder
  MemDecoder MemDecoder_i15(
      .virtAddr(aluResult),
      .memWrite(memWrite),
      .memRead(memRead),
      .physAddr(physicalAddr),
      .memEn(memEnabler),
      .memBank(memBankSel),
      .invAddr(invalidAddr)
  );

  //DataMen
  DataMem dataMem(
      .clk(clk),
      .en(memEnabler[0]),
      .memWrite(encMW),
      .addr(physicalAddr[12:2]),
      .wdata(memWriteOutData),
      .rdata(memData)
  );

  //MemWriteDataEncode
  MemWriteDataEncoder MemWriteDataEncoder_i17(
      .inData(rfData2),
      .offset(physicalAddr[1:0]),
      .memWrite(memWrite),
      .dataSize(memDataSize),
      .outData(memWriteOutData),
      .encMW(encMW)
  );
   
  //MemReadDataDecoder
  MemReadDataDecoder MemReadDataDecoder_i18(
      .inData(memReadInData),
      .offset(physicalAddr[1:0]),
      .bitExt(memBitExt),
      .dataSize(memDataSize),
      .outData(memReadOutData)
  );

  //VGA MODULES
  //VGATextCard
  VGATextCard vgaTextCard(
      .vclk(1'd0),
      .clk(1'd0),
      .rst(1'd0),
      .en(1'd0),
      .memWrite(4'd0),
      .addr(11'd0),
      .wdata(32'd0),
      .rdata(readVGAData),
      .red(red),
      .green(green),
      .blue(blue),
      .hsync(hsync),
      .vsync(vsync)
  );


endmodule

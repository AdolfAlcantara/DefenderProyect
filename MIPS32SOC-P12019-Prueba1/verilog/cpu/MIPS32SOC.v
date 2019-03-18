/* verilator lint_off UNUSED */
module MIPS32SOC (
    input clk, // Clock signal
    input reset  // Reset signal
);
    wire [31:0] inst /*verilator public*/;
    reg  [31:0] nextPC; // Should be 'reg' because it used in a always block
    reg  [31:0] PC /*verilator public*/; // The PC (Program Counter) register
    wire [31:0] pcPlus4;
    wire [5:0] func;
    wire [4:0] rd /*verilator public*/;
    wire [4:0] rt /*verilator public*/;
    wire [4:0] rs /*verilator public*/;
    wire [4:0] sa;
    wire [5:0] opcode;
    wire [1:0] aluSrc;
    wire aluSrc1;
    wire rfWriteEnable; // Register File Write Enable
    wire [1:0] rfWriteAddrSel; // Register File Write Address Select
    wire [1:0] rfWriteDataSel; // Register File Write Data Select
    reg [4:0] rfWriteAddr; // Register File Write Address
    reg  [31:0] rfWriteData; // Register File Write Data
    wire [31:0] rfData1 /*verilator public*/;
    wire [31:0] rfData2 /*verilator public*/;
    wire [31:0] imm32;
    wire [15:0] imm16;
    // wire [7:0] memAddr;
    wire memWrite;
    wire memRead;
    wire [31:0] memData;
    wire [3:0] aluFunc;
    wire [31:0] aluOperand1;
    reg [31:0] aluOperand2;
    wire [31:0] aluResult;
    wire [31:0] branchTargetAddr;
    wire [31:0] jmpTarget32;
    wire isJmp /*verilator public*/;
    wire isJal;
    wire isJr;
    wire [2:0] branchSel /*verilator public*/;
    // wire isBEQ /*verilator public*/;
    // wire isBNE /*verilator public*/;
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

    reg [31:0] memReadInData;  //cable de salida del MUX para decidir la entrada del MemReadDataDecoder

    //cuarta entrega
    wire branchTaken;

    //vga wires
    wire [31:0] readVGAData;
    wire [2:0] red;
    wire [2:0] green;
    wire [1:0] blue;
    wire vsync,hsync;    


    assign func = inst[5:0];
    assign sa = inst[10:6];
    assign rd = inst[15:11];
    assign rt = inst[20:16];
    assign rs = inst[25:21];
    assign opcode = inst[31:26];
    assign imm16 = inst[15:0];
    // assign memAddr = aluResult; //antes aluResult[9:2]



    assign pcPlus4 = PC + 32'd4;
    assign jmpTarget32 = {pcPlus4[31:28], inst[25:0], 2'b00};
    assign branchTargetAddr = pcPlus4 + {imm32[29:0], 2'b00};

    // assign rfWriteAddr = rfWriteAddrSel? rd : rt; // MUX
    // assign aluOperand2 = aluSrc? imm32 : rfData2; // MUX
    // assign rfWriteData = rfWriteDataSel[0]? memReadOutData : aluResult; // MUX

    //cuarta entrega 
    //bitshifting muxes
    assign aluOperand1 = aluSrc1? rfData2 : rfData1;

    //rfWriteDataSel
    always @(*) begin
        case(rfWriteDataSel)
            2'd0: rfWriteData = aluResult;
            2'd1: rfWriteData = memReadOutData;
            2'd2: rfWriteData = pcPlus4;
            default: rfWriteData = 32'dx;
        endcase
    end

    //rfWriteAddr
    always @(*) begin
        case(rfWriteAddrSel)
            2'd0: rfWriteAddr = rt;
            2'd1: rfWriteAddr = rd;
            2'd2: rfWriteAddr = 5'd31;
            default: rfWriteAddr = 5'dx;
        endcase
    end

    //second AluOperand
    always @(*) begin
        case(aluSrc)
            2'd0: aluOperand2 = rfData2;
            2'd1: aluOperand2 = imm32;
            2'd2: aluOperand2 = {27'd0,sa};
            2'd3: aluOperand2 = rfData1; 
            default: aluOperand2 = 32'dx;
        endcase
    end


    //MemReadDataDecoderInData
    always @(*) begin
        case(memBankSel)
            2'd0: memReadInData = memData;
            2'd1: memReadInData = readVGAData; //vga
            2'd2: memReadInData = 32'd0; //io
            2'd3: memReadInData = 32'd0; //0
            default: memReadInData = 32'd0;
        endcase
    end

    // Next PC value
    always @ (*) begin
        if (isJmp || isJal)
            nextPC = jmpTarget32;
        else if(isJr)
            nextPC = rfData1;
        else begin
            if (branchTaken)
                nextPC = branchTargetAddr;
            else
                nextPC = pcPlus4;
        end
    end
  
    // PC
    always @ (posedge clk) begin
        if (reset)
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
    .rst( reset),
    .rd1( rfData1 ),
    .rd2( rfData2 )
);

  // ALU
  ALU ALU_i11 (
    .a( aluOperand1 ),
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
    .rt(rt),
    .jmp( isJmp ),
    .jal( isJal),
    .jr( isJr), 
    .branch(branchSel),
    .rfWriteDataSel( rfWriteDataSel ),
    .rfWriteAddrSel( rfWriteAddrSel ),
    .rfWriteEnable( rfWriteEnable ),
    .memWrite( memWrite ),
    .memRead( memRead ),
    .aluSrc( aluSrc ),
    .aluSrc1(aluSrc1),    
    .aluFunc( aluFunc ),
    .bitXtend( bitXtend ),
    .invOpcode( invalidOpcode ),
    .memDataSize(memDataSize),
    .memBitExt(memBitExt)
  );

  //BranchResolver
  BranchResolver branchResolver(
      .branch(branchSel),
      .zero(isZero),
      .sign(aluResult[31]),
      .branchTaken(branchTaken)
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
      .clk(clk),
      .rst(reset),
      .en(memEnabler[1]),
      .memWrite(encMW),
      .addr(physicalAddr[12:2]),
      .wdata(memWriteOutData),
      .rdata(readVGAData),
      .red(red),
      .green(green),
      .blue(blue),
      .hsync(hsync),
      .vsync(vsync)
  );


endmodule

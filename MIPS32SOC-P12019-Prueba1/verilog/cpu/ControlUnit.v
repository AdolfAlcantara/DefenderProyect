`include "opcodes.vh" 

module ControlUnit(
    input [5:0] opc, //! Opcode
    input [5:0] func, //! Function
    input [4:0] rt, //input code to difference BGEZ from BLTZ
    output jmp, //! Jump signal
    // output beq, //! BEQ signal
    // output bne, //! BNE signal
    output [2:0] branch;
    output [1:0] rfWriteDataSel, //! Register File Write Data Select
    output rfWriteAddrSel, //! Register File Write Address Select
    output rfWriteEnable, //! Register Write Enable
    output memWrite, //! Memory Write
    output memRead, //! Memory Read
    output aluSrc, //! ALU source
    output [3:0] aluFunc, //! ALU operation
    output bitXtend, //! Bit extension, 0 = sign extend, 1 = zero extend
    output invOpcode, //! Invalid opcode or function
    output memBitExt, //!indicates the type of extension in case of read
    output [1:0] memDataSize //!Size of data to read/write
);

always @ (opc or func)
begin
    jmp = 1'd0;
    branch = 3'd0; 
    memRead = 1'd0; 
    rfWriteDataSel = 2'd0; 
    memWrite = 1'd0; 
    aluSrc = 1'd0;
    rfWriteEnable = 1'd0; 
    rfWriteAddrSel = 1'd0; 
    aluFunc = 4'd0;
    bitXtend = 1'd0; 
    invOpcode = 1'd0;
    memBitExt = 1'd0;
    memDataSize = 2'd0;
    

  case(opc)
    6'b000000 :begin//R
      rfWriteEnable = 1'd1;
      rfWriteAddrSel = 1'd1;
      case(func)
        `ADD,`ADDU: aluFunc = 4'd0;
        `SUB,`SUBU: aluFunc = 4'd1;
        `AND:      aluFunc = 4'd2;
        `OR:       aluFunc = 4'd3;
        `SLT:      aluFunc = 4'd4;
        `SLTU:     aluFunc = 4'd7;
        `XOR:      aluFunc = 4'd6;
        default: aluFunc = 4'dx;
      endcase
    end
    `ADDI,`ADDIU:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd0;
      aluSrc = 1'd1;
    end
    `ANDI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd2;
      aluSrc = 1'd1;
      bitXtend = 1'd1;
    end
    `ORI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd3;
      aluSrc = 1'd1;
      bitXtend = 1'd1;
    end
    `XORI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd6;
      aluSrc = 1'd1;
      bitXtend = 1'd1;
    end
    `SLTI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd4;
      aluSrc = 1'd1;
    end
    `SLTIU:begin
     rfWriteEnable =1'd1;
     aluFunc = 4'd7;
     aluSrc = 1'd1;
    end
    `LUI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd5;
      aluSrc = 1'd1;
      bitXtend = 1'd1;
    end
    `BEQ:begin//beq
      aluFunc = 4'd1;
      branch = 3'd1;
    end
    `BNE: begin //bne
      aluFunc = 4'd1;
      branch = 3'd2;
    end
    `BGTZ: begin //greater than zero
      aluFunc = 4'd1;
      branch = 3'd4;
    end
    `BLEZ: begin  //less or equal zero
      aluFunc = 4'd1;
      branch = 3'd5;
    end
    6'h01: begin 
      if(rt == `BGEZ) begin //greater or equal zero
        aluFunc = 4'd1;
        branch = 3'd3;
      end
      else if(rt == `BLTZ) begin //less than zero
        aluFunc = 4'd1;
        branch = 3'd6;
      end
      else begin
        branch = 3'd0;
        aluFunc = 4'dx;.
      end
    end
    `LW:begin //lw;
      rfWriteEnable = 1'd1;
      aluFunc = 4'd0;
      aluSrc = 1'd1;
      memRead = 1'd1;
      rfWriteDataSel = 2'd1;
    end
    `LH:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd0;
      aluSrc = 1'd1;
      memRead = 1'd1;
      rfWriteDataSel = 2'd1;
      memDataSize = 2'd1; 
    end 
    `LHU:begin
      rfWriteEnable=1'd1;
      aluFunc = 4'd0;
      aluSrc =1'd1;
      memRead = 1'd1;
      rfWriteDataSel = 2'd1;
      memDataSize = 2'd1;
      memBitExt = 1'd1;
    end
    `LB:begin
      rfWriteEnable = 1'd1;
      aluFunc = 4'd0;
      aluSrc = 1'd1;
      memRead = 1'd1;
      rfWriteDataSel = 2'd1;
      memDataSize = 2'd2; 
    end
    `LBU:begin
      rfWriteEnable=1'd1;
      aluFunc = 4'd0;
      aluSrc =1'd1;
      memRead = 1'd1;
      rfWriteDataSel = 2'd1;
      memDataSize = 2'd2;
      memBitExt = 1'd1;
    end
    `SW:begin //sw
      aluFunc = 4'd0;
      memWrite = 1'd1;
      aluSrc = 1'd1;
    end
    `SH: begin //store half
      aluFunc = 4'd0;
      memWrite = 1'd1;
      aluSrc = 1'd1;
      memDataSize = 2'd1;
    end
    `SB: begin //store byte
      aluFunc = 4'd0;
      memWrite = 1'd1;
      aluSrc = 1'd1;
      memDataSize = 2'd2;
    end
    `JUMP: begin //jump
      jmp = 1'd1;
    end
    default: begin
      jmp = 1'dx; ; 
      memRead = 1'dx; 
      rfWriteDataSel = 2'dx; 
      memWrite = 1'dx; 
      aluSrc = 1'dx;
      rfWriteEnable = 1'dx; 
      rfWriteAddrSel = 1'dx; 
      aluFunc = 4'dx;
      bitXtend = 1'dx; 
      invOpcode = 1'd1;
      memDataSize=2'dx;
      memBitExt = 1'dx;
    end
  endcase
end

endmodule


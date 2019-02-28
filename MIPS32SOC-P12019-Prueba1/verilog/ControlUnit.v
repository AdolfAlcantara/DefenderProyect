`include "opcodes.vh" 

module ControlUnit(
    input [5:0] opc, //! Opcode
    input [5:0] func, //! Function
    output jmp, //! Jump signal
    output beq, //! BEQ signal
    output bne, //! BNE signal
    output [1:0] rfWriteDataSel, //! Register File Write Data Select
    output rfWriteAddrSel, //! Register File Write Address Select
    output rfWriteEnable, //! Register Write Enable
    output memWrite, //! Memory Write
    output memRead, //! Memory Read
    output aluSrc, //! ALU source
    output [2:0] aluFunc, //! ALU operation
    output bitXtend, //! Bit extension, 0 = sign extend, 1 = zero extend
    output invOpcode //! Invalid opcode or function
);

always @ (opc or func)
begin
    jmp = 1'd0; 
    beq = 1'd0; 
    bne = 1'd0; 
    memRead = 1'd0; 
    rfWriteDataSel = 2'd0; 
    memWrite = 1'd0; 
    aluSrc = 1'd0;
    rfWriteEnable = 1'd0; 
    rfWriteAddrSel = 1'd0; 
    aluFunc = 3'd0;
    bitXtend = 1'd0; 
    invOpcode = 1'd0;

  case(opc)
    6'b000000 :begin//R
      rfWriteEnable = 1'd1;
      rfWriteAddrSel = 1'd1;
      case(func)
        `ADD,`ADDU: aluFunc = 3'd0;
        `SUB,`SUBU: aluFunc = 3'd1;
        `AND:      aluFunc = 3'd2;
        `OR:       aluFunc = 3'd3;
        `SLT:      aluFunc = 3'd4;
        default: aluFunc = 3'dx;
      endcase
    end
    `ADDI,`ADDIU:begin
      rfWriteEnable = 1'd1;
      aluFunc = 3'd0;
      aluSrc = 1'd1;
    end
    `ANDI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 3'd2;
      aluSrc = 1'd1;
    end
    `ORI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 3'd3;
      aluSrc = 1'd1;
    end
    `LUI:begin
      rfWriteEnable = 1'd1;
      aluFunc = 3'd5;
      aluSrc = 1'd1;
    end
    `BEQ:begin//beq
      aluFunc = 3'd1;
      beq = 1'd1;
    end
    `BNE: begin //bne
      aluFunc = 3'd1;
      bne = 1'd1;
    end
    `LW:begin //lw;
      rfWriteEnable = 1'd1;
      aluFunc = 3'd0;
      aluSrc = 1'd1;
      memRead = 1'd1;
      rfWriteDataSel = 1'd1; 
    end 
    `SW:begin //sw
      aluFunc = 3'd0;
      memWrite = 1'd1;
      aluSrc = 1'd1;
    end
    `JUMP: begin //jump
      jmp = 1'd1;
    end
    default: begin
      jmp = 1'dx; 
      beq = 1'dx; 
      bne = 1'dx; 
      memRead = 1'dx; 
      rfWriteDataSel = 2'dx; 
      memWrite = 1'dx; 
      aluSrc = 1'dx;
      rfWriteEnable = 1'dx; 
      rfWriteAddrSel = 1'dx; 
      aluFunc = 3'dx;
      bitXtend = 1'dx; 
      invOpcode = 1'd1;
    end
  endcase
end

endmodule

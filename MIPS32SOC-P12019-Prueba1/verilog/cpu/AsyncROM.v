module AsyncROM (
  input [9:0] addr,
  input en,
  output [31:0] dout
);
  reg [31:0] memory[0:1023] /*verilator public*/;

  assign dout = en? memory[addr] : 32'hz;
  
  initial begin
`ifndef NO_INIT_MEM
    $readmemh("code.mif", memory, 0, 1023);
`endif
  end
endmodule 
 

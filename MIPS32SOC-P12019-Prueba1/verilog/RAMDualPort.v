module RAMDualPort (
  input [7:0] A,
  input [31:0] D_in,
  input str,
  input C,
  input ld,
  output [31:0] D
);
  reg [31:0] memory[0:255] /*verilator public*/;

  assign D = ld? memory[A] : 32'hz;

  always @ (posedge C) begin
    if (str)
      memory[A] <= D_in;
  end

  initial begin
`ifndef NO_INIT_MEM
    $readmemh("data.mif", memory, 0, 255);
`endif
  end
endmodule 

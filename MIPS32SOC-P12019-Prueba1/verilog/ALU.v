module ALU(
    input [31:0] a,
    input [31:0] b,
    input [2:0] func,
    output reg [31:0] res,
    output reg isZero
    );

always @ (*) begin
  res = 32'dx;
  case (func)
    3'd0: res = a + b;
    3'd1: res = a - b;
    3'd2: res = a & b;
    3'd3: res = a | b;
    3'd4: res = {31'd0,($signed(a) < $signed(b))};
    3'd5: res = b << 16;
    3'd6: res = a ^ b;
    3'd7: res = {31'd0,($unsigned(a) < $unsigned(b))};
    default: res = 32'dx;
  endcase

  isZero = (res == 32'd0);
end
endmodule
 
module ALU(
    input [31:0] a,
    input [31:0] b,
    input [3:0] func,
    output reg [31:0] res,
    output reg isZero
    );

always @ (*) begin
  res = 32'dx;
  case (func)
    4'd0: res = a + b;
    4'd1: res = a - b;
    4'd2: res = a & b;
    4'd3: res = a | b;
    4'd4: res = {31'd0,($signed(a) < $signed(b))};
    4'd5: res = b << 16;
    4'd6: res = a ^ b;
    4'd7: res = {31'd0,($unsigned(a) < $unsigned(b))};
    4'd8: res = a << b;
    4'd9: res = a >> b;
    4'd10: res = $signed(a) >>> $signed(b);
    default: res = 32'dx;
  endcase

  isZero = (res == 32'd0);
end
endmodule
 
module BitExtender (
    input x, //! If x==0, do sign extend, else do zero extend
    input [15:0] in, //! Input value (16 bits)
    output [31:0] out //! Extended output value (32 bits)
);

always @(*) begin
    if(in[15])
        out = {16'h0000,in};
    else
        out = {16'hFFFF,in};
end

endmodule


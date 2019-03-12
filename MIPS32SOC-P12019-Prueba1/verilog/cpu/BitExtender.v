module BitExtender (
    input x, //! If x==0, do sign extend, else do zero extend
    input [15:0] in, //! Input value (16 bits)
    output [31:0] out //! Extended output value (32 bits)
);

assign out = x ? {{16{1'h0}},in} : {{16{in[15]}},in};

endmodule


/* verilator lint_off WIDTH */
/* verilator lint_off UNUSED */
module PCDecoder (
    input [31:0] virtPC, //! Virtual PC
    output [11:0] physPC, //! Physical PC
    output invPC // Invalid program counter
);

always @(*) begin

    invPC = 1'd0;
    physPC = 12'dx;

    if(virtPC >= 32'h400000 & virtPC < 32'h401000)
        physPC = virtPC[11:0];
    else
        invPC = 1'd1;
end

endmodule

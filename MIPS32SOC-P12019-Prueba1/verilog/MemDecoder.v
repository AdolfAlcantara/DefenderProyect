/* verilator lint_off WIDTH */
module MemDecoder (
    input [31:0] virtAddr, //! Virtual address
    output [12:0] physAddr, //! Physical address
    output invAddr //! Invalid address
);

always @(*) begin

    invAddr = 1'd0;
    physAddr = 13'dx;

    if(virtAddr >= 32'h7FFFEFFC & virtAddr < 32'h7FFFFFFC)
        physAddr = virtAddr - 32'h7FFFEFFC + 32'h400;
    else if(virtAddr >= 32'h10010000 & virtAddr < 32'h10011000)
        physAddr = virtAddr[12:0];
    else
        invAddr = 1'd1;

end

endmodule

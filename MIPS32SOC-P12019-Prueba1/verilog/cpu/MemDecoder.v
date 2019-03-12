/* verilator lint_off WIDTH */
module MemDecoder (
    input [31:0] virtAddr, //! Virtual address
    input memWrite,
    input memRead,
    output [12:0] physAddr, //! Physical address
    output  [2:0] memEn,
    output [1:0] memBank,
    output invAddr //! Invalid address
);

always @(*) begin

    invAddr = 1'd0;
    physAddr = 13'dx;
    memEn = 3'dx;
    memBank = 2'dx;

    if(virtAddr >= 32'h7FFFEFFC && virtAddr < 32'h7FFFFFFC) begin
        physAddr = virtAddr[12:0] - 32'h7FFFEFFC + 32'h400;
        memBank = 2'd0;
        if(!(memWrite || memRead))
            memEn = 3'd0;
        else
            memEn = 3'd1;
    end
    else if(virtAddr >= 32'h10010000 && virtAddr < 32'h10011000) begin
        physAddr = virtAddr[12:0];
        memBank = 2'd0;
        if(!(memWrite || memRead))
            memEn = 3'd0;
        else
            memEn = 3'd1;
    end
    else if(virtAddr >= 32'hB800 && virtAddr < 32'hCACF) begin 
        physAddr = virtAddr[12:0] - 32'hB800;
        memBank = 2'd1;
        if(~(memWrite || memRead))
            memEn = 3'd0;
        else
            memEn = 3'd2;
    end
    else if(virtAddr >= 32'hFFFF0000 && virtAddr < 32'hFFFF000C) begin
        physAddr = virtAddr[12:0] - 32'hFFFF0000;
        memBank = 2'd2;
        if(~(memWrite || memRead))
            memEn = 3'd0;
        else
            memEn = 3'd4;
    end
    else
        if(memWrite || memRead)
            invAddr = 1'd1;

end

endmodule

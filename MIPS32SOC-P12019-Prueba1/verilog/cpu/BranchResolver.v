module BranchResolver(
    input [2:0] branch,
    input zero,
    input sign,
    output branchTaken
);

always @(*) begin

    branchTaken = 1'dx;

    case(branch)
        3'd1: branchTaken = zero? 1'd1 : 1'd0;                  //equal 
        3'd2: branchTaken = zero? 1'd0 : 1'd1;                  //not equal
        3'd3: branchTaken = sign ? 1'd0 : 1'd1;                 //greater or equal 0
        3'd4: branchTaken = sign || zero? 1'd0 : 1'd1;          //greater than
        3'd5: branchTaken = sign || zero? 1'd1 : 1'd0;          //less  or equal 0
        3'd6: branchTaken = sign ? 1'd1 : 1'd0;                 //less than
        default: branchTaken = 1'd0;
    endcase
end

endmodule
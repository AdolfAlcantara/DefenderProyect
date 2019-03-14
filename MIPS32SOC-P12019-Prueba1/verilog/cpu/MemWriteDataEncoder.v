module MemWriteDataEncoder(
    input [31:0] inData,
    input [1:0] offset,
    input memWrite,
    input [1:0] dataSize,
    output [31:0] outData,
    output [3:0] encMW
);

always @(*) begin

    if(memWrite)
        case(dataSize)
            2'd0: begin
                outData = inData;
                encMW = 4'b1111;
            end
            2'd1:begin
                if(offset == 2'd0) begin
                    encMW = 4'b0011;
                    outData  = {inData[15:0],16'd0};
                end
                else begin
                    encMW = 4'b1100;
                    outData = {16'd0,inData[15:0]};
                end
            end
            2'd2:begin
                case(offset)
                    2'd0:begin
                        encMW = 4'b0001;
                        outData = {inData[7:0],24'd0};
                    end
                    2'd1: begin
                        encMW = 4'b0010;
                        outData = {8'd0,inData[7:0],16'd0};
                    end
                    2'd2:begin
                        encMW = 4'b0100;
                        outData = {16'd0,inData[7:0],8'd0};
                    end
                    2'd3:begin
                        encMW = 4'b1000;
                        outData = {24'd0,inData[7:0]};
                    end
                    default: begin
                        encMW = 4'dx;
                        outData = 32'dx;
                    end
                endcase
            end
            default: begin
                outData = 32'dx;
                encMW = 4'dx;
            end
        endcase
    else begin
        outData =32'dx;
        encMW = 4'dx;
    end
end


endmodule
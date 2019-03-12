module MemReadDataDecoder(
    input [31:0] inData,
    input [1:0] offset,
    input bitExt,
    input [1:0] dataSize,
    output [31:0] outData
);

always @(*) begin

    case(dataSize)
        2'd0: outData = inData;
        2'd1:begin
            if(offset == 2'd0)
                outData = bitExt ? {16'd0,inData[31:16]} :  {{16{inData[31]}},inData[31:16]};
            else
                outData = bitExt ? {16'd0,inData[15:0]} : {{16{inData[15]}},inData[15:0]}; 
        end
        2'd2:begin
            case(offset)
                2'd0:begin
                    outData = bitExt ? {24'd0,inData[31:24]} : {{24{inData[31]}},inData[31:24]};
                end
                2'd1: begin
                    outData = bitExt ? {24'd0,inData[23:16]} : {{24{inData[23]}},inData[23:16]};
                end
                2'd2:begin
                    outData = bitExt ? {24'd0,inData[15:8]} : {{24{inData[15]}},inData[15:8]};
                end
                2'd3:begin
                    outData = bitExt ? {24'd0,inData[7:0]} : {{24{inData[7]}},inData[7:0]};
                end
                default: begin
                    outData = 32'dx;
                end
            endcase
        end
        default: begin
            outData = 32'dx;
        end
    endcase
end


endmodule
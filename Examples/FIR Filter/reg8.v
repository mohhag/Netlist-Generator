module reg8	(reset, en, clk, rin, rout);
	
	input reset;
	input en;
	input clk;
	input [7:0] rin;
	output [7:0] rout;
	
	reg [7:0] rout;
	
	always @ (posedge clk)
	begin
		if (reset == 1'b1)
			rout <= 8'b00000000;
		else if (en == 1'b1)
			rout <= rin;
	end
endmodule
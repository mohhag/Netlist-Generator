
//--Module for testing the netlist of FIR filter
module tb_FIR();
	reg reset;
	reg clk;
	reg [7:0] x;
	wire [7:0] y; 
	wire [7:0] y_net;

	FIR fir(reset, clk, x,y);
	FIR_net net_fir(reset, clk, x,y_net);
		
	initial begin
		clk = 0;
		reset = 1;
		x = 8'h45;
		#100
		reset = 0;
		#100
		x = 8'hf5;
		#100
		x = 8'hfe;
		#100
		x = 8'h03;	
	end
		
	always #10 clk = ~clk;
endmodule
		

	

//--Module for testing the netlist of FIR filter
module tb_mult8();
	
	reg [0:7]i1;
	reg [0:7]i2;
	wire [0:7]o;
	wire [0:7]o_net;

	mult8 mult(i1, i2, o);
	mult8_net mult_net(i1, i2, o_net);
		
	initial begin
		i1 = 0;
		i2 = 0;
		#100
		i1 = 8'h45;
		#100
		i2 = 8'h1;
		#100
		i1 = 8'hf5;
		#100
		i2 = 8'hfe;
		#100
		i1 = 0;	
	end
		
endmodule
		

	
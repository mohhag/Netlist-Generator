module mult8 (i1, i2, o);
	
	input [7:0] i1;
	input [7:0] i2;
	output [7:0] o;
	
	assign o = i1 * i2;
endmodule
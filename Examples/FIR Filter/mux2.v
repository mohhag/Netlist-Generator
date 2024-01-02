module mux2(I1, I0, s, O);

	input [7:0]  I1; 
	input [7:0]  I0;
	input s;
	output [7:0] O;

	assign O = (s == 1'b0) ? I0 : I1;
endmodule
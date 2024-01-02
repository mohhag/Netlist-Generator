 
module FIR_ctrl (reset, clk, x, r1out, m1out,m3out,m4out, a1out,a2out, y,x0s,x1s,x2s,x3s,x4s,x5s,x6s,x7s,x8s,x9s,x10s,x11s,r1en,r2en,r3en,r4en,m1a1out, m4a2out, m3a1out, x1,x2,x3, x5, x6, x7, a0,a1,a2,a3, a4, a5, a6, a7);

	
	input reset;
	input clk;
	input [7:0] x;
	input [7:0] r1out;
	input [7:0] m1out;
	input [7:0] m3out;
	input [7:0] m4out;	
	input [7:0] a1out;
	input [7:0] a2out;
	

	output [7:0] y; 
	

	output  x0s;
	output  x1s;
	output  x2s;
	output  x3s;
	output  x4s;
	output  x5s;
	output  x6s;
	output  x7s;
	output  x8s;
	output  x9s;
	output  x10s;
	output  x11s;

	
	output r1en;
	output r2en;
	output r3en;
	output r4en;
	
	output [7:0] m1a1out;
	output [7:0] m4a2out;
	output [7:0] m3a1out;
	
	

	output [7:0] x1;
	output [7:0] x2;
	output [7:0] x3;
	//output [7:0] x4;
	output [7:0] x5;
	output [7:0] x6;
	output [7:0] x7;
	
	output [7:0] a0;
	output [7:0] a1;
	output [7:0] a2;
	output [7:0] a3;
	output [7:0] a4;
	output [7:0] a5;	
	output [7:0] a6;
	output [7:0] a7;	

	
	reg [2:0] sout;	
	
	reg [7:0] x1;
	reg [7:0] x2;
	reg [7:0] x3;
	//reg [7:0] x4;
	reg [7:0] x5;
	reg [7:0] x6;
	reg [7:0] x7;
	
	reg [7:0] a0;
	reg [7:0] a1;
	reg [7:0] a2;
	reg [7:0] a3;
	reg [7:0] a4;
	reg [7:0] a5;	
	reg [7:0] a6;
	reg [7:0] a7;	

	
	assign x0s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x1s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x2s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x3s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x4s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x5s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x6s = (sout == 3'b001) ? 1'b1 : 1'b0;
	assign x7s = (sout == 3'b001) ? 1'b1 : 1'b0;

	assign x8s = (sout == 3'b010) ? 1'b1 : 1'b0;
	assign x9s = (sout == 3'b010) ? 1'b1 : 1'b0;
	
	assign x10s = (sout == 3'b011) ? 1'b1 : 1'b0;
	assign x11s = (sout == 3'b011) ? 1'b1 : 1'b0;	
	

	assign r1en = ((sout == 3'b001) || (sout == 3'b010) || (sout == 3'b011)) ? 1'b1 : 1'b0;
	assign r2en = ((sout == 3'b001) || (sout == 3'b010)) ? 1'b1 : 1'b0;	
	assign r3en = (sout == 3'b101) ? 1'b0 : 1'b1;
	assign r4en = 1'b1;
	

        assign m1a1out = (sout == 3'b011) ? a1out : m1out;
        assign m3a1out = ((sout == 3'b010) || (sout == 3'b100)) ? a1out : m3out;        
        assign m4a2out = ((sout == 3'b001) || (sout == 3'b100)) ? m4out : a2out;           
		
        assign y = (sout == 3'b101) ? a2out : 8'b00000000;      
        //assign y1 = (sout == 3'b101) ? a2out_vote : 8'b00000000;        
        
	
	always @(posedge clk)
	begin
		if (reset == 1'b1) begin 
			sout <= 3'b001;
			x1<=8'b00000000;
			x2<=8'b00000000;
			x3<=8'b00000000;
			x5<=8'b00000000;
			x6<=8'b00000000;
			x7<=8'b00000000;


			a0<=8'b00000011;
			a1<=8'b00000010;
			a2<=8'b00000011;
			a3<=8'b00000100;
			a4<=8'b00000010;
			a5<=8'b00000100;
			a6<=8'b00000101;
			a7<=8'b00000011;
		end
		else begin
			if (sout == 3'b001)
				sout <= 3'b010;
			else if (sout == 3'b010)
				sout <= 3'b011;
			else if (sout == 3'b011)
				sout <= 3'b100;
			else if (sout == 3'b100)
				sout <= 3'b101;
			else if (sout == 3'b101)
			  begin
				sout <= 3'b001;	
				x3<=x2;
				x2<=x1;
				x1<=x;
				x7<=x6;
				x6<=x5;
				x5<=r1out;				
		          end
		end
		//y<=y1;
	end
	
	
endmodule



//--FIR filter
module FIR (reset, clk, x,y);

	input reset;
	input clk;
	input [7:0] x;
	output [7:0] y; 
	
	wire x0s, x1s, x2s, x3s, x4s, x5s, x6s , x7s, x8s, x9s, x10s, x11s;
	
	wire r1en, r2en, r3en, r4en;
	wire [7:0] s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, r1out, r2out, r3out, r4out;
	wire [7:0] x1,x2,x3,x5,x6,x7;
	wire [7:0] a0,a1,a2,a3, a4, a5, a6, a7;
        wire [7:0] m1out,m2out, m3out,m4out, a1out,a2out;
	
	
	wire [7:0] m1a1out, m4a2out, m3a1out;


        FIR_ctrl ctrl(reset, clk, x, r1out, m1out,m3out,m4out, a1out,a2out, y,x0s,x1s,x2s,x3s,x4s,x5s,x6s,x7s,x8s,x9s,x10s,x11s,r1en,r2en,r3en,r4en, m1a1out, m4a2out, m3a1out, x1,x2,x3, x5, x6, x7, a0,a1,a2,a3, a4, a5, a6, a7);
        
	mult8 m1 (s0, s1, m1out);
	mult8 m2 (s2, s3, m2out);
	mult8 m3 (s4, s5, m3out);
	mult8 m4 (s6, s7, m4out);
	
	adder8 a01 (s8, s9, a1out);
	adder8 a02 (s10, s11, a2out);
       
	
	mux2 x00(x, x6, x0s, s0);
	mux2 x01(a0,a6, x1s, s1);
	mux2 x02(x1, x7, x2s, s2);  
	mux2 x03(a1, a7, x3s, s3); 
	mux2 x04(x2, x5, x4s, s4); 
	mux2 x05(a2, a5, x5s, s5);  
	mux2 x06(x3, r1out, x6s, s6); 
	mux2 x07(a3, a4, x7s, s7);
	mux2 x08(r1out, r3out, x8s, s8);
	mux2 x09(r2out, r4out, x9s, s9);
	mux2 x010(r1out, r3out, x10s, s10);
	mux2 x011(r2out, r4out, x11s, s11);
	
	reg8 r1(reset, r1en, clk, m1a1out, r1out);
	reg8 r2(reset, r2en, clk, m2out, r2out);
	reg8 r3(reset, r3en, clk, m3a1out, r3out);
	reg8 r4(reset, r4en, clk, m4a2out, r4out);
	

endmodule
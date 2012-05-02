// cell spu register file
module regfile #(parameter RFWIDTH = 128, WIDTH = 32, REGBITS = 7)
                (input                clk, 
                 input                regwrite,
//                 input  [REGBITS-1:0] ra1, ra2, rt, wa,
		 input  i_en_0, i_en_1, even_or_odd_0, even_or_odd_1,
                 input  [REGBITS-1:0] ra_0, ra_1, rb_0, rb_1, rt_0, rt_1, wa,
                 input  [RFWIDTH-1:0] wd,
		 input                i10_en_0, i10_en_1, i16_en_0, i16_en_1,
                 input  [9:0]         i10_0, i10_1,
		 input  [15:0]        i16_0, i16_1,
		 output reg [2:0]     evencont, oddcont,
                 output reg [RFWIDTH-1:0]   rd1, rd2);

   // 128 bits * 128 GPRs
   reg  [RFWIDTH-1:0] RAM [127:0];
   wire	[RFWIDTH-1:0]	    imm_0, imm_1;

   // previous instruction rt
   reg [6:0] 	    pre_rt_0, pre_rt_1;
   
   assign imm0 = {112'b0, i16_0};
   assign imm1 = {112'b0, i16_1};

   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0
   always @(posedge clk)
     begin
	if (i16_en_0)
	  begin
	     RAM[rt_0] <= imm0;
	     $display("load imm0 %d into reg %d", imm0, rt_0);
	  end

	if (i16_en_1)
	  begin
	     RAM[rt_1] <= imm1;
	     $display("load imm1 %d into reg %d", imm_1, rt_1);
	  end
	
        if (regwrite) RAM[wa] <= wd;

	rd1 <= RAM[ra_0];
	rd2 <= RAM[rb_0];

	evencont <= 3'b010;

	if (~wd[127]) RAM[pre_rt_0] = wd;
	pre_rt_0 = rt_0;
     end

//   assign rd1 = ra1 ? RAM[ra1] : 0;
//   assign rd2 = ra2 ? RAM[ra2] : 0;
endmodule


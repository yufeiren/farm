// cell spu register file
module regfile #(parameter RFWIDTH = 128, WIDTH = 32, REGBITS = 7)
                (input                clk, 
                 input                regwrite,
		 input  i_en_0, i_en_1, even_or_odd_0, even_or_odd_1,
                 input  [REGBITS-1:0] ra_0, ra_1, rb_0, rb_1, rt_0, rt_1, wa,
                 input  [RFWIDTH-1:0] wd_even, wd_odd,
		 input                i10_en_0, i10_en_1, i16_en_0, i16_en_1,
                 input  [9:0]         i10_0, i10_1,
		 input  [15:0]        i16_0, i16_1,
		 output reg [2:0]     cont0, cont1,
                 output [RFWIDTH-1:0]   rd00, rd01, rd10, rd11,
                 output even_mux_sel, odd_mux_sel);

   // 128 bits * 128 GPRs
   reg  [RFWIDTH-1:0] RAM [127:0];
   wire	[RFWIDTH-1:0]	    imm0, imm1;

   // previous instruction rt
   reg [6:0] 	    pre_rt_even, pre_rt_odd;
   
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
	     $display("load imm1 %d into reg %d", imm1, rt_1);
	  end
	
        // if (regwrite) RAM[wa] <= wd;

	cont0 <= 3'b010;
	cont1 <= 3'b010;

	if (~wd_even[127]) RAM[pre_rt_even] = wd_even;
	if (~wd_odd[127]) RAM[pre_rt_odd] = wd_odd;

	pre_rt_even = rt_0;
	pre_rt_odd = rt_1;
     end

     // first instruction output
     assign rd00 = ra_0 ? RAM[ra_0] : 0;
     assign rd01 = rb_0 ? RAM[rb_0] : 0;
     assign even_mux_sel = 0;
   
     // second instruction output
     assign rd10 = ra_1 ? RAM[ra_1] : 0;
     assign rd11 = rb_1 ? RAM[rb_1] : 0;
     assign odd_mux_sel = 1;

endmodule


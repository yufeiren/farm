// cell spu register file
// module regfile #(parameter WIDTH = 128, REGBITS = 7)
module regfile #(parameter WIDTH = 32, REGBITS = 7)
                (input                clk, 
                 input                regwrite, 
                 input  [REGBITS-1:0] ra1, ra2, rt, wa,
                 input  [WIDTH-1:0]   wd,
		 input                i10_en, i16_en,
                 input  [9:0]         i10,
		 input  [15:0]        i16,
		 output reg [2:0]         evencont, oddcont,
                 output reg [127:0]   rd1, rd2);

   // 128 bits * 128 GPRs
   reg  [127:0] RAM [127:0];
   wire	[127:0]	    imm;

   assign imm = {112'b0, i16};

   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0
   always @(posedge clk)
     begin
	if (i16_en)
	  begin
	     RAM[rt] <= imm;
	     $display("load imm %d into reg %b", imm, rt);
	  end
	
        if (regwrite) RAM[wa] <= wd;

	rd1 <= RAM[ra1];
	rd2 <= RAM[ra2];

	evencont <= 3'b010;
	
     end

//   assign rd1 = ra1 ? RAM[ra1] : 0;
//   assign rd2 = ra2 ? RAM[ra2] : 0;
endmodule


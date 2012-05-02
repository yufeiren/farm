// Yufei Ren (yufren@ic.sunysb.edu)
// Cell SPU processor - Lite edition implementation
// for ESE 545 project
module cellspu #(parameter RFWIDTH = 128, WIDTH = 32, REGBITS = 7)
             (input              clk, reset,
              input  [WIDTH-1:0] instrdata0, instrdata1,
	      input  [RFWIDTH-1:0] memdata, writedata,
              output             memread, memwrite, 
              output [WIDTH-1:0] adr);

   // wire: connect the controller and datapath
   wire [31:0] instr0, instr1;
   wire        zero, alusrca, memtoreg, iord, pcen, regwrite, regdst;
   wire [1:0]  aluop,pcsource,alusrcb;
   wire [3:0]  irwrite;
   wire [2:0]  alucont;

   // controller - instr bits from 0:10 is opcode
//   controller  cont(clk, reset, instr0[31:21], zero, memread, memwrite, 
//                    alusrca, memtoreg, iord, pcen, regwrite, regdst,
//                    pcsource, alusrcb, aluop, irwrite);

   // alu controller?
   alucontrol  ac(aluop, instr0[5:0], alucont);

   // datapath
   datapath    #(RFWIDTH, WIDTH, REGBITS)
               dp(clk, reset, instrdata0, instrdata1, memdata, alusrca, memtoreg, iord, pcen,
                  regwrite, regdst, pcsource, alusrcb, irwrite, alucont,
                  zero, instr0, instr1, adr, writedata);

endmodule // cellspu


module datapath #(parameter RFWIDTH = 128, WIDTH = 32, REGBITS = 7)
                 (input              clk, reset, 
                  input  [WIDTH-1:0] memdata0, memdata1,
		  input  [RFWIDTH-1:0] memdata,
                  input              alusrca, memtoreg, iord, pcen, regwrite, regdst,
                  input  [1:0]       pcsource, alusrcb, 
                  input  [3:0]       irwrite, 
                  input  [2:0]       alucont, 
                  output             zero, 
                  output [31:0]      instr0, instr1,
                  output [WIDTH-1:0] adr,
                  output [RFWIDTH-1:0] writedata);

   localparam CONST_ZERO = 32'b0;
   localparam CONST_ONE =  32'b1;

   wire [REGBITS-1:0] ra1, ra2, wa, ra, rb, rt;
   wire [WIDTH-1:0]   pc, nextpc, rd1, rd2, wd, a, src1, src2, aluresult,
                          aluout, constx4;
   // from rf to mux
   wire [2:0] 	      cont0, cont1;
   // from mux to even pipe or odd pipe
   wire [2:0] 	      even_mode, odd_mode;
   
   wire [15:0] 	      i16_0, i16_1;
   wire [9:0] 	      i10_0, i10_1;
   wire               i10_en_0, i16_en_1;
   wire [6:0]         reg_ra_0, reg_rb_0, reg_rt_0, reg_ra_1, reg_rb_1, reg_rt_1;
   // from rf to mux
   wire [RFWIDTH-1:0]       reg_rd00, reg_rd01, reg_rd10, reg_rd11;
   // from mux to even pipe or odd pipe
   wire [RFWIDTH-1:0]       even_rd0, even_rd1, odd_rd0, odd_rd1;
   wire [RFWIDTH-1:0]       ep_result, ep_out; // even pipe result
   wire [RFWIDTH-1:0] 	    op_result, op_out; // odd pipe result
   wire [RFWIDTH-1:0] 	    md;

   wire 		    i_en_0, i_en_1, even_or_odd_0, even_or_odd_1;

   // mux selection signal
   wire                     even_mux_sel, odd_mux_sel;

   // structure hazard is determined by decode stage
   wire                     struc_hazard;
   
   // according to instruction type
   // get register file address fields
   // assign ra1 = instr[REGBITS+20:21];
   // assign ra2 = instr[REGBITS+15:16];
   // mux2       #(REGBITS) regmux(instr[REGBITS+15:16], instr[REGBITS+10:11], regdst, wa);

//   assign ra = instr[20:14];
//   assign rb = instr[13:7];
//   assign rt = instr[6:0];
   
   // load instruction into buffer - instr
//   flopen #(32) ir(clk, irwrite[0], memdata[31:0], instr[31:0]);
//   flopen #(WIDTH) ir0(clk, 1, memdata0[31:0], instr0[31:0]);
//   flopen #(WIDTH) ir1(clk, 1, memdata1[31:0], instr1[31:0]);
   mux2       #(WIDTH)  ir0(memdata0[31:0], 32'b0, 0, instr0);
   mux2       #(WIDTH)  ir1(memdata1[31:0], 32'b0, 0, instr1);
    
   // decode and check if there is any hazard
   decodeen #(WIDTH) id(clk, 1, instr0[31:0], instr1[31:0], i_en_0, i_en_1, even_or_odd_0, even_or_odd_1, reg_ra_0, reg_ra_1, reg_rb_0, reg_rb_1, reg_rt_0, reg_rt_1, i10_en_0, i10_en_1, i16_en_0, i16_en_1, i10_0, i10_1, i16_0, i16_1, struc_hazard);
   
   // datapath
   // program counter
   pcalu      #(WIDTH)  pcalu(clk, reset, 1'b1, struc_hazard, pc, nextpc);
   flopenr    #(WIDTH)  pcreg(clk, reset, 1'b1, nextpc, pc);

   // memory data register
   flop       #(RFWIDTH)  mdr(clk, memdata, md);

   // even pipe out flop
   flop       #(RFWIDTH)  epres(clk, ep_result, ep_out);
   //  odd pipe out flop
   flop       #(RFWIDTH)  opres(clk, op_result, op_out);
   
   flop       #(WIDTH)  areg(clk, rd1, a);	
//   flop       #(WIDTH)  wrd(clk, rd2, writedata);
   flop       #(WIDTH)  res(clk, aluresult, aluout);
//   mux2       #(WIDTH)  adrmux(pc, aluout, iord, adr);
   mux2       #(WIDTH)  adrmux(pc, aluout, 0, adr);
   mux2       #(WIDTH)  src1mux(pc, a, alusrca, src1);
//   mux4       #(WIDTH)  src2mux(writedata, CONST_ONE, instr[WIDTH-1:0], 
//                                constx4, alusrcb, src2);
//   mux4       #(WIDTH)  pcmux(aluresult, aluout, constx4, CONST_ZERO, pcsource, nextpc);
//   mux2       #(WIDTH)  wdmux(aluout, md, memtoreg, wd); today

   // mux for even pipe
   mux2 #(RFWIDTH) even_ra_mux(reg_rd00, reg_rd10, even_mux_sel, even_rd0);
   mux2 #(RFWIDTH) even_rb_mux(reg_rd01, reg_rd11, even_mux_sel, even_rd1);
   mux2 #(3)       even_cont_mux(cont0, cont1, even_mux_sel, even_mode);
   
   // mux for odd pipe
   mux2 #(RFWIDTH) odd_ra_mux(reg_rd00, reg_rd10, odd_mux_sel, odd_rd0);
   mux2 #(RFWIDTH) odd_rb_mux(reg_rd01, reg_rd11, odd_mux_sel, odd_rd1);
   mux2 #(3)       odd_cont_mux(cont0, cont1, odd_mux_sel, odd_mode);
   
   // register file
   regfile    #(RFWIDTH,WIDTH,REGBITS) rf(clk, regwrite, i_en_0, i_en_1, even_or_odd_0, even_or_odd_1, reg_ra_0, reg_ra_1, reg_rb_0, reg_rb_1, reg_rt_0, reg_rt_1, wa, ep_out, op_out, i10_en_0, i10_en_1, i16_en_0, i16_en_1, i10_0, i10_1, i16_0, i16_1, cont0, cont1, reg_rd00, reg_rd01, reg_rd10, reg_rd11, even_mux_sel, odd_mux_sel);

   // even pipe integer unit
   epfx       #(RFWIDTH) evenpipe(even_rd0, even_rd1, even_mode, ep_result);
   opls       #(RFWIDTH) oddpipe(odd_rd0, odd_rd1, odd_mode, op_result);

   zerodetect #(WIDTH) zd(aluresult, zero);

endmodule // datapath

module flop #(parameter WIDTH = 32)
             (input                  clk, 
              input      [WIDTH-1:0] d, 
              output reg [WIDTH-1:0] q);

   always @(posedge clk)
      q <= d;
endmodule

		   
module flopen #(parameter WIDTH = 32)
               (input                  clk, en,
                input      [WIDTH-1:0] d, 
                output reg [WIDTH-1:0] q);

   always @(posedge clk)
   //begin
      if (en) q <= d;
   //$display("%b",q);
   //$finish;
   //end
endmodule

module flopenr #(parameter WIDTH = 32)
                (input                  clk, reset, en,
                 input      [WIDTH-1:0] d, 
                 output reg [WIDTH-1:0] q);
 
//   always @(posedge clk)
   always @(*)
      if      (reset) q <= 0;
      else if (en)    q <= d;
endmodule

module pcalu #(parameter WIDTH = 32)
                (input                  clk, reset, en, str_haz,
                 input      [WIDTH-1:0] d, 
                 output reg [WIDTH-1:0] q);

   wire [WIDTH-1:0] 			nextpc;

   assign nextpc = d + 8;
   
   always @(posedge clk)
      if      (reset) q <= 0;
      else if (en)
	begin
	   if (str_haz)
	     q <= nextpc - 4;
	   else
	     q <= nextpc;
	end

endmodule

module mux2 #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] d0, d1, 
              input              s, 
              output [WIDTH-1:0] y);

   assign y = s ? d1 : d0; 
endmodule

module mux4 #(parameter WIDTH = 32)
             (input      [WIDTH-1:0] d0, d1, d2, d3,
              input      [1:0]       s, 
              output reg [WIDTH-1:0] y);

   always @(*)
      case(s)
         2'b00: y <= d0;
         2'b01: y <= d1;
         2'b10: y <= d2;
         2'b11: y <= d3;
      endcase
endmodule

module epfx #(parameter WIDTH = 128)
            (input      [WIDTH-1:0] a, b, 
             input      [2:0]       alucont, 
             output reg [WIDTH-1:0] result);

   wire     [WIDTH-1:0] b2, sum, slt;

   assign b2 = alucont[2] ? ~b:b; 
   assign sum = a + b2 + alucont[2];
   // slt should be 1 if most significant bit of sum is 1
   assign slt = sum[WIDTH-1];

   always@(*)
      case(alucont[1:0])
         2'b00: result <= a & b; // asynchronize way
         2'b01: result <= a | b;
         2'b10: result <= sum;
         2'b11: result <= slt;
      endcase
endmodule

module opls #(parameter WIDTH = 128)
            (input      [WIDTH-1:0] a, b, 
             input      [2:0]       alucont, 
             output reg [WIDTH-1:0] result);

   wire     [WIDTH-1:0] slw;
   
   always@(*)
      case(alucont[1:0])
         2'b00: result <= a & b; // asynchronize way
         2'b01: result <= a | b;
	 2'b10: result <= a;
      endcase
endmodule


module zerodetect #(parameter WIDTH = 32)
                   (input [WIDTH-1:0] a, 
                    output            y);

   assign y = (a==0);
endmodule
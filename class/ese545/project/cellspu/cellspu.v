// Yufei Ren (yufren@ic.sunysb.edu)
// Cell SPU processor - Lite edition implementation
// for ESE 545 project
module cellspu #(parameter WIDTH = 32, REGBITS = 7)
             (input              clk, reset, 
              input  [WIDTH-1:0] memdata, 
              output             memread, memwrite, 
              output [WIDTH-1:0] adr, writedata);

   // wire: connect the controller and datapath
   wire [31:0] instr;
   wire        zero, alusrca, memtoreg, iord, pcen, regwrite, regdst;
   wire [1:0]  aluop,pcsource,alusrcb;
   wire [3:0]  irwrite;
   wire [2:0]  alucont;

   // controller - instr bits from 0:10 is opcode
   controller  cont(clk, reset, instr[31:26], zero, memread, memwrite, 
                    alusrca, memtoreg, iord, pcen, regwrite, regdst,
                    pcsource, alusrcb, aluop, irwrite);

   // alu controller?
   alucontrol  ac(aluop, instr[5:0], alucont);

   // datapath
   datapath    #(WIDTH, REGBITS) 
               dp(clk, reset, memdata, alusrca, memtoreg, iord, pcen,
                  regwrite, regdst, pcsource, alusrcb, irwrite, alucont,
                  zero, instr, adr, writedata);

endmodule // cellspu


module datapath #(parameter WIDTH = 32, REGBITS = 7)
                 (input              clk, reset, 
                  input  [WIDTH-1:0] memdata, 
                  input              alusrca, memtoreg, iord, pcen, regwrite, regdst,
                  input  [1:0]       pcsource, alusrcb, 
                  input  [3:0]       irwrite, 
                  input  [2:0]       alucont, 
                  output             zero, 
                  output [31:0]      instr, 
                  output [WIDTH-1:0] adr, writedata);

   localparam CONST_ZERO = 32'b0;
   localparam CONST_ONE =  32'b1;

   wire [REGBITS-1:0] ra1, ra2, wa;
   wire [WIDTH-1:0]   pc, nextpc, md, rd1, rd2, wd, a, src1, src2, aluresult,
                          aluout, constx4;

   // according to instruction type
   // get register file address fields
   assign ra1 = instr[REGBITS+20:21];
   assign ra2 = instr[REGBITS+15:16];
   mux2       #(REGBITS) regmux(instr[REGBITS+15:16], instr[REGBITS+10:11], regdst, wa);

   // load instruction into register
   flopen #(32) ir(clk, irwrite[0], memdata[31:0], instr[31:0]);

   // datapath
   // program counter
   flopenr    #(WIDTH)  pcreg(clk, reset, pcen, nextpc, pc);
   // memory data register
   flop       #(WIDTH)  mdr(clk, memdata, md);
   
   flop       #(WIDTH)  areg(clk, rd1, a);	
   flop       #(WIDTH)  wrd(clk, rd2, writedata);
   flop       #(WIDTH)  res(clk, aluresult, aluout);
   mux2       #(WIDTH)  adrmux(pc, aluout, iord, adr);
   mux2       #(WIDTH)  src1mux(pc, a, alusrca, src1);
   mux4       #(WIDTH)  src2mux(writedata, CONST_ONE, instr[WIDTH-1:0], 
                                constx4, alusrcb, src2);
   mux4       #(WIDTH)  pcmux(aluresult, aluout, constx4, CONST_ZERO, pcsource, nextpc);
   mux2       #(WIDTH)  wdmux(aluout, md, memtoreg, wd);
   // register file
   regfile    #(WIDTH,REGBITS) rf(clk, regwrite, ra1, ra2, wa, wd, rd1, rd2);
   // even pipe integer unit
   epfx       #(WIDTH) evenpipeinteger(src1, src2, alucont, aluresult);
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
      if (en) q <= d;
endmodule

module flopenr #(parameter WIDTH = 32)
                (input                  clk, reset, en,
                 input      [WIDTH-1:0] d, 
                 output reg [WIDTH-1:0] q);
 
   always @(posedge clk)
      if      (reset) q <= 0;
      else if (en)    q <= d;
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

module epfx #(parameter WIDTH = 32)
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

module zerodetect #(parameter WIDTH = 32)
                   (input [WIDTH-1:0] a, 
                    output            y);

   assign y = (a==0);
endmodule
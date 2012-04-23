// external memory accessed by cellspu
// in SPU architecture, the memory means Local Storage
module exmemory #(parameter WIDTH = 32)
   (input clk,
    input memwrite,
    input [WIDTH-1:0] adr, writedata,
    output reg [WIDTH-1:0] memdata);

   reg [31:0] 		   RAM[(1<<WIDTH-2)-1:0];
   wire [31:0] 		   word;

   initial
     begin
	$readmemh("memoryhex.dat",RAM);
     end

   // read and write bytes from 32-bit word
   always @(posedge clk)
     if (memwrite)
       case (adr[1:0])
	 2'b00: RAM[adr>>2][7:0] <= writedata;
	 2'b01: RAM[adr>>2][15:8] <= writedata;
	 2'b10: RAM[adr>>2][23:16] <= writedata;
	 2'b11: RAM[adr>>2][31:24] <= writedata;
       endcase // case (adr[1:0])

   assign word = RAM[adr>>2];
   always @(*)
     case (adr[1:0])
       2'b00: memdata <= word[31:24];
       2'b01: memdata <= word[23:16];
       2'b10: memdata <= word[15:8];
       2'b11: memdata <= word[7:0];
     endcase // case (adr[1:0])
endmodule // exmemory

// Cell SPU processor
module cellspu #(parameter WIDTH = 32, REGBITS = 7)
             (input              clk, reset, 
              input  [WIDTH-1:0] memdata, 
              output             memread, memwrite, 
              output [WIDTH-1:0] adr, writedata);


endmodule // cellspu




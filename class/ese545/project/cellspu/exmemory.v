// external memory accessed by cellspu
// in SPU architecture, the memory means Local Storage
// in each clock cycle, the memory module send out
// a 32 bit word to the out world
module exmemory #(parameter WIDTH = 32)
   (input clk,
    input memread, memwrite,
    input [WIDTH-1:0] adr, writedata,
    output reg [WIDTH-1:0] memdata);

//   reg [31:0] 		   RAM[(1<<WIDTH-2)-1:0];
   reg [31:0] 		   RAM[4095:0]; // 16KB memory array
   wire [31:0] 		   word;

   initial
     begin
	$readmemh("memoryhex.dat",RAM);
     end

   // read and write bytes from 32-bit word
   always @(posedge clk)
     begin
     if (memwrite)
       RAM[adr>>2] <= writedata;
     if (memread)
       memdata <= RAM[adr>>2];
     end
   
//   assign word = RAM[adr>>2];
//   always @(*)
//     memdata <= word[31:0];  non-block mode
endmodule // exmemory


// external memory accessed by cellspu
// in SPU architecture, the memory means Local Storage
// in each clock cycle, the memory module send out
// for instruction, it sends out 2 * 32 bits instruction each time
// for data, it sends out 128 bits data each time
module exmemory #(parameter RFWIDTH = 128, WIDTH = 32)
   (input clk,
    input fetchinstr, memread, memwrite,
    input [WIDTH-1:0] adr,
    input [RFWIDTH-1:0] writedata,
    output reg [WIDTH-1:0] instr0, instr1,
    output reg [RFWIDTH-1:0] memdata);

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
       begin
	  RAM[(adr>>2)+0] <= writedata[127:96];
	  RAM[(adr>>2)+1] <= writedata[95:64];
	  RAM[(adr>>2)+2] <= writedata[63:32];
	  RAM[(adr>>2)+3] <= writedata[31:0];
       end

     if (memread)
       begin
	  memdata[127:96] <= RAM[adr>>2 + 0];
	  memdata[95:64] <= RAM[adr>>2 + 1];
	  memdata[63:32] <= RAM[adr>>2 + 2];
	  memdata[31:0] <= RAM[adr>>2 + 3];
       end
	
     if (fetchinstr)
       begin
	  instr0 <= RAM[adr>>2];
	  instr1 <= RAM[(adr>>2)+1];
       end
       // memdata <= RAM[adr>>2];
     end
   
//   assign word = RAM[adr>>2];
//   always @(*)
//     memdata <= word[31:0];  non-block mode
endmodule // exmemory


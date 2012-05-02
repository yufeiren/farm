// top level design for testing
module top #(parameter RFWIDTH = 128, WIDTH = 32, REGBITS = 7)();
   reg clk;
   reg reset;
   wire memread, memwrite;
   wire [WIDTH-1:0] adr;
   wire [WIDTH-1:0] instr0, instr1;
   wire [RFWIDTH-1:0] memdata, writedata;

   // instantiate devices to be tested
   cellspu #(RFWIDTH, WIDTH,REGBITS) spulite(clk, reset, instr0, instr1, memdata, writedata, memread, memwrite, adr);

   // external memory for instruction and data
   exmemory #(RFWIDTH, WIDTH) exmem(clk, 1, 0, 0, adr, writedata, instr0, instr1, memdata);
   
   // initialize test
   initial
     begin
        $dumpfile("top.vcd");
        $dumpvars(0,top);
	reset <= 1; # 7; reset <=0;
     end

   //generate clock to sequence tests
    always
      begin
	 clk <= 1; # 5; clk <= 0; # 5;
      end

   always@(negedge clk)
     begin
	if (memwrite)
	  if (adr == 5 & writedata == 7)
	    $display("Simulation complete successful");
	  else $display("Simulation failed");
     end
endmodule // top


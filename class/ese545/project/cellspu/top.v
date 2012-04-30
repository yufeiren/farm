// top level design for testing
module top #(parameter WIDTH = 32, REGBITS = 7)();
   reg clk;
   reg reset;
   wire memread, memwrite;
   wire [WIDTH-1:0] adr, writedata;
   wire [WIDTH-1:0] memdata;

   // instantiate devices to be tested
   cellspu #(WIDTH,REGBITS) spulite(clk, reset, memdata, memread, memwrite, adr, writedata);

   // external memory for code and data
//   exmemory #(WIDTH) exmem(clk, memread, memwrite, adr, writedata, memdata);
   exmemory #(WIDTH) exmem(clk, 1, memwrite, adr, writedata, memdata);
   
   // initialize test
   initial
     begin
        $dumpfile("top.vcd");
        $dumpvars(0,top);
	reset <= 1; # 22; reset <=0;
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


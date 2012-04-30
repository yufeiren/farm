// controller
module controller(input clk, reset, 
                  input      [10:0] op, 
                  input            zero, 
                  output reg       memread, memwrite, alusrca, memtoreg, iord, 
                  output           pcen, 
                  output reg       regwrite, regdst, 
                  output reg [1:0] pcsource, alusrcb, aluop, 
                  output reg [3:0] irwrite);

   // stages defination
   parameter   FETCH   =  4'b0001;
   parameter   DECODE  =  4'b0010;
   parameter   ROUTE   =  4'b0011;
   parameter   MEMADR  =  4'b0110;
   parameter   LBRD    =  4'b0111;
   parameter   LBWR    =  4'b1000;
   parameter   SBWR    =  4'b1001;
   parameter   RTYPEEX =  4'b1010;
   parameter   RTYPEWR =  4'b1011;
   parameter   BEQEX   =  4'b1100;
   parameter   JEX     =  4'b1101;

   parameter   LB      =  6'b100000;
   parameter   SB      =  6'b101000;
   parameter   RTYPE   =  6'b0;
   parameter   BEQ     =  6'b000100;
   parameter   J       =  6'b000010;

   // Even pipe execution
   parameter   EVFP    =  4'b1001;   // fix point in even pipe
   parameter   EVFX    =  4'b1010;   // float point in even pipe
   parameter   EVBY    =  4'b1011;   // byte
   // Odd pipe execution
   parameter   ODLS    =  4'b1101;   // load store
   parameter   ODPM    =  4'b1110;   // permute
   parameter   ODBR    =  4'b1111;   // branch
   
   // Instruction Opcode
   parameter   LQX     = 11'b00111000100;     // load quadword (x-form)
   parameter   STQX    = 11'b00101000100;     // store quadword (x-form)
   parameter   IL      =  9'b010000001;       // immediate load word
   parameter   ILEX    = 11'b01000000100;     // immediate load word
   parameter   AH      = 11'b00011001000;     // add halfword
   parameter   A       = 11'b00011000000;     // add word
   parameter   AI      =  8'b00011100;        // add word immediate
   parameter   AIEX    = 11'b00011100000;     // add word immediate
   parameter   SF      = 11'b00010000000;     // subtract from word
   parameter   SFI     =  8'b00001100;        // subtract from word immediate
   parameter   SFIEX   = 11'b00001100000;     // subtract from word immediate
   parameter   MPY     = 11'b01111000100;     // multiply
   parameter   MPYI    =  8'b01110100;        // multiply immediate
   parameter   MPYIEX  = 11'b01110100000;     // multiply immediate
   parameter   AVGB    = 11'b00011010011;     // average bytes
   parameter   ABSDB   = 11'b00001010011;     // absolute differences of bytes
   parameter   GBB     = 11'b00110110010;     // gather bits from bytes
   parameter   AND     = 11'b00011000001;     // and
   parameter   OR      = 11'b00001000001;     // or
   parameter   XOR     = 11'b01001000001;     // exclusive or
   parameter   NAND    = 11'b00011001001;     // nand
   parameter   NOR     = 11'b00001001001;     // nor
   parameter   SHL     = 11'b00001011011;     // shift left word
   parameter   ROT     = 11'b00001011000;     // rotate word

   parameter   BR      =  9'b001100100;       // branch relative
   parameter   BREX    = 11'b00110010000;       // branch relative
   parameter   BRA     =  9'b001100000;       // branch absolute
   parameter   BRNZ    =  9'b001000010;       // branch if not zero word
   parameter   BRHNZ   =  9'b001000110;       // branch if not zero halfword
   parameter   HBR     = 11'b00110101100;     // hint for branch (r-form)

   parameter   FA      = 11'b01011000100;     // floating add
   parameter   FS      = 11'b01011000101;     // floating subtract
   parameter   FM      = 11'b01011000110;     // floating multiply
   parameter   FCEQ    = 11'b01111000010;     // floating compare equal
   parameter   FCGT    = 11'b01011000010;     // floating compare greater than
   
   reg [3:0] state, nextstate;
   reg       pcwrite, pcwritecond;

   reg	[10:0]	    newop;

   // state register
   always @(posedge clk)
      if(reset) state <= FETCH;
      else state <= nextstate;

   // next state logic
   always @(*)
      begin
         // set all the opcode to 11 bits
         case(op[10:2])
           IL:    newop <= {IL, 2'b0};
           AI:    newop <= {AI, 3'b0};
	   SFI:   newop <= {SFI, 3'b0};
	   MPYI:  newop <= {MPYI, 3'b0};
	   BR:    newop <= {BR, 2'b0};
         endcase  

         case(state)
            FETCH:   nextstate <= DECODE;
            DECODE:  case(newop)
		        ILEX:    nextstate <= MEMADR;
                        A:       nextstate <= EVFX;
                        LB:      nextstate <= MEMADR;
                        SB:      nextstate <= MEMADR;
                        RTYPE:   nextstate <= RTYPEEX;
                        BEQ:     nextstate <= BEQEX;
                        J:       nextstate <= JEX;
                        default: nextstate <= FETCH; // should never happen
                     endcase
            EVFX:    nextstate <= FETCH;
            MEMADR:  case(newop)
		        ILEX:    nextstate <= LBRD;
                        LB:      nextstate <= LBRD;
                        SB:      nextstate <= SBWR;
                        default: nextstate <= FETCH; // should never happen
                     endcase
            LBRD:    nextstate <= LBWR;
            LBWR:    nextstate <= FETCH;
            SBWR:    begin
                     nextstate <= FETCH;
                     $display("next is SBWR");
	             end
            RTYPEEX: nextstate <= RTYPEWR;
            RTYPEWR: nextstate <= FETCH;
            BEQEX:   nextstate <= FETCH;
            JEX:     nextstate <= FETCH;
            default: nextstate <= FETCH; // should never happen
         endcase
      end

   always @(*)
      begin
            // set all outputs to zero, then conditionally assert just the appropriate ones
            irwrite <= 4'b0000;
            pcwrite <= 0; pcwritecond <= 0;
            regwrite <= 0; regdst <= 0;
            memread <= 0; memwrite <= 0;
            alusrca <= 0; alusrcb <= 2'b00; aluop <= 2'b00;
            pcsource <= 2'b00;
            iord <= 0; memtoreg <= 0;
            case(state)
               FETCH:
                  begin
                     memread <= 1; 
                     irwrite <= 4'b0001; // change to reflect new memory
                     alusrcb <= 2'b01;   // get the IR bits in the right spots
                     pcwrite <= 1;       // FETCH 2,3,4 also changed... 
                  end
               DECODE: alusrcb <= 2'b11;
               MEMADR:
                  begin
                     alusrca <= 1;
                     alusrcb <= 2'b10;
                  end
               LBRD:
                  begin
                     memread <= 1;
                     iord    <= 1;
                  end
               LBWR:
                  begin
                     regwrite <= 1;
                     memtoreg <= 1;
                  end
               SBWR:
                  begin
                     memwrite <= 1;
                     iord     <= 1;
                  end
               RTYPEEX: 
                  begin
                     alusrca <= 1;
                     aluop   <= 2'b10;
                  end
               RTYPEWR:
                  begin
                     regdst   <= 1;
                     regwrite <= 1;
                  end
               BEQEX:
                  begin
                     alusrca     <= 1;
                     aluop       <= 2'b01;
                     pcwritecond <= 1;
                     pcsource    <= 2'b01;
                  end
               JEX:
                  begin
                     pcwrite  <= 1;
                     pcsource <= 2'b10;
                  end
         endcase
      end
   assign pcen = pcwrite | (pcwritecond & zero); // program counter enable
endmodule

module alucontrol(input      [1:0] aluop, 
                  input      [5:0] funct, 
                  output reg [2:0] alucont);

   always @(*)
      case(aluop)
         2'b00: alucont <= 3'b010;  // add for lb/sb
         2'b01: alucont <= 3'b110;  // sub (for beq)
         default: case(funct)       // R-Type instructions
                     6'b100000: alucont <= 3'b010; // add (for add)
                     6'b100010: alucont <= 3'b110; // subtract (for sub)
                     6'b100100: alucont <= 3'b000; // logical and (for and)
                     6'b100101: alucont <= 3'b001; // logical or (for or)
                     6'b101010: alucont <= 3'b111; // set on less (for slt)
                     default:   alucont <= 3'b101; // should never happen
                  endcase
      endcase
endmodule


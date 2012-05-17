module decodeen #(parameter WIDTH = 32)
		   (input clk, en,
		    input [WIDTH-1:0] instr0, instr1,
		    output reg i_en_0, i_en_1, even_or_odd_0, even_or_odd_1,
		    output reg [6:0] ra_0, ra_1, rb_0, rb_1, rt_0, rt_1,
		    output reg i10_en_0, i10_en_1, i16_en_0, i16_en_1,
		    output reg [9:0] i10_0, i10_1,
		    output reg [15:0] i16_0, i16_1,
                    output reg struc_hazard);

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

   reg	[10:0]	    newop0, newop1;

   always @(posedge clk)
      if (en)
      begin
	 i16_en_0 <= 0;
	 i16_en_1 <= 0;
	 i10_en_0 <= 0;
	 i10_en_1 <= 0;

         i_en_0 <= 1; // the previous instruction would execute first
         i_en_1 <= 0; // if no hazard, the next instruction could execute follows by
         even_or_odd_0 <= 0;
         even_or_odd_1 <= 1;

	 struc_hazard <= 0;

	 newop0 <= 11'b0;
	 newop1 <= 11'b0;
	 	 
	 // extend 8 bits and 9 bits opcode into 11 bits opcode
	 case(instr0[31:24])
	   AI:    newop0 <= {AI, 3'b0};
	   SFI:   newop0 <= {SFI, 3'b0};
	   MPYI:  newop0 <= {MPYI, 3'b0};
	   default: begin
	      case(instr0[31:23])
	        IL: begin
                  newop0 <= {IL, 2'b0};
                  assign even_or_odd_0 = 1;
		end
	      default: begin
		 case(instr0[31:21])
		   A: assign even_or_odd_0 = 0;
		 endcase // case (instr1[31:21])
	      end
	      endcase // case (instr0[31:23])
	   end
	 endcase // case (instr0[31:24])
	 
	 case(instr1[31:24])
	   AI:    newop1 <= {AI, 3'b0};
	   SFI:   newop1 <= {SFI, 3'b0};
	   MPYI:  newop1 <= {MPYI, 3'b0};
	   default: begin
	      case(instr1[31:23])
	        IL: begin
                  newop1 <= {IL, 2'b0};
                  assign even_or_odd_1 = 1;
		end
	      default: begin
		 case(instr1[31:21])
		   A: assign even_or_odd_1 = 0;
		 endcase // case (instr1[31:21])
	      end
	      endcase // case (instr1[31:23])
	      end
	 endcase // case (instr0[31:24])
	 
	 case(instr0[31:23])
	   IL: begin 
              assign newop0 = {IL, 2'b0};
              even_or_odd_0 <= 1;
	      end
	   BR:    newop0 <= {BR, 2'b0};
	   BRA:   newop0 <= {BRA, 2'b0};
	   default: newop0 <= instr0[31:21];
	 endcase // case (instr0[31:23])
	 case(instr1[31:23])
	   IL:   begin 
              assign newop0 = {IL, 2'b0};
              even_or_odd_0 <= 1;
	      end
	   BR:    newop1 <= {BR, 2'b0};
	   BRA:   newop1 <= {BRA, 2'b0};
	   default: newop1 <= instr1[31:21];
	 endcase // case (instr0[31:23])

	 // decode the two instruction to be executed in odd pipe or even pipe, to determine if there is structure hazard
	 case(newop0)
           AH: even_or_odd_0 <= 0;
           A: even_or_odd_0 <= 0;
           AIEX: even_or_odd_0 <= 0;
           SF: even_or_odd_0 <= 0;  
           SFIEX: even_or_odd_0 <= 0;
           MPY: even_or_odd_0 <= 0;
           MPYIEX: even_or_odd_0 <= 0;
           AVGB: even_or_odd_0 <= 0;
           ABSDB: even_or_odd_0 <= 0;
           GBB: even_or_odd_0 <= 0;  
           AND: even_or_odd_0 <= 0;
           OR: even_or_odd_0 <= 0;
           XOR: even_or_odd_0 <= 0;
           NAND: even_or_odd_0 <= 0;
           NOR: even_or_odd_0 <= 0;
           FA: even_or_odd_0 <= 0;
           FS: even_or_odd_0 <= 0;
           FM: even_or_odd_0 <= 0;
           FCEQ: even_or_odd_0 <= 0;
           FCGT: even_or_odd_0 <= 0;
// odd pipe
	   ILEX: even_or_odd_0 <= 1;
           LQX: even_or_odd_0 <= 1;
           STQX: even_or_odd_0 <= 1;
           SHL: even_or_odd_0 <= 1;
           ROT: even_or_odd_0 <= 1;
           BREX: even_or_odd_0 <= 1;
           HBR: even_or_odd_0 <= 1;
	 endcase // case (newop0)
	 case(newop1)
           AH: even_or_odd_1 <= 0;
           A: even_or_odd_1 <= 0;
           AIEX: even_or_odd_1 <= 0;
           SF: even_or_odd_1 <= 0;  
           SFIEX: even_or_odd_1 <= 0;
           MPY: even_or_odd_1 <= 0;
           MPYIEX: even_or_odd_1 <= 0;
           AVGB: even_or_odd_1 <= 0;
           ABSDB: even_or_odd_1 <= 0;
           GBB: even_or_odd_1 <= 0;  
           AND: even_or_odd_1 <= 0;
           OR: even_or_odd_1 <= 0;
           XOR: even_or_odd_1 <= 0;
           NAND: even_or_odd_1 <= 0;
           NOR: even_or_odd_1 <= 0;
           FA: even_or_odd_1 <= 0;
           FS: even_or_odd_1 <= 0;
           FM: even_or_odd_1 <= 0;
           FCEQ: even_or_odd_1 <= 0;
           FCGT: even_or_odd_1 <= 0;
// odd pipe
	   ILEX: even_or_odd_1 <= 1;
           LQX: even_or_odd_1 <= 1;
           STQX: even_or_odd_1 <= 1;
           SHL: even_or_odd_1 <= 1;
           ROT: even_or_odd_1 <= 1;
           BREX: even_or_odd_1 <= 1;
           HBR: even_or_odd_1 <= 1;
	 endcase // case (newop0)

	 // structure hazard, pc += 4 instead of pc += 8
	 if (even_or_odd_0 == even_or_odd_1)
	   begin
	      i_en_0 <= 1;
	      i_en_1 <= 0;
	      struc_hazard <= 0;
	   end
	 else // no structure hazard
	   begin
	      i_en_0 <= 1;
	      i_en_1 <= 1;
	   end
	 
	 
      // decode instr0	 
	// 9 bit instruction decode
         case(instr0[31:23])
            IL:
	       begin
	          rt_0 <= instr0[6:0];
	          i16_0 <= instr0[22:7];
	          i16_en_0 <= 1;
	       end
	 endcase // case (instr[31:23])
         case(instr1[31:23])
            IL:
	       begin
	          rt_1 <= instr1[6:0];
	          i16_1 <= instr1[22:7];
	          i16_en_1 <= 1;

                  i_en_1 <= 1;
	       end
	 endcase // case (instr[31:23])

        // 11 bit instruction decode	
	case(instr0[31:21])
          A:
            begin
               ra_0 <= instr0[20:14];
               rb_0 <= instr0[13:7];
               rt_0 <= instr0[6:0];
            end
	endcase // case (instr[31:21])
	case(instr1[31:21])
          A:
            begin
               ra_1 <= instr1[20:14];
               rb_1 <= instr1[13:7];
               rt_1 <= instr1[6:0];

               i_en_1 <= 1;
            end
	endcase // case (instr[31:21])
      end
endmodule // decode


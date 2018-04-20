// Verilog FIFO test bench

// You must modify this code to create a test vector to test
// Your FIFO.

// See lines 94-96 to push a UDP packet onto the FIFO

// The testbench can use all the verilog features (e.g. unbounded loops, $display, ) as it is only
// used to test the fifo module.

// This code is modified from: http://electrosofts.com/verilog/fifo.html

`define BUF_WIDTH 8
`define MAX_CYCLES 10000

module fifo_test();
reg clk, rst, wr_en, rd_en ;
reg[7:0] buf_in;
reg[7:0] tempdata;
wire [7:0] buf_out;
reg [31:0] cycle_counter;   // counter

reg [7:0] udp_packet [0:2047]; // example byte array. This is a 1-Dimensional array of 8-bit bytes
reg [7:0] 	packet_len ; // length of the packet
reg [31:0] 	i;          // 32 bit loop counter

// this instantiates a FIFO (creates a copy of one) called ff
// and connects the ports. e.g. the ff's srst is connected to rst

fifo ff( .clk(clk), .srst(rst), .din(buf_in), .dout(buf_out),
         .wr_en(wr_en), .rd_en(rd_en), .empty(buf_empty),
         .full(buf_full) );

initial
begin
   clk = 0;

   rst = 1;
   cycle_counter = 0;
   rd_en = 0;
   wr_en = 0;
   tempdata = 0;
   buf_in = 0;
   packet_len = 37 ;
   udp_packet[0] = 8'h45;
   udp_packet[1] = 8'h00;
   udp_packet[2] = 8'h00;
   udp_packet[3] = 8'h25;
   udp_packet[4] = 8'hce;
   udp_packet[5] = 8'h16;
   udp_packet[6] = 8'h40;
   udp_packet[7] = 8'h00;
   udp_packet[8] = 8'h40;
   udp_packet[9] = 8'h11;
   udp_packet[10]= 8'hf9;
   udp_packet[11]= 8'h41;
   udp_packet[12]= 8'hc0;
   udp_packet[13]= 8'ha8;
   udp_packet[14]= 8'h02;
   udp_packet[15]= 8'h0d;
   udp_packet[16]= 8'h42;
   udp_packet[17]= 8'h81;
   udp_packet[18]= 8'h6e;
   udp_packet[19]= 8'h39;
   udp_packet[20]= 8'h8e;
   udp_packet[21]= 8'h35;
   udp_packet[22]= 8'h7a;
   udp_packet[23]= 8'hb7;
   udp_packet[24]= 8'h00;
   udp_packet[25]= 8'h11;
   udp_packet[26]= 8'hf3;
   udp_packet[27]= 8'hfd;
   udp_packet[28]= 8'h52;
   udp_packet[29]= 8'h55;
   udp_packet[30]= 8'h52;
   udp_packet[31]= 8'h61;
   udp_packet[32]= 8'h68;
   udp_packet[33]= 8'h52;
   udp_packet[34]= 8'h61;
   udp_packet[35]= 8'h68;
   udp_packet[36]= 8'h21;

   #15 rst = 0;

        push(1);
        // fork
        //    push(2);
        //    pop(tempdata);
        // join              //push and pop together
        push(2);
        push(3);
        push(4);
        push(5);
        // push(30);
        // push(40);
        // push(50);
        // push(60);
        // push(70);
        // push(80);
        // push(90);
        // push(100);
        // push(110);
        // push(120);
        // push(8'hFF);
        // push(8'hFF);
   // Push a UDP packet
        // push(packet_len);
        // for (i = 0; i< packet_len; i++) begin
        //     push (udp_packet[i]);
        // end

        // pop(tempdata);
        // push(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // push(140);
        // pop(tempdata);
        // push(tempdata);//
        pop(tempdata);
        pop(tempdata);
        pop(tempdata);
        pop(tempdata);
        pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // pop(tempdata);
        // push(5);
        // pop(tempdata);
end

always begin
   #5 clk = ~clk;
end

always @(posedge clk) begin
   if (cycle_counter > `MAX_CYCLES) begin
      $finish;
   end
   else begin
     cycle_counter <= cycle_counter +1 ;
   end
end

task push;
input[7:0] data;


   if( buf_full )
            $display("---Cannot push: Buffer Full---");
        else
        begin
           $display("Pushed ",data );
           buf_in = data;
           wr_en = 1;
                @(posedge clk);
                #1 wr_en = 0;
        end

endtask

task pop;
output [7:0] data;

   if( buf_empty )
            $display("---Cannot Pop: Buffer Empty---");
   else
        begin

     rd_en = 1;
          @(posedge clk);

          #1 rd_en = 0;
          data = buf_out;
           $display("-------------------------------Poped ", data);
           #1 $display("--------- clock", cycle_counter);

        end
endtask

endmodule

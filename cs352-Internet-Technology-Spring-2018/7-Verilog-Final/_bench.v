
`define BUF_WIDTH 8
`define MAX_CYCLES 1000

module fifo_test();
reg clk, rst, wr_en, rd_en ;
reg[7:0] buf_in;
reg[7:0] tempdata;
wire [7:0] buf_out;
reg [31:0] cycle_counter;   // counter

reg [7:0] udp_packet [0:2047]; // example byte array. This is a 1-Dimensional array of 8-bit bytes
reg [7:0] 	packet_len ; // length of the packet
reg [31:0] 	i;          // 32 bit loop counter

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
   #15 rst = 0;

   push(196);
   push(111);
   push(147);
   push(244);
   push(99);
   push(2);
   push(37);
   push(150);
   push(244);
   push(141);
   

   i = 0;

   while (i < 10000) begin

    if (!buf_empty) begin
      pop(tempdata);
    end
    i += 1;
   end

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

   if( buf_empty)
            $display("---Cannot Pop: Buffer Empty---");
   else
        begin

     rd_en = 1;
          @(posedge clk);

          #1 rd_en = 0;
          data = buf_out;

          if (data != 0) begin
            $display("-----------------Poped", data, $time);
          end

          // $display("-----------------Poped", data, $time);

        end
endtask

endmodule


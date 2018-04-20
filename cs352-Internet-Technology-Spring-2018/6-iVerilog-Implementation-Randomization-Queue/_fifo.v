// Example verilog FIFO implementation that conforms to th Xillybus/Xilinx library version
//
// Your code must conform to this interface
//
// There is no initial cause, you must initialize the FIFO when srst is high
//
// modified from: http://electrosofts.com/verilog/fifo.html

`define BUF_WIDTH 8    // BUF_SIZE = 16 -> BUF_WIDTH = 4, no. of bits to be used in pointer
`define BUF_SIZE ( 1<<`BUF_WIDTH )

module fifo( clk, srst, din, dout, wr_en, rd_en, empty, full);

input                 srst, clk, wr_en, rd_en;
// synchronous reset, system clock, write enable and read enable.
input [7:0]           din;
// data input to be pushed to buffer
output[7:0]           dout;
// data output the data using pop.
output                empty, full;
// status lines if full or empty

reg[7:0]              dout;
reg                   empty, full;
reg[`BUF_WIDTH :0]    fifo_counter;
reg[`BUF_WIDTH -1:0]  rd_ptr, wr_ptr;           // pointer to read and write addresses
reg[7:0]              buf_mem[`BUF_SIZE -1 : 0]; //

reg[7:0]              delay = 90;

always @(fifo_counter)
begin
   empty = (fifo_counter==0);
   full = (fifo_counter== `BUF_SIZE);

end


always @(posedge clk or posedge srst)
begin
   if( srst )
       fifo_counter <= 0;
   else if( (!full && wr_en) && ( !empty && rd_en ) )
       fifo_counter <= fifo_counter;

   else if( !full && wr_en )
       fifo_counter <= fifo_counter + 1;

   else if( !empty && rd_en )
       fifo_counter <= fifo_counter - 1;
   else
      fifo_counter <= fifo_counter;

end

always @( posedge clk or posedge srst)
begin
   if( srst )
      dout <= 0;
   else
   begin
      if( rd_en && !empty )
        dout <= buf_mem[rd_ptr];
      else
         dout <= dout;
   end
end

always @(posedge clk)
begin

   if( wr_en && !full )
    buf_mem[ wr_ptr ] <= din;

   else
      buf_mem[ wr_ptr ] <= buf_mem[ wr_ptr ];
end

always@(posedge clk or posedge srst)
begin
   if( srst )
   begin
      wr_ptr <= 0;
      rd_ptr <= 0;
   end
   else
   begin
      if( !full && wr_en )

        wr_ptr <= wr_ptr + 1;

      else
        wr_ptr <= wr_ptr;

      if( !empty && rd_en )
        rd_ptr <= rd_ptr + 1;
      else
        rd_ptr <= rd_ptr;
   end

end
endmodule // fifo

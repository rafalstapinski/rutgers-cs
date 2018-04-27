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

reg[31:0]             lfsr = 987654321;
reg[1:0]              shifted = 0;
reg[7:0]              delay = 2;
reg[7:0]              delay_cap = 7;


always @(fifo_counter) begin

   empty = (fifo_counter==0);
   full = (fifo_counter== `BUF_SIZE);

end


always @(posedge clk or posedge srst) begin

  if (srst) begin

    fifo_counter <= 0;

  end else if( (!full && wr_en) && ( !empty && rd_en ) ) begin

    fifo_counter <= fifo_counter;

  end else if( !full && wr_en ) begin

    fifo_counter <= fifo_counter + 1;

  // add delay == 0, so that we count the pop only on the action
  end else if( !empty && rd_en && delay == 0 ) begin

    fifo_counter <= fifo_counter - 1;

  end else begin

    fifo_counter <= fifo_counter;

  end
end



always @( posedge clk or posedge srst) begin

  if (srst) begin

    dout <= 0;

  end else begin

    if( rd_en && !empty ) begin

      if (shifted == 1) begin

        lfsr[0] <= lfsr[1] ^ lfsr[13] ^ lfsr[0] ^ 1;
        shifted <= 0;
        delay <= delay;

        dout <= dout;

      end else begin

        lfsr <= lfsr;
        shifted <= shifted;
        delay <= delay;

        dout <= dout;

      end

      if (delay == 0) begin

        lfsr <= lfsr << 1;
        shifted <= 1;
        delay <= lfsr & delay_cap;

        dout <= buf_mem[rd_ptr];

      end else begin

        $display(delay);

        lfsr[31] <= lfsr[0];
        shifted <= 0;
        delay <= delay - 1;

        dout <= 0;

      end

    end else begin

      lfsr <= lfsr;
      shifted <= shifted;  
      delay <= delay;

      dout <= 0;

    end
  end
end

always @(posedge clk) begin

  if( wr_en && !full ) begin

    buf_mem[ wr_ptr ] <= din;

  end else begin

    buf_mem[ wr_ptr ] <= buf_mem[ wr_ptr ];

  end
end

always@(posedge clk or posedge srst) begin

  if( srst ) begin

    wr_ptr <= 0;
    rd_ptr <= 0;

   end else begin

    if( !full && wr_en ) begin

      wr_ptr <= wr_ptr + 1;

    end else begin

      wr_ptr <= wr_ptr;

    end
    // add delay == 0, so that we move the ptr only on the action
    if( !empty && rd_en && delay == 0 ) begin

      rd_ptr <= rd_ptr + 1;

    end else begin

      rd_ptr <= rd_ptr;

    end
  end
end

endmodule

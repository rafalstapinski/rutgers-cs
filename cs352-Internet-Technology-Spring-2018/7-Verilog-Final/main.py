import sys
import random
from subprocess import call

def usage():

    print '''
    python main.py <byte_order> <byte_amt> <packet_size> <delay>
        byte_order: seq (sequential), rand (random)
        byte_amt: amount of bytes, 1 -> n
        packet_size: size of packet in bytes, 1 -> n
        delay: max cycles of delay 2^n - 1 power, 1 -> n
    '''
    sys.exit()

fifo_code = '''
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
reg[7:0]              delay_cap = {};

reg[7:0]              packet_size = {};
reg[7:0]              packet_counter = {};
reg[7:0]              multiplier = 1;


always @(fifo_counter) begin

   empty = (fifo_counter == 0);
   full = (fifo_counter == `BUF_SIZE);

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


always @(posedge clk or posedge srst) begin

  if (srst) begin

    packet_size <= packet_size;

    lfsr <= lfsr;
    shifted <= 0;
    delay <= delay;

    dout <= 0;

  end else begin

    if( rd_en && !empty ) begin

      if (shifted == 1) begin

        packet_counter <= packet_counter;
        multiplier <= multiplier;

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

        if (packet_counter == 0) begin

          packet_counter <= packet_size;
          multiplier <= 5;

        end else begin

          packet_counter <= packet_counter;
          multiplier <= multiplier;

        end

        packet_counter <= packet_counter - 1;

        delay <= lfsr & delay_cap * multiplier;

        dout <= buf_mem[rd_ptr];

      end else begin

        // $display(delay);

        packet_counter <= packet_counter;
        multiplier <= multiplier;

        lfsr[31] <= lfsr[0];
        shifted <= 0;
        delay <= delay - 1;

        dout <= 0;

      end

    end else begin

      packet_counter <= packet_counter;
      multiplier <= multiplier;

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

always @(posedge clk or posedge srst) begin

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

'''

bench_code = '''
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

   {}

   i = 0;

   while (i < {}) begin

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

'''

if __name__ == '__main__':

    try:
        byte_order = sys.argv[1]
        byte_amt = int(sys.argv[2])
        packet_size = int(sys.argv[3])
        delay = (2 ** int(sys.argv[4])) - 1
    except:
        usage()

    pushes = ''

    if byte_order == 'seq':

        for i in range(byte_amt):

            pushes += 'push({});\n   '.format(i + 1)

    elif byte_order == 'rand':

        for i in range(byte_amt):

            pushes += 'push({});\n   '.format(random.randint(1, 255))

    else:
        usage()



    bench = bench_code.format(pushes, 10000)
    fifo = fifo_code.format(delay, packet_size, packet_size)

    with open('_bench.v', 'w') as f:
        f.write(bench)

    with open('_fifo.v', 'w') as f:
        f.write(fifo)

    call(['iverilog', '-o', '_bench.vvp', '_bench.v', '_fifo.v'])
    call(['vvp', '_bench.vvp'])

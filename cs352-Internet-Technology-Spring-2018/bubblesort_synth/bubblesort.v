// Skeleton start of a bubble sort in Verilog
//
// This small program is a start to an excercise and demonstration of how to use
// Verilog in a clock-driven style.
//
// (c) 2018 R. P. Martin, under GPL Version 2.

// This is analagous to a #define in C
`define MAX_CYCLES 20

module bubblesort();
// Variable section
    reg signed [31:0] data [0:7] ;  // the data to be sorted as eight 32-bit intergers
    reg signed [31:0] temp;         // temp for swapping the data
    reg              swapped;      // Flag if we have made any swaps

     // clock variables
    reg              clock;   // drive the simulation
    reg              signed [31:0] cycle_count;

    reg signed [31:0] end_index;
    reg signed [31:0] i;

    // This example task prints the data array to be sorted
    task print_data;
        reg signed [31:0] i;            // loop counter has local scope
        begin
            for (i = 0; i < 8; i = i + 1) begin
                $display("cycle: %4d data[%0d] = %4d", cycle_count, i, data[i]);
            end
        end
    endtask

    // Initialization section. This is run before the simulation starts.
    initial begin
        clock = 0;          // Key point: the clock drives the program
        cycle_count = 0;    // the cycle counter keep track of how far along execution is
        swapped = 0;        // flag for if the any swaps in the bubble sort have been done

        // This is the data array to be sorted
        data[0] = 0;  data[1] = 1010;  data[2] = 2020; data[3] = 3030;
        data[4] = 40;  data[5] = 50;  data[6] = 60; data[7] = 70;

        end_index = 7;
        i = 0;

    end

    // This is the main thread to swap the elements
    // It is driven by the clock
    // The clock changes from a 0 to 1 every 1 time unit
    // The code below is run on the postive rising edge of the clock

    always @(posedge clock) begin // at every positive rising edge (going from 0 to 1) of the clock
        $display("iteration %d",cycle_count);  // this is a print statement in verilog
        // recall the bubble sort loops through the array swapping elements
        // until there are no more swaps.
        swapped = 0;  // set the flag to zero
        print_data;   // call the task that prints the current state of the array

        // Overall Verilog tutorial can be found here:
        // For looping can be found here: http://www.asic-world.com/verilog/vbehave3.html
        // Your code goes here ...

        if (data[i] > data[i + 1]) begin

            temp = data[i];
            data[i] = data[i + 1];
            data[i + 1] = temp;
            swapped = 1;

        end

        i = i + 1;

        if (i == end_index) begin

            if (swapped == 0) begin
                $finish;
            end

            swapped = 0;
            end_index = end_index - 1;
            i = 0;

        end

    end

    // this "thread" (always with sensitivity list ) keeps track of how much time has passed
    // This code uses delayed assignment
    // The value of cycle_count is not updated until the next clock tick
    always @(posedge clock) begin
        cycle_count <= cycle_count + 1;
    end

    // keep the simulation from an infinite loop
    always @(posedge clock) begin
        if (cycle_count > `MAX_CYCLES) begin
            $display("Maximum cycles %d reached ", cycle_count);
            $finish;
        end
    end

    // The clock generator as an always block with no sensitivity list
    // After a delay of 1 time unit, set the clock to a 0 or 1 depending on the previous
    // value
    always begin
        #1 clock = !clock ;
    end

endmodule

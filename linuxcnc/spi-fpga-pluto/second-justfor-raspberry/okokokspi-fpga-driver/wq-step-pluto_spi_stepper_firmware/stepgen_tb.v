//    This is a component of pluto_step, a hardware step waveform generator
//    Copyright 2007 Jeff Epler <jepler@unpythonic.net>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

// install ubuntu package "verilog" from universe
// Compile with: iverilog -DTESTING test_stepgen.v stepgen.v
// run with "./a.out | less" and look at output for problems
`timescale  1ns/1ps
module stepgen_tb();
parameter W=12;
parameter F=10;
parameter T=5;

reg clk;
reg [F:0] vel;//[4:0] 
wire [W+F-1:0] pos;//[19:0]
wire step, dir;

stepgen s(clk, 1, pos, vel, 1, 0, step, dir, 2);//#(10,4,16) clk, 1, pos, vel, 1, 0, step, dir, 3
integer q;
reg ost;

initial begin
    ost = 0;
    vel = 10'h3f0;//5'h8; // two useful test cases:
                // vel=5'h8 (max step speed)
                // vel=5'h2 (~1 step per repeat)
    q = 0;
    repeat(50) begin//50
        repeat(50) begin
            #12 clk<=1;//20
            #12 clk<=0;//20

            if(step && !ost) begin
                if(dir) q = q+1;
                else q = q - 1;
            end
            ost <= step;

            //$display("%d %d %d %d %d %d %d %d %d",//"%d %d %x %x %d %d %d %d %d"
             //   step, dir, vel, pos, s.state, s.ones, s.pbit, s.timer, q);
        end
        vel = vel + 1; //vel = 10'h20 - vel;//
    end
end

endmodule

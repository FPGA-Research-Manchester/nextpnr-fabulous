module top (
	input clki, resetn,
	output trap,

	output        mem_valid,
	output        mem_instr,
	input         mem_ready,

	output [31:0] mem_addr,
	output [31:0] mem_wdata,
	output [ 3:0] mem_wstrb,
	input  [31:0] mem_rdata
);

    wire clk;
    BUFGCTRL clk_gb (
        .I0(clki),
        .CE0(1'b1),
        .CE1(1'b0),
        .S0(1'b1),
        .S1(1'b0),
        .IGNORE0(1'b0),
        .IGNORE1(1'b0),
        .O(clk)
    );

    picorv32 #(
        .ENABLE_COUNTERS(0),
        .TWO_STAGE_SHIFT(0),
        .CATCH_MISALIGN(0),
        .CATCH_ILLINSN(0)
    ) cpu (
        .clk      (clk     ),
        .resetn   (resetn   ),
        .trap     (trap     ),
        .mem_valid(mem_valid),
        .mem_instr(mem_instr),
        .mem_ready(mem_ready),
        .mem_addr (mem_addr ),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata)
    );
endmodule

module template ();

wire Tile_X9Y1_OPB_O0, Tile_X9Y1_OPB_O1;
(* keep *) InPass4_frame_config Tile_X9Y1_B (.O0(Tile_X9Y1_OPB_O0));

wire Tile_X9Y1_RES0_I0;
(* keep *) OutPass4_frame_config Tile_X9Y1_C (.I0(Tile_X9Y1_RES0_I0));

//wire Tile_X0Y1_A_O;
//(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y1_A (.O(Tile_X0Y1_A_O));

//reg and_reg, or_reg;

wire reset;
reg counter;
//wire clk;

    //SB_GB clk_gb (
     //   .USER_SIGNAL_TO_GLOBAL_BUFFER(Tile_X0Y1_A_O),
     //   .GLOBAL_BUFFER_OUTPUT(clk)
    //);
//assign clk = Tile_X0Y1_A_O;
assign reset = Tile_X9Y1_OPB_O0;

always @ ($global_clock) begin
    if (reset) counter <= 0;
    else counter <= counter + 1;
end

assign Tile_X9Y1_RES0_I0 = counter;

endmodule

    //and_reg <= Tile_X0Y1_A_O & Tile_X0Y2_A_O;
    //or_reg <= Tile_X0Y1_A_O | Tile_X0Y2_A_O;
    //Tile_X0Y1_A_I <= and_reg ^ or_reg;

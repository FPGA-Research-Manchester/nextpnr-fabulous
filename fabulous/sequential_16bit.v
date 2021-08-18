module sequential_16bit ();

//parameter VCC = 1'b1;
//parameter GND = 1'b0;

wire Tile_X0Y1_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y1_A (.I(Tile_X0Y1_A_I));//, .T(GND));

wire Tile_X0Y1_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y1_B (.I(Tile_X0Y1_B_I));//, .T(GND));

wire Tile_X0Y2_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y2_A (.I(Tile_X0Y2_A_I));//, .T(GND));

wire Tile_X0Y2_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y2_B (.I(Tile_X0Y2_B_I));//, .T(GND));

wire Tile_X0Y3_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y3_A (.I(Tile_X0Y3_A_I));//, .T(GND));

wire Tile_X0Y3_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y3_B (.I(Tile_X0Y3_B_I));//, .T(GND));

wire Tile_X0Y4_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y4_A (.I(Tile_X0Y4_A_I));//, .T(GND));

wire Tile_X0Y4_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y4_B (.I(Tile_X0Y4_B_I));//, .T(GND));

wire Tile_X0Y5_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y5_A (.I(Tile_X0Y5_A_I));//, .T(GND));

wire Tile_X0Y5_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y5_B (.I(Tile_X0Y5_B_I));//, .T(GND));

wire Tile_X0Y6_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y6_A (.I(Tile_X0Y6_A_I));//, .T(GND));

wire Tile_X0Y6_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y6_B (.I(Tile_X0Y6_B_I));//, .T(GND));

wire Tile_X0Y7_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y7_A (.I(Tile_X0Y7_A_I));//, .T(GND));

wire Tile_X0Y7_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y7_B (.I(Tile_X0Y7_B_I));//, .T(GND));

wire Tile_X0Y8_A_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y8_A (.I(Tile_X0Y8_A_I));//, .T(GND));

wire Tile_X0Y8_B_I;
(* keep *) IO_1_bidirectional_frame_config_pass Tile_X0Y8_B (.I(Tile_X0Y8_B_I));//, .T(GND));

reg [15:0] counter;

initial begin
    counter = 16'b0000000000000000;
end

always @ ($global_clock) begin
    counter <= counter + 1'b1;
end

assign {Tile_X0Y1_A_I,Tile_X0Y1_B_I,Tile_X0Y2_A_I,Tile_X0Y2_B_I,Tile_X0Y3_A_I,Tile_X0Y3_B_I,Tile_X0Y4_A_I,Tile_X0Y4_B_I,Tile_X0Y5_A_I,Tile_X0Y5_B_I,Tile_X0Y6_A_I,Tile_X0Y6_B_I,Tile_X0Y7_A_I,Tile_X0Y7_B_I,Tile_X0Y8_A_I,Tile_X0Y8_B_I} = counter;

endmodule

    //and_reg <= Tile_X0Y1_A_O & Tile_X0Y2_A_O;
    //or_reg <= Tile_X0Y1_A_O | Tile_X0Y2_A_O;
    //Tile_X0Y1_A_I <= and_reg ^ or_reg;

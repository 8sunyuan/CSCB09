// Part 2 skeleton


    (
        CLOCK_50,                        //    On Board 50 MHz
        // Your inputs and outputs here
        KEY,
        SW,
        // The ports below are for the VGA output.  Do not change.
        VGA_CLK,                           //    VGA Clock
        VGA_HS,                            //    VGA H_SYNC
        VGA_VS,                            //    VGA V_SYNC
        VGA_BLANK_N,                        //    VGA BLANK
        VGA_SYNC_N,                        //    VGA SYNC
        VGA_R,                           //    VGA Red[9:0]
        VGA_G,                             //    VGA Green[9:0]
        VGA_B,                           //    VGA Blue[9:0]
        HEX0,
        HEX1
    );
    input            CLOCK_50;                //    50 MHz
    input   [9:0]   SW;
    input   [3:0]   KEY;
    output   [6:0] HEX0;
    output   [6:0] HEX1;
    // Declare your inputs and outputs here
    // Do not change the following outputs
    output            VGA_CLK;                   //    VGA Clock
    output            VGA_HS;                    //    VGA H_SYNC
    output            VGA_VS;                    //    VGA V_SYNC
    output            VGA_BLANK_N;                //    VGA BLANK
    output            VGA_SYNC_N;                //    VGA SYNC
    output    [9:0]    VGA_R;                   //    VGA Red[9:0]
    output    [9:0]    VGA_G;                     //    VGA Green[9:0]
    output    [9:0]    VGA_B;                   //    VGA Blue[9:0]
   
    wire resetn;
    assign resetn = alwaysOne ;
   
    // Create the colour, x, y and writeEn wires that are inputs to the controller.
   reg [2:0] colour;// notice they were originally wire ,  I made them reg     edit:Mar20, 2:30am
    reg [6:0] x;
    reg [6:0] y;
    reg  writeEn;
   

    // Create an Instance of a VGA controller - there can be only one!
    // Define the number of colours as well as the initial background
    // image file (.MIF) for the controller.
    vga_adapter VGA(
            .resetn(resetn),
            .clock(CLOCK_50),
            .colour(colour),
            .x(x),
            .y(y),
            .plot(writeEn),
            /* Signals for the DAC to drive the monitor. */
            .VGA_R(VGA_R),
            .VGA_G(VGA_G),
            .VGA_B(VGA_B),
            .VGA_HS(VGA_HS),
            .VGA_VS(VGA_VS),
            .VGA_BLANK(VGA_BLANK_N),
            .VGA_SYNC(VGA_SYNC_N),
            .VGA_CLK(VGA_CLK));
    defparam VGA.RESOLUTION = "160x120";
    defparam VGA.MONOCHROME = "FALSE";
    defparam VGA.BITS_PER_COLOUR_CHANNEL = 1;
    defparam VGA.BACKGROUND_IMAGE = "black.mif";
   
    // Put your code here. Your code should produce signals x,y,colour and writeEn/plot
    // for the VGA controller, in addition to any other functionality your design may require.
    reg alwaysOne = 1'b1;
    reg alwaysZero = 1'b0;
	 reg speed1 = 2'b00;
	 reg speed2 = 2'b01;
	 reg speed3 = 2'b10;
   
    wire ld_x, ld_y;
    wire [3:0] stateNum;
    reg  [6:0] init_player_coord = 7'b0101111; // this is x coord
    wire [2:0] colour_player;
    wire [6:0] x_player;
    wire [6:0] y_player;
    wire writeEn_player;
    reg [25:0] counter_for_player = 26'b00000000000000000000000000;
    reg [6:0] init_y_p = 7'b1110000;
    reg [2:0] acolour_p = 3'b110;
    // Instansiate datapath                             
    datapath d0(.clk(CLOCK_50), .ld_x(ld_x), .ld_y(ld_y), .in(  init_player_coord), .reset_n(resetn), .x(x_player), .y(y_player), .colour(colour_player), .write(writeEn_player), .stateNum(stateNum), .init_y(init_y_p), .acolour(acolour_p));
   
    // Instansiate FSM control
    control c0(.clk(CLOCK_50), .move_r(~KEY[0]), .move_l(~KEY[3]), .move_d(~KEY[1]), .move_u(~KEY[2]), .reset_n(resetn), .ld_x(ld_x), .ld_y(ld_y), .stateNum(stateNum), .reset_game(reset_game), .dingding(counter_for_player), .how_fast(speed1));
   
     
    // --------------------------------------car movement starts here, for all cars----------------------------------------------------------
   
    wire ld_x_car0, ld_y_car0;
    wire [3:0] stateNum_car0;
    reg  [6:0] car0_coord = 7'b0101111;
    wire [2:0] colour_car0;
    wire [6:0] x_car0;
    wire [6:0] y_car0;
    wire writeEn_car0;
    reg [25:0] counter_for_car0 = 26'b00000000000000000000000001;
    reg [6:0] init_y_c0 = 7'b0101010;
    reg [2:0] acolour_c0 = 3'b100;
    // Instansiate datapath                                
    datapath car_0_d(.clk(CLOCK_50), .ld_x(ld_x_car0), .ld_y(ld_y_car0), .in(  car0_coord), .reset_n(resetn), .x(x_car0), .y(y_car0), .colour(colour_car0), .write(writeEn_car0), .stateNum(stateNum_car0),  .init_y(init_y_c0), .acolour(acolour_c0));
   
    // Instansiate FSM control
    control car_0_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(~KEY[2]), .move_d(~KEY[3]),  .move_u(~KEY[0]), .reset_n(resetn), .ld_x(ld_x_car0), .ld_y(ld_y_car0), .stateNum(stateNum_car0), .reset_game(alwaysZero), .dingding(counter_for_car0), .how_fast(speed2));
    //The outputs are: x_car0, y_car0, colour_car0, writeEn_car0
    //car0 movement ends here----------------------------------------------------------------------------------------------------
     
     
    wire ld_x_car1, ld_y_car1;
    wire [3:0] stateNum_car1;
    reg  [6:0] car1_coord = 7'b0101100; // this is init x coord!!!
    wire [2:0] colour_car1;
    wire [6:0] x_car1;
    wire [6:0] y_car1;
    wire writeEn_car1;
    reg [25:0] counter_for_car1 = 26'b00000000000000000000000010;
    reg [6:0] init_y_c1 = 7'b0100111;
    reg [2:0] acolour_c1 = 3'b101;
    // Instansiate datapath                                
    datapath car_1_d(.clk(CLOCK_50), .ld_x(ld_x_car1), .ld_y(ld_y_car1), .in(  car1_coord), .reset_n(resetn), .x(x_car1), .y(y_car1), .colour(colour_car1), .write(writeEn_car1), .stateNum(stateNum_car1),  .init_y(init_y_c1), .acolour(acolour_c1));
   
    // Instansiate FSM control
    control car_1_c(.clk(CLOCK_50), .move_r(alwaysZero), .move_l(alwaysOne), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car1), .ld_y(ld_y_car1), .stateNum(stateNum_car1), .reset_game(alwaysZero), .dingding(counter_for_car1), .how_fast(speed3));
    //car1 movement ends here----------------------------------------------------------------------------------------------------
     
     
    wire ld_x_car2, ld_y_car2;
    wire [3:0] stateNum_car2;
    reg  [6:0] car2_coord = 7'b0110000; // this is init x coord!!!
    wire [2:0] colour_car2;
    wire [6:0] x_car2;
    wire [6:0] y_car2;
    wire writeEn_car2;
    reg [25:0] counter_for_car2 = 26'b00000000000000000000000011;
    reg [6:0] init_y_c2 = 7'b0100100;
    reg [2:0] acolour_c2 = 3'b110;
    // Instansiate datapath                                
    datapath car_2_d(.clk(CLOCK_50), .ld_x(ld_x_car2), .ld_y(ld_y_car2), .in(car2_coord), .reset_n(resetn), .x(x_car2), .y(y_car2), .colour(colour_car2), .write(writeEn_car2), .stateNum(stateNum_car2),  .init_y(init_y_c2), .acolour(acolour_c2));
   
    // Instansiate FSM control
    control car_2_c(.clk(CLOCK_50), .move_r(alwaysZero), .move_l(alwaysOne), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car2), .ld_y(ld_y_car2), .stateNum(stateNum_car2), .reset_game(alwaysZero), .dingding(counter_for_car2), .how_fast(speed1));
    //car2 movement ends here----------------------------------------------------------------------------------------------------
     
     
    wire ld_x_car3, ld_y_car3;
    wire [3:0] stateNum_car3;
    reg  [6:0] car3_coord = 7'b0100100; // this is init x coord!!!
    wire [2:0] colour_car3;
    wire [6:0] x_car3;
    wire [6:0] y_car3;
    wire writeEn_car3;
    reg [25:0] counter_for_car3 = 26'b00000000000000000000000100;
    reg [6:0] init_y_c3 = 7'b0100001;
    reg [2:0] acolour_c3 = 3'b111;
    // Instansiate datapath                                
    datapath car_3_d(.clk(CLOCK_50), .ld_x(ld_x_car3), .ld_y(ld_y_car3), .in(car3_coord), .reset_n(resetn), .x(x_car3), .y(y_car3), .colour(colour_car3), .write(writeEn_car3), .stateNum(stateNum_car3),  .init_y(init_y_c3), .acolour(acolour_c3));
   
    // Instansiate FSM control
    control car_3_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car3), .ld_y(ld_y_car3), .stateNum(stateNum_car3), .reset_game(alwaysZero), .dingding(counter_for_car3), .how_fast(speed2));
    //car3 movement ends here----------------------------------------------------------------------------------------------------
     
     
    wire ld_x_car4, ld_y_car4;
    wire [3:0] stateNum_car4;
    reg  [6:0] car4_coord = 7'b1010011; // this is init x coord!!!
    wire [2:0] colour_car4;
    wire [6:0] x_car4;
    wire [6:0] y_car4;
    wire writeEn_car4;
    reg [25:0] counter_for_car4 = 26'b00000000000000000000000101;
    reg [6:0] init_y_c4 = 7'b0011110;
    reg [2:0] acolour_c4 = 3'b001;
    // Instansiate datapath                                
    datapath car_4_d(.clk(CLOCK_50), .ld_x(ld_x_car4), .ld_y(ld_y_car4), .in(car4_coord), .reset_n(resetn), .x(x_car4), .y(y_car4), .colour(colour_car4), .write(writeEn_car4), .stateNum(stateNum_car4),  .init_y(init_y_c4), .acolour(acolour_c4));
   
    // Instansiate FSM control
    control car_4_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car4), .ld_y(ld_y_car4), .stateNum(stateNum_car4), .reset_game(alwaysZero), .dingding(counter_for_car4), .how_fast(speed3));
    //car4 movement ends here----------------------------------------------------------------------------------------------------
     
     
    wire ld_x_car5, ld_y_car5;
    wire [3:0] stateNum_car5;
    reg  [6:0] car5_coord = 7'b1010001; // this is init x coord!!!
    wire [2:0] colour_car5;
    wire [6:0] x_car5;
    wire [6:0] y_car5;
    wire writeEn_car5;
    reg [25:0] counter_for_car5 = 26'b00000000000000000000000110;
    reg [6:0] init_y_c5 = 7'b0011100;
    reg [2:0] acolour_c5 = 3'b110;
    // Instansiate datapath                                
    datapath car_5_d(.clk(CLOCK_50), .ld_x(ld_x_car5), .ld_y(ld_y_car5), .in(car5_coord), .reset_n(resetn), .x(x_car5), .y(y_car5), .colour(colour_car5), .write(writeEn_car5), .stateNum(stateNum_car5),  .init_y(init_y_c5), .acolour(acolour_c5));
   
    // Instansiate FSM control
    control car_5_c(.clk(CLOCK_50), .move_r(alwaysZero), .move_l(alwaysOne), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car5), .ld_y(ld_y_car5), .stateNum(stateNum_car5), .reset_game(alwaysZero), .dingding(counter_for_car5), .how_fast(speed1));
    //car5 movement ends here----------------------------------------------------------------------------------------------------
     
     
    wire ld_x_car6, ld_y_car6;
    wire [3:0] stateNum_car6;
    reg  [6:0] car6_coord = 7'b0111000; // this is init x coord!!!
    wire [2:0] colour_car6;
    wire [6:0] x_car6;
    wire [6:0] y_car6;
    wire writeEn_car6;
    reg [25:0] counter_for_car6 = 26'b00000000000000000000000111;
    reg [6:0] init_y_c6 = 7'b0011010;
    reg [2:0] acolour_c6 = 3'b001;
    // Instansiate datapath                                
    datapath car_6_d(.clk(CLOCK_50), .ld_x(ld_x_car6), .ld_y(ld_y_car6), .in(car6_coord), .reset_n(resetn), .x(x_car6), .y(y_car6), .colour(colour_car6), .write(writeEn_car6), .stateNum(stateNum_car6),  .init_y(init_y_c6), .acolour(acolour_c6));
   
    // Instansiate FSM control
    control car_6_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car6), .ld_y(ld_y_car6), .stateNum(stateNum_car6), .reset_game(alwaysZero), .dingding(counter_for_car6), .how_fast(speed2));
    //car6 movement ends here----------------------------------------------------------------------------------------------------
     
   
    wire ld_x_car7, ld_y_car7;
    wire [3:0] stateNum_car7;
    reg  [6:0] car7_coord = 7'b0100000; // this is init x coord!!!
    wire [2:0] colour_car7;
    wire [6:0] x_car7;
    wire [6:0] y_car7;
    wire writeEn_car7;
    reg [25:0] counter_for_car7 = 26'b00000000000000000000001000;
    reg [6:0] init_y_c7 = 7'b1011001;
    reg [2:0] acolour_c7 = 3'b101;
    // Instansiate datapath                                
    datapath car_7_d(.clk(CLOCK_50), .ld_x(ld_x_car7), .ld_y(ld_y_car7), .in(car7_coord), .reset_n(resetn), .x(x_car7), .y(y_car7), .colour(colour_car7), .write(writeEn_car7), .stateNum(stateNum_car7),  .init_y(init_y_c7), .acolour(acolour_c7));
   
    // Instansiate FSM control
    control car_7_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car7), .ld_y(ld_y_car7), .stateNum(stateNum_car7), .reset_game(alwaysZero), .dingding(counter_for_car7), .how_fast(speed3));
    //car7 movement ends here----------------------------------------------------------------------------------------------------
     
   
    wire ld_x_car8, ld_y_car8;
    wire [3:0] stateNum_car8;
    reg  [6:0] car8_coord = 7'b0110001; // this is init x coord!!!
    wire [2:0] colour_car8;
    wire [6:0] x_car8;
    wire [6:0] y_car8;
    wire writeEn_car8;
    reg [25:0] counter_for_car8 = 26'b00000000000000000000001001;
    reg [6:0] init_y_c8 = 7'b0010000;
    reg [2:0] acolour_c8 = 3'b110;
    // Instansiate datapath                                
    datapath car_8_d(.clk(CLOCK_50), .ld_x(ld_x_car8), .ld_y(ld_y_car8), .in(car8_coord), .reset_n(resetn), .x(x_car8), .y(y_car8), .colour(colour_car8), .write(writeEn_car8), .stateNum(stateNum_car8),  .init_y(init_y_c8), .acolour(acolour_c8));
   
    // Instansiate FSM control
    control car_8_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car8), .ld_y(ld_y_car8), .stateNum(stateNum_car8), .reset_game(alwaysZero), .dingding(counter_for_car8), .how_fast(speed1));
    //car8 movement ends here----------------------------------------------------------------------------------------------------
     
       
    wire ld_x_car9, ld_y_car9;
    wire [3:0] stateNum_car9;
    reg  [6:0] car9_coord = 7'b1000000; // this is init x coord!!!
    wire [2:0] colour_car9;
    wire [6:0] x_car9;
    wire [6:0] y_car9;
    wire writeEn_car9;
    reg [25:0] counter_for_car9 = 26'b00000000000000000000001010;
    reg [6:0] init_y_c9 = 7'b0010111;
    reg [2:0] acolour_c9 = 3'b010;
    // Instansiate datapath                                
    datapath car_9_d(.clk(CLOCK_50), .ld_x(ld_x_car9), .ld_y(ld_y_car9), .in(car9_coord), .reset_n(resetn), .x(x_car9), .y(y_car9), .colour(colour_car9), .write(writeEn_car9), .stateNum(stateNum_car9),  .init_y(init_y_c9), .acolour(acolour_c9));
   
    // Instansiate FSM control
    control car_9_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car9), .ld_y(ld_y_car9), .stateNum(stateNum_car9), .reset_game(alwaysZero), .dingding(counter_for_car9), .how_fast(speed2));
    //car9 movement ends here----------------------------------------------------------------------------------------------------
     
   
    wire ld_x_car10, ld_y_car10;
    wire [3:0] stateNum_car10;
    reg  [6:0] car10_coord = 7'b0000100; // this is init x coord!!!
    wire [2:0] colour_car10;
    wire [6:0] x_car10;
    wire [6:0] y_car10;
    wire writeEn_car10;
    reg [25:0] counter_for_car10 = 26'b00000000000000000000001011;
    reg [6:0] init_y_c10 = 7'b1010110;
    reg [2:0] acolour_c10 = 3'b011;
    // Instansiate datapath                                
    datapath car_10_d(.clk(CLOCK_50), .ld_x(ld_x_car10), .ld_y(ld_y_car10), .in(car10_coord), .reset_n(resetn), .x(x_car10), .y(y_car10), .colour(colour_car10), .write(writeEn_car10), .stateNum(stateNum_car10),  .init_y(init_y_c10), .acolour(acolour_c10));
   
    // Instansiate FSM control
    control car_10_c(.clk(CLOCK_50), .move_r(alwaysZero), .move_l(alwaysOne), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car10), .ld_y(ld_y_car10), .stateNum(stateNum_car10), .reset_game(alwaysZero), .dingding(counter_for_car10), .how_fast(speed3));
    //car10 movement ends here----------------------------------------------------------------------------------------------------
     
   
    wire ld_x_car11, ld_y_car11;
    wire [3:0] stateNum_car11;
    reg  [6:0] car11_coord = 7'b0110111; // this is init x coord!!!
    wire [2:0] colour_car11;
    wire [6:0] x_car11;
    wire [6:0] y_car11;
    wire writeEn_car11;
    reg [25:0] counter_for_car11 = 26'b00000000000000000000001100;
    reg [6:0] init_y_c11 = 7'b0010101;
    reg [2:0] acolour_c11 = 3'b101;
    // Instansiate datapath                                
    datapath car_11_d(.clk(CLOCK_50), .ld_x(ld_x_car11), .ld_y(ld_y_car11), .in(car11_coord), .reset_n(resetn), .x(x_car11), .y(y_car11), .colour(colour_car11), .write(writeEn_car11), .stateNum(stateNum_car11),  .init_y(init_y_c11), .acolour(acolour_c11));
   
    // Instansiate FSM control
    control car_11_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car11), .ld_y(ld_y_car11), .stateNum(stateNum_car11), .reset_game(alwaysZero), .dingding(counter_for_car11), .how_fast(speed1));
    //car11 movement ends here----------------------------------------------------------------------------------------------------
     
   
    wire ld_x_car12, ld_y_car12;
    wire [3:0] stateNum_car12;
    reg  [6:0] car12_coord = 7'b0110100; // this is init x coord!!!
    wire [2:0] colour_car12;
    wire [6:0] x_car12;
    wire [6:0] y_car12;
    wire writeEn_car12;
    reg [25:0] counter_for_car12 = 26'b00000000000000000000001101;
    reg [6:0] init_y_c12 = 7'b1010100;
    reg [2:0] acolour_c12 = 3'b010;
    // Instansiate datapath                                
    datapath car_12_d(.clk(CLOCK_50), .ld_x(ld_x_car12), .ld_y(ld_y_car12), .in(car12_coord), .reset_n(resetn), .x(x_car12), .y(y_car12), .colour(colour_car12), .write(writeEn_car12), .stateNum(stateNum_car12),  .init_y(init_y_c12), .acolour(acolour_c12));
   
    // Instansiate FSM control
    control car_12_c(.clk(CLOCK_50), .move_r(alwaysOne), .move_l(alwaysZero), .move_d(alwaysZero),  .move_u(alwaysZero), .reset_n(resetn), .ld_x(ld_x_car12), .ld_y(ld_y_car12), .stateNum(stateNum_car12), .reset_game(alwaysZero), .dingding(counter_for_car12), .how_fast(speed2));
    //car12 movement ends here----------------------------------------------------------------------------------------------------
     
	  
    // --------------------------------------car movement ends here, for all cars----------------------------------------------------------


    wire [9:0] score;
    wire reset_game;
    scoreCounter player_and_car0(x_player, y_player, x_car0, y_car0, x_car1, y_car1, x_car2, y_car2, x_car3, y_car3, x_car4, y_car4, x_car5, y_car5, x_car6, y_car6, x_car7, y_car7, x_car8, y_car8, x_car9, y_car9, x_car10, y_car10, x_car11, y_car11, x_car12, y_car12, score, reset_game);
    hex_display first_digit(score, HEX1[6:0], HEX0[6:0]); //notice score is displayed in hexadecimal
     
    //The following for processing player movement and car movement (make sure that only one of play or the car can move on the same clock cycle)
    // If player is moving ,then link the vga to the player, if the car is moving, than link the vga to the car
    // If both of them are moving in the same clock cycle (very unlikely), then move the player
    // edited: mar 20, 5pmreset_n
    // Notice: writeEn_player is write Enable for player; writeEn_car0 is write enable for car_0
    // The following is for choosing to print the player movement or to print the car movement
   
    always @(posedge CLOCK_50)
    begin
        if(writeEn_player) 
            begin
                writeEn <= writeEn_player;   //  Do I use   <=   or   =   ????? writeEn, x, y and colour are originally type wire, but I need to make them type reg???  ????????????????????????????????
                x <= x_player;       
                y <= y_player;
                colour = colour_player; // Notice: I made the following variable type reg: writeEn, x, y, colour
            end
        else if (writeEn_car0)    // if player isnt moving, then let the car move
            begin
                writeEn <= writeEn_car0;    
                x <= x_car0;                       
                y <= y_car0;
                colour <= colour_car0;
            end   
        // Added on Mar27 -------------
        else if (writeEn_car1) 
            begin
                writeEn <= writeEn_car1;    
                x <= x_car1;                       
                y <= y_car1;
                colour <= colour_car1;
            end   
        else if (writeEn_car2)   
            begin
                writeEn <= writeEn_car2;    
                x <= x_car2;                       
                y <= y_car2;
                colour <= colour_car2;
            end   
        else if (writeEn_car3)   
            begin
                writeEn <= writeEn_car3;    
                x <= x_car3;                       
                y <= y_car3;
                colour <= colour_car3;
            end   
        else if (writeEn_car4)  
            begin
                writeEn <= writeEn_car4;    
                x <= x_car4;                       
                y <= y_car4;
                colour <= colour_car4;
            end   
        else if (writeEn_car5)   
            begin
                writeEn <= writeEn_car5;    
                x <= x_car5;                       
                y <= y_car5;
                colour <= colour_car5;
            end   
        else if (writeEn_car6)   
            begin
                writeEn <= writeEn_car6;    
                x <= x_car6;                       
                y <= y_car6;
                colour <= colour_car6;
            end   
        else if (writeEn_car7)  
            begin
                writeEn <= writeEn_car7;    
                x <= x_car7;                       
                y <= y_car7;
                colour <= colour_car7;
            end   
        else if (writeEn_car8)   
            begin
                writeEn <= writeEn_car8;    
                x <= x_car8;                       
                y <= y_car8;
                colour <= colour_car8;
            end   
        else if (writeEn_car9)   
            begin
                writeEn <= writeEn_car9;    
                x <= x_car9;                       
                y <= y_car9;
                colour <= colour_car9;
            end   
        else if (writeEn_car10)   
            begin
                writeEn <= writeEn_car10;    
                x <= x_car10;                       
                y <= y_car10;
                colour <= colour_car10;
            end   
        else if (writeEn_car11)  
            begin
                writeEn <= writeEn_car11;    
                x <= x_car11;                       
                y <= y_car11;
                colour <= colour_car11;
            end   
        else if (writeEn_car12)  
            begin
                writeEn <= writeEn_car12;    
                x <= x_car12;                       
                y <= y_car12;
                colour <= colour_car12;
            end           
        //Mar27 adding end---------------
    end
       
endmodule


//You need to extend the parameter of this module such that all cood. of all cars are inputed into this module
//outputs are score and reset,
//if reset is 1, this means the play crashed into a car and game should be reset
module scoreCounter(x_player, y_player,
x_car0, y_car0, x_car1, y_car1, x_car2, y_car2, x_car3, y_car3, x_car4, y_car4, x_car5, y_car5, x_car6, y_car6,
x_car7, y_car7, x_car8, y_car8, x_car9, y_car9, x_car10, y_car10, x_car11, y_car11, x_car12, y_car12,
score, reset_game);
    input [6:0] x_player;
    input [6:0] y_player;
    input [6:0] x_car0;
    input [6:0] y_car0;
    input [6:0] x_car1;
    input [6:0] y_car1;
    input [6:0] x_car2;
    input [6:0] y_car2;
    input [6:0] x_car3;
    input [6:0] y_car3;
    input [6:0] x_car4;
    input [6:0] y_car4;
    input [6:0] x_car5;
    input [6:0] y_car5;
    input [6:0] x_car6;
    input [6:0] y_car6;
    input [6:0] x_car7;
    input [6:0] y_car7;
    input [6:0] x_car8;
    input [6:0] y_car8;
    input [6:0] x_car9;
    input [6:0] y_car9;
    input [6:0] x_car10;
    input [6:0] y_car10;
    input [6:0] x_car11;
    input [6:0] y_car11;
    input [6:0] x_car12;
    input [6:0] y_car12;
    output reg [9:0] score;
    output reg reset_game;
    
	
    always @(*) //Notice it is not positive edge
    begin
        if (y_player == 7'b0000011) // When player reaches the top of the screen
            begin // it is going into this state
                score = score + 1'b1;
                reset_game = 1'b1;
            end
		  else if (y_player == 7'b1110001)
				begin
			   reset_game = 1'b1;
				score = score;
				end
        else if ((x_player == x_car0 && y_player == y_car0) || (x_player == x_car1 && y_player == y_car1) ||
          (x_player == x_car2 && y_player == y_car2) || (x_player == x_car3 && y_player == y_car3) ||
          (x_player == x_car4 && y_player == y_car4) || (x_player == x_car5 && y_player == y_car5) ||
          (x_player == x_car6 && y_player == y_car6) || (x_player == x_car7 && y_player == y_car7) ||
          (x_player == x_car8 && y_player == y_car8) || (x_player == x_car9 && y_player == y_car9) ||
          (x_player == x_car10 && y_player == y_car10) || (x_player == x_car11 && y_player == y_car11) ||
          (x_player == x_car12 && y_player == y_car12) )  // player collide with car (any of the 13 cars)
            begin
                     reset_game = 1'b1;
                     score = 1'b0;
            end
			
        else
            reset_game = 1'b0;
    end

endmodule


module control(clk, move_r, move_l, move_d, move_u, reset_n, ld_x, ld_y, stateNum, reset_game, dingding, how_fast);
    input [25:0] dingding; // dingding is the counter! It counts like this: Ding!!! Ding!!! Ding!!! Ding!!! Ding!!!
    input reset_game;
    input clk, move_r, move_l, move_d, move_u, reset_n;
	 input [1:0] how_fast;
    output reg ld_y, ld_x;
    reg [3:0] curr, next;
    output reg [3:0] stateNum;
    localparam    S_CLEAR    = 4'b0000;
    localparam S_LOAD_X    = 4'b0001;
    localparam S_WAIT_Y    = 4'b0010;
    localparam S_LOAD_Y    = 4'b0011;
   
    localparam    wait_input    = 4'b0100;
    localparam    clear_all    = 4'b0101;
    localparam    print_right    = 4'b0110;
    localparam    print_left    = 4'b0111;
    localparam    print_down    = 4'b1000;
    localparam    print_up    = 4'b1001;
    localparam  temp_selecting_state = 4'b1010;
    localparam after_drawing = 4'b1011;
    localparam cleanUp = 4'b1100;
    wire [26:0] press_now;   
    wire [26:0] press_now_for_car;   
    wire result_press_now;
	 reg [25:0] speed;
    //wire result_for_car;
    
	 always @(*)
	 begin
		if (how_fast == 2'b00)
		   speed <= 26'b0101111101011110000100;

		else if (how_fast == 2'b01)
		   speed <= 26'b010111110101111000010;
		else
		   speed <= 26'b01011111010111100001;
	 end
	 RateDividerForCar player_counter1(clk, press_now, reset_n, 
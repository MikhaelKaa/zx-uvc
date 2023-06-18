module rgbi2dcmi // EPM7128
(
	// ZX Spectrum signals
	input ZX_R, ZX_G, ZX_B, ZX_I, ZX_PIX_CLK, ZX_VS, ZX_HS,
	
	// DCMI signals
	output [7:0]DCMI_DATA, 
	output DCMI_PIXCLK, DCMI_VSYNC, DCMI_HSYNC,
	
	// SPI 
	input SPI_MOSI, SPI_NSS, SPI_CLK,
	output SPI_MISO,
	
	// global reset
	input reset,
	
	// LDM-PP 2.7128 board
	output led1, led2, led3, led4,
	input button1, button2, button3, button4
	
);


// SPI
reg [7:0] spi_rx = 8'd0;
wire spi_edge = SPI_CLK & (~SPI_NSS);
// Control register
reg [7:0] control;

always @(posedge spi_edge) begin	
	if(!reset) begin
	
	end
	else begin
		spi_rx    <= spi_rx << 1;
		spi_rx[0] <= SPI_MOSI;
	end
end
assign SPI_MISO = SPI_MOSI;

always @(posedge SPI_NSS) begin	
	control <= spi_rx;
	//if(func == 0) spi_tx  <= spi_rx;
end

wire enable = control[0];
wire [1:0] func = {control[2], control[1]};
wire sync_meter = control[3];

wire [3:0] rgbi_direct = {ZX_I, ZX_B, ZX_G, ZX_R};
wire [3:0] sync_direct = {1'b0, ZX_HS, ZX_VS, ZX_PIX_CLK};


wire out_pix_clk = sync_meter ? 1'b0 : ZX_PIX_CLK;
wire out_vs      = sync_meter ? 1'b0 : ZX_VS;
wire out_hs      = sync_meter ? 1'b0 : ZX_HS;


assign {DCMI_DATA[3], DCMI_DATA[2], DCMI_DATA[1], DCMI_DATA[0]} = rgbi_direct;
assign {DCMI_DATA[7], DCMI_DATA[6], DCMI_DATA[5], DCMI_DATA[4]} = sync_direct;


assign DCMI_PIXCLK = enable ? out_pix_clk : 1'b0;
assign DCMI_VSYNC  = enable ? out_vs      : 1'b0;
assign DCMI_HSYNC  = enable ? out_hs      : 1'b0;


assign led1 = enable;
assign led2 = func[0];
assign led3 = func[1];
assign led4 = reset;

endmodule


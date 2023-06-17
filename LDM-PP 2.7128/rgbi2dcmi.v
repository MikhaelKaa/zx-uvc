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
reg [15:0] spi_rx = 16'd0;
reg [15:0] spi_tx = 16'd0;
reg [3:0]  tx_cnt = 4'd0;
wire spi_edge = SPI_CLK & (~SPI_NSS);
// Control register
reg [15:0] control;

always @(posedge spi_edge) begin	
	if(!reset) begin
		tx_cnt <= 0;
	end
	else begin
		spi_rx    <= spi_rx << 1;
		spi_rx[0] <= SPI_MOSI;
		tx_cnt    <= tx_cnt - 4'd1;
	end
end
assign SPI_MISO = SPI_NSS ? 1'b0 : spi_tx[tx_cnt-1];

always @(posedge SPI_NSS) begin	
	control <= spi_rx;
	if(func == 0) spi_tx  <= spi_rx;
	if(func == 1) spi_tx  <= vs_cnt;
end

wire enable = control[0];
wire [1:0] func = {control[2], control[1]};



reg [15:0] vs_cnt = 16'd0;
always @(posedge ZX_PIX_CLK) begin
	if(ZX_VS) vs_cnt = vs_cnt + 1;
end


assign DCMI_DATA[0] = ZX_R;
assign DCMI_DATA[1] = ZX_G;
assign DCMI_DATA[2] = ZX_B;
assign DCMI_DATA[3] = ZX_I;

assign DCMI_DATA[4] = 1'b0;
assign DCMI_DATA[5] = 1'b0;
assign DCMI_DATA[6] = 1'b0;
assign DCMI_DATA[7] = 1'b0;

assign DCMI_PIXCLK = enable ? ZX_PIX_CLK : 1'b0;
assign DCMI_VSYNC  = enable ? ZX_VS      : 1'b0;
assign DCMI_HSYNC  = enable ? ZX_HS      : 1'b0;


assign led1 = enable;
assign led2 = func[0];
assign led3 = func[1];
assign led4 = reset;

endmodule


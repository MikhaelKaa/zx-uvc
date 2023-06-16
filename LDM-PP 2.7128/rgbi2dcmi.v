	module rgbi2dcmi // EPM7128
(
	// ZX Spectrum signals
	input ZX_R, ZX_G, ZX_B, ZX_I, ZX_PIX_CLK, ZX_VS, ZX_HS,
	
	// DCMI signals
	output [7:0]DCMI_DATA, 
	output DCMI_PIXCLK, DCMI_VSYNC, DCMI_HSYNC,
	
	// LDM-PP 2.7128 board
	output led1, led2, led3, led4,
	input button1, button2, button3, button4
	
);

assign DCMI_DATA[0] = ZX_R;
assign DCMI_DATA[1] = ZX_G;
assign DCMI_DATA[2] = ZX_B;
assign DCMI_DATA[3] = ZX_I;

assign DCMI_DATA[4] = 1'b0;
assign DCMI_DATA[5] = 1'b0;
assign DCMI_DATA[6] = 1'b0;
assign DCMI_DATA[7] = 1'b0;

assign DCMI_PIXCLK = ZX_PIX_CLK;
assign DCMI_VSYNC = ZX_VS;
assign DCMI_HSYNC = ZX_HS;

reg [23:0] cnt;
always @(negedge ZX_PIX_CLK) begin
	cnt = cnt + 1'b1;
end

assign led1 = button1?cnt[23]:1'b1;
assign led2 = button2?cnt[22]:1'b1;
assign led3 = button3?cnt[21]:1'b1;
assign led4 = button4?cnt[20]:1'b1;

endmodule


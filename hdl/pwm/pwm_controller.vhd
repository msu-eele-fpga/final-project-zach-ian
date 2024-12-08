library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity pwm_controller is
	generic(
		CLK_PERIOD 	: time := 20 ns
	);
	port(
		clk		: in std_logic;
		rst		: in std_logic;
		period		: in unsigned(19 downto 0); -- U20.14
		duty_cycle	: in std_logic_vector(11 downto 0); -- U12.11
		output		: out std_logic := '0'
	);
end entity;

architecture pwm_controller_arch of pwm_controller is
	
	constant sysclockrate 	: unsigned(25 downto 0)	:= "10111110101111000010000000";
	signal percount		: unsigned(45 downto 0) := "0000000000000000000000000000000000000000000000";
	signal clkcount1	: unsigned(57 downto 0) := "0000000000000000000000000000000000000000000000000000000000";
	signal clkcount2	: unsigned(57 downto 0) := "0000000000000000000000000000000000000000000000000000000000";
	signal count		: unsigned(57 downto 0) := "0000000000000000000000000000000000000000000000000000000000";
	signal output_midman	: std_logic := '0';
	signal on_tog		: boolean := false;
	
	begin
		percount <= shift_right(sysclockrate * period,24);
		clkcount1 <= shift_right(percount * unsigned(duty_cycle),11);
		clkcount2 <= percount - clkcount1;
		output <= output_midman;
		
		
		process(clk,rst) is
			begin
				if(rst = '1') then
					output_midman 	<= '0';
					count 		<= "0000000000000000000000000000000000000000000000000000000000";
					on_tog 		<= false;
					
				elsif(rising_edge(clk)) then
					if(on_tog) then
						if(count >= clkcount1) then
							count <= "0000000000000000000000000000000000000000000000000000000000";
							output_midman <= '0';
							on_tog <= false;
						else count <= count + "1";
						end if;
					else 
						if(count >= clkcount2) then
							count <= "0000000000000000000000000000000000000000000000000000000000";
							output_midman <= '1';
							on_tog <= true;
						else count <= count + "1";
						end if;
					end if;
				end if;
		end process;
end architecture;

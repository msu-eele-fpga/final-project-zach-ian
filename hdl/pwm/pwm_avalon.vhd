library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RGB_LED_avalon is
	port(
		clk : in std_ulogic;
		rst : in std_ulogic;
		
		-- avalon memory-mapped slave interface
		avs_read	: in std_logic;
		avs_write	: in std_logic;
		avs_address	: in std_logic_vector(1 downto 0);
		avs_readdata	: out std_logic_vector(31 downto 0);
		avs_writedata	: in std_logic_vector(31 downto 0);
		
		-- external I/O; export to top-level
		red_pwm_signal 		: out std_ulogic;
		green_pwm_signal 	: out std_ulogic;
		blue_pwm_signal 	: out std_ulogic
	);
end entity;

architecture RGB_LED_avalon_arch of RGB_LED_avalon is
	
	signal duty_cycle_r 	: std_logic_vector(11 downto 0) := "010000000000";
	signal duty_cycle_g 	: std_logic_vector(11 downto 0) := "010000000000";
	signal duty_cycle_b 	: std_logic_vector(11 downto 0) := "010000000000";
	signal pwm_period	: unsigned(19 downto 0) := "00000000000010000000";
	
	
	component pwm_controller is
		port(
			clk		: in std_logic;
			rst		: in std_logic;
			period		: in unsigned(19 downto 0); -- U20.14
			duty_cycle	: in std_logic_vector(11 downto 0); -- U12.11
			output		: out std_logic := '0'
		);
	end component;
	
	begin
	
	avalon_register_read : process(clk) is
		begin
			if(rising_edge(clk) and avs_read = '1') then 
				case avs_address is
					when "00"	=> avs_readdata <= "00000000000000000000" & duty_cycle_r;
					when "01"	=> avs_readdata <= "00000000000000000000" & duty_cycle_g;
					when "10"	=> avs_readdata <= "00000000000000000000" & duty_cycle_b;
					when "11"	=> avs_readdata <= "000000000000" & std_logic_vector(pwm_period);
					when others	=> avs_readdata <= (others => '0'); -- return zeros for unused registers
				end case;
			end if;
	end process;
	
	avalon_register_write : process(clk, rst) is
		begin
			if(rst = '1') then
				duty_cycle_r	<= "010000000000";
				duty_cycle_g	<= "010000000000";
				duty_cycle_b	<= "010000000000";
				pwm_period 	<= "00000000000010000000";
			elsif (rising_edge(clk) and avs_write = '1') then
				case avs_address is
					when "00"	=> duty_cycle_r <= avs_writedata(11 downto 0);
					when "01"	=> duty_cycle_g <= avs_writedata(11 downto 0);
					when "10"	=> duty_cycle_b <= avs_writedata(11 downto 0);
					when "11"	=> pwm_period 	<= unsigned(avs_writedata(19 downto 0));
					when others	=> null;
				end case;
			end if;
	end process;
	
	PWMGUYr : pwm_controller port map(
		clk => clk,
		rst => rst,
		period => pwm_period,
		duty_cycle => duty_cycle_r,
		output => red_pwm_signal
	);
	
	PWMGUYg : pwm_controller port map(
		clk => clk,
		rst => rst,
		period => pwm_period,
		duty_cycle => duty_cycle_g,
		output => green_pwm_signal
	);
	
	PWMGUYb : pwm_controller port map(
		clk => clk,
		rst => rst,
		period => pwm_period,
		duty_cycle => duty_cycle_b,
		output => blue_pwm_signal
	);
end architecture;
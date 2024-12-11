library ieee;
use ieee.std_logic_1164.all;

entity one_pulse is
	port (
		clk		: in std_ulogic;
		rst		: in std_ulogic;
		input	: in std_ulogic;
		pulse	: out std_ulogic);
end entity;

architecture one_pulse_arch of one_pulse is
	
	signal prev_input : std_ulogic;
	
	begin
	
	PULSEGEN : process (clk) is
		begin
			if (rst = '1') then
				pulse <= '0';
			elsif (rising_edge(clk)) then
				if (input = not prev_input and input = '1') then
					pulse <= '1';
				else 
					pulse <= '0';
				end if;
				prev_input <= input;
			end if;
	end process;	
end architecture;

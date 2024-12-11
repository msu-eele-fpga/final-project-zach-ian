library ieee;
use ieee.std_logic_1164.all;

entity debouncer is
	generic (
		clk_period		: time := 20 ns;
		debounce_time	: time
		);
	port (
		clk			: in std_ulogic;
		rst			: in std_ulogic;
		input		: in std_ulogic;
		debounced	: out std_ulogic
		);
end entity;

architecture debouncer_arch of debouncer is
	
	signal tracker : integer;
	signal delay_amount : integer := debounce_time / clk_period;
	signal toggled : std_ulogic;
	signal debounced_sig : std_ulogic;
	
	begin
	
	debounced <= debounced_sig;
	
	OOF : process (clk) is
		begin
			if (rst = '1') then
				debounced_sig <= '0';
				tracker <= delay_amount;
				toggled <= '0';
			elsif (rising_edge(clk)) then
				if (input = not debounced_sig and tracker = delay_amount) then
					debounced_sig <= not debounced_sig;
					tracker <= 1;
					toggled <= '1';
				elsif (toggled = '1') then
					if (tracker = delay_amount - 1) then
						toggled <= '0';
					end if;
					tracker <= tracker + 1;
				end if;
			end if;
	end process;
end architecture;

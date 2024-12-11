library IEEE;
use IEEE.std_logic_1164.all;

entity synchronizer is
port (
	clk : in std_ulogic;
	async : in std_ulogic;
	sync : out std_ulogic
);
end entity;

architecture synchronizer_arch of synchronizer is

	signal Q1 : std_ulogic;

	begin 
	process(clk)
		begin 
		if (rising_edge(clk)) then
			Q1 <= async;
			sync <= Q1;
		end if;
	end process;
end architecture;
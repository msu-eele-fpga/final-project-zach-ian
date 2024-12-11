library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Shifter_avalon is
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
		output		: out std_ulogic_vector(7 downto 0);
		PB_Right	: in std_ulogic;
		PB_Left		: in std_ulogic
	);
end entity;

architecture Shifter_avalon_arch of Shifter_avalon is
	
	signal Pattern 	: std_logic_vector(7 downto 0) := "01010101";
	
	
	component Shifter is
		port(
			clk		: in std_logic;
			rst		: in std_logic;
			Pattern		: in std_ulogic_vector(7 downto 0);
			PB_Right	: in std_ulogic;
			PB_Left		: in std_ulogic;
			output		: out std_ulogic_vector(7 downto 0)
		);
	end component;
	
	begin
	
	avalon_register_read : process(clk) is
		begin
			if(rising_edge(clk) and avs_read = '1') then
				case avs_address is
					when "00"	=> avs_readdata <= "000000000000000000000000" & Pattern;
					when others	=> avs_readdata <= (others => '0'); -- return zeros for unused registers
				end case;
			end if;
	end process;
	
	avalon_register_write : process(clk, rst) is
		begin
			if(rst = '1') then
				Pattern	<= "01010101";
			elsif (rising_edge(clk) and avs_write = '1') then
				case avs_address is
					when "00"	=> Pattern <= avs_writedata(7 downto 0);
					when others	=> null;
				end case;
			end if;
	end process;
	
	SHIFTYSHIFTY : Shifter port map(
		clk => clk,
		rst => rst,
		Pattern => std_ulogic_vector(Pattern),
		PB_Right => PB_Right,
		PB_Left => PB_Left,
		output => output	
	);
end architecture;
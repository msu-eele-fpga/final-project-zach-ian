library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Shifter is
	port(
		clk		: in std_logic;
		rst		: in std_logic;
		Pattern		: in std_logic_vector(7 downto 0);
		PB_Right	: in std_ulogic;
		PB_Left		: in std_ulogic;
		output		: out std_ulogic_vector(7 downto 0)
	);
end entity;

architecture Shifter_arch of Shifter is
	
	component async_conditioner is
		port (
			clk	: in std_ulogic;
			rst	: in std_ulogic;
			async	: in std_ulogic;
			sync	: out std_ulogic
		);
	end component;
	
	signal PB_Right_pulse : std_ulogic;
	signal PB_Left_pulse : std_ulogic;
	signal curpat	: unsigned(7 downto 0) := "01010101";
	signal toggle	: std_logic := '0';
	signal toggle2	: std_logic := '0';
	
	begin 		
		process(clk,rst)
			begin 
			if(rst = '1') then
				curpat <= unsigned(Pattern);
			elsif(rising_edge(clk)) then
				if(PB_Right_pulse = '1') then
					curpat <= rotate_right(curpat, 1);
				elsif(PB_Left_pulse = '1') then
					curpat <= rotate_left(curpat, 1);
				elsif(toggle = toggle2) then
					curpat <= unsigned(Pattern);
					toggle2 <= not toggle;
				end if;
			end if;
		end process;
		
		process(Pattern)
			begin
			toggle <= not toggle;
		end process;
		
		output <= std_ulogic_vector(curpat);
		
		BUTTON1 : async_conditioner port map(
			clk => clk,
			rst => rst,
			async => PB_Right,
			sync => PB_Right_pulse
		);
		BUTTON2 : async_conditioner port map(
			clk => clk,
			rst => rst,
			async => PB_Left,
			sync => PB_Left_pulse
		);
		
end architecture;

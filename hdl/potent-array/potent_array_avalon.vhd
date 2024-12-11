library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity potent_array_avalon is
	port(
		clk : in std_ulogic;
		rst : in std_ulogic;
		
		-- avalon memory-mapped slave interface
		avs_read	: in std_logic;
		avs_write	: in std_logic;
		avs_address	: in std_logic_vector(1 downto 0);
		avs_readdata	: out std_logic_vector(31 downto 0);
		avs_writedata	: in std_logic_vector(31 downto 0);
		
		led_value    : out std_ulogic_vector(7 downto 0)
		);
end entity;

architecture potent_array_avalon_arch of potent_array_avalon is

	
	signal size_of_step 	   : std_ulogic := '0'; 
	signal potentiometer_value : std_logic_vector(11 downto 0) := "011111111111";
	signal boolibi             : boolean := true;
	
	component potent_array is
		port(
			clk   : in std_ulogic;
			rst   : in std_ulogic;
			step    : in boolean;
			potent  : in std_ulogic_vector(11 downto 0);
			led_out : out std_ulogic_vector(7 downto 0)
		   );
	end component;
	
  begin

    bool : process(size_of_step)
      begin
	     if (size_of_step = '0') then
		    boolibi <= false;
		  else
		    boolibi <= true;
    	end if;
	end process;



	
	avalon_register_read : process(clk) is
		begin
			if(rising_edge(clk) and avs_read = '1') then 
				case avs_address is
					when "00"	=> avs_readdata <= "0000000000000000000000000000000" & size_of_step;
					when "01"	=> avs_readdata <= "00000000000000000000" & potentiometer_value;
					when others	=> avs_readdata <= (others => '0'); -- return zeros for unused registers
				end case;
			end if;
	end process;
	
	avalon_register_write : process(clk, rst) is
		begin
			if(rst = '1') then
				size_of_step <= '1';
				potentiometer_value <= "011111111111";
			elsif (rising_edge(clk) and avs_write = '1') then
				case avs_address is
					when "00"	=> size_of_step <= avs_writedata(0);
					when "01"	=> potentiometer_value <= avs_writedata(11 downto 0);
					when others	=> null;
				end case;
			end if;
	end process;
	
	potenti : potent_array port map(
		clk  => clk,
		rst  => rst,
		step   => boolibi,
		potent  => std_ulogic_vector(potentiometer_value),
		led_out => led_value
	);
	

end architecture;
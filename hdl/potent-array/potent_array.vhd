library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity potent_array is
  port(clk   : in std_ulogic;
       rst   : in std_ulogic;
       step    : in boolean;
       potent  : in std_ulogic_vector(11 downto 0);
       led_out : out std_ulogic_vector(7 downto 0)
      );
end entity;


architecture potent_array_arch of potent_array is


  signal potentiometer_step_choice : boolean := true;

  begin

    process (clk, rst) is
      begin

      if (potentiometer_step_choice = true) then 

        if (potent > "000000000000" and potent < "001000000000") then -- 0 to 511
          led_out <= "10000000";
      
        elsif (potent > "001000000001" and potent < "001111111111") then -- 512 to 1023
          led_out <= "11000000";

        elsif (potent > "010000000000" and potent < "010111111111") then -- 1024 to 1535
          led_out <= "11100000";

        elsif (potent > "011000000000" and potent < "011111111111") then --1536 to 2047
          led_out <= "11110000";
  
        elsif (potent > "100000000000" and potent < "101000000001") then --2048 to 2561
          led_out <= "11111000";
      
        elsif (potent > "101000000010" and potent < "101111111111") then --2562 to 3071
          led_out <= "11111100";
  
        elsif (potent > "110000000000" and potent < "110111111111") then --3072 to 3583
          led_out <= "11111110";

        elsif (potent > "111000000000" and potent < "111111111111") then --3584 to 4095
          led_out <= "11111111";
        end if;


--------------------------------(Second Step option)-----------------------------------------------------
      elsif (potentiometer_step_choice = false) then
        if (potent > "000000000000" and potent < "000011111111") then -- 0 to 255
          led_out <= "10000000";
      
        elsif (potent > "000100000000" and potent < "000111111110") then -- 256 to 510
          led_out <= "01000000";

        elsif (potent > "000111111111" and potent < "001011111101") then -- 511 to 765
          led_out <= "00100000";

        elsif (potent > "001011111110" and potent < "001111111100") then --766 to 1020
          led_out <= "00010000";
  
        elsif (potent > "001111111101" and potent < "010011111011") then --1021 to 1275
          led_out <= "00001000";
      
        elsif (potent > "010011111100" and potent < "010111111010") then --1276 to 1530
          led_out <= "00000100";
  
        elsif (potent > "010111111011" and potent < "011011111001") then --1531 to 1785
          led_out <= "00000010";

        elsif (potent > "011011111010" and potent < "011111111000") then --1786 to 2040
          led_out <= "00000001";
---------------------------------(Second Half)------------------------------------------------------
        elsif (potent > "011111111001" and potent < "100011110111") then -- 2041 to 2295
          led_out <= "00000011";

        elsif (potent > "100011111000" and potent < "100111110110") then -- 2296 to 2550
          led_out <= "00000111";

        elsif (potent > "100111110111" and potent < "101011110101") then -- 2551 to 2805
          led_out <= "00001111";

        elsif (potent > "101011110110" and potent < "101111110100") then --2806 to 3060
          led_out <= "00011111";

        elsif (potent > "101111110101" and potent < "110011110011") then --3061 to 3315
          led_out <= "00111111";

        elsif (potent > "110011110100" and potent < "110111110010") then --3316 to 3570
          led_out <= "01111111";

        elsif (potent > "110111110011" and potent < "111111111111") then --3571 to 4095
          led_out <= "11111111";

        end if;
      end if;

    end process;

   potentiometer_step_choice <= step;

end architecture;
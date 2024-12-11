library ieee;
use ieee.std_logic_1164.all;

entity async_conditioner is
	port (
		clk		: in std_ulogic;
		rst		: in std_ulogic;
		async	: in std_ulogic;
		sync	: out std_ulogic
	);
end entity;

architecture async_conditioner_arch of async_conditioner is
	
	component synchronizer is
		port (
			clk : in std_ulogic;
			async : in std_ulogic;
			sync : out std_ulogic
		);
		end component;
	
	component debouncer is
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
	end component;
	
	component one_pulse is
		port (
			clk		: in std_ulogic;
			rst		: in std_ulogic;
			input	: in std_ulogic;
			pulse	: out std_ulogic
		);
	end component;
	
	signal raw_sync : std_ulogic;
	signal sync_debounced : std_ulogic;
	
	begin 
	
	SYNC1 : synchronizer port map(
		clk => clk,
		async => async,
		sync => raw_sync
		);
	
	DEBOUNCE1 : debouncer 
		generic map(
			clk_period	=> 20 ns,
			debounce_time => 100 ms
		)
		port map(
			clk => clk,
			rst => rst,
			input => raw_sync,
			debounced => sync_debounced
		);	
	
	PULSE1 : one_pulse port map(
		clk => clk,
		rst => rst,
		input => sync_debounced,
		pulse => sync
	);
end architecture;

# Long — DHT20, LCD and timing adjustment

Check the LCD address and backpack wiring on the board. Record a temperature and humidity reading, unplug the DHT20, then reconnect it. Note when the display switches to dashes and how long it takes for readings to return.

In the timing menu, try 8/5/2 for red/green/amber. The LCD should show `Need R = G + A`. Change green to 6 s and confirm that Automatic mode restarts. The [controller tests](../../tests/test_controller.c) cover both cases with a simulated LCD.

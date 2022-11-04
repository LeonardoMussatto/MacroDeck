#MD_REncoder_OverMCP

This is an adaptation of Marco Colli's [MD_REncoder - Rotary Encoder Library](https://github.com/MajicDesigns/MD_REncoder) to work over an mcp23008 in the scope of this project.

## Original Features

- Debounce handling with support for high rotation speeds
- Correctly handles direction changes mid-step
- Checks for valid state changes for more robust counting and noise immunity
- Interrupt based or polling in loop()
- Counts full-steps (default) or half-steps
- Calculates speed of rotation

## Added Features

- pin setup and read from MCP23008

# LoRa-Serial-Defined-Control-Software (LoRa SDCS)

A quick, hacked-together program to allow serial control of a Feather M0 LoRa board. For work.

## How to access device...

I've found that [Termite](https://www.compuphase.com/software_termite.htm) works the best.

Settings:

| Setting          | Value     |                    |
|------------------|-----------|--------------------|
| Baud rate        | 9600      |                    |
| Flow control     | RTS/CTS   |                    |
| Transmitted Text | Append LF | Local echo = False |

Close port when inactive is very useful when flashing multiple versions one after another.

# bswap
A simple Windbg extension for swapping byte endianness.
This was based off the simple (legacy) Windbg plugin.
## Usage
.load C:\Path\To\Binary\bswap.dll<br>
!bswap 1234abc<br>
Output:<br>
```
Little Endian: 1234ABC
Big Endian: BC4A230100000000
```

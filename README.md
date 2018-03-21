# lnclk
`lnclk` (pronounced lineclock) is a minimalist clock displaying time as a progress bar of 1 pixel of height.
It should display as a dock in the top of the screen in most EWMH-compliant Window Managers (tested in i3 and KWin).

# Installation
You'll need the [XCB](https://xcb.freedesktop.org/) library in order to build from source.

Clone the repository : 
`git clone https://github.com/Askrenteam/lnclk.git`

Build the files :
`make`

# Configuration
The configuration file is located at `~/.config/lnclk/config`.
You can configure the background(`bg`) and foreground(`fg`) colors of the bar (`#rrggbb` values).
You can also configure the time unit (`tu`) displayed (seconds, minutes, hours). Default is seconds.
Lines that start with a '#' are comments.
Example configuration file : 
```
# Foreground color
fg = #906010
# Background color
bg = #101010
# Time unit
tu = minutes
```

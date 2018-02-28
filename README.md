This is a command-line program for playing with Conway's game of life and other life-like cellular automata.

Use:

`clife -m mapfile [-r rulestring]`

`clife -d width:height [-s [density]] ...`

Description of options:

`-m mapfile`
Load mapfile, is a 2d bitfield with newlines separating rows

`-d width:height`
Loads a map with given dimentions. Map will be empty unless `-s` option is supplied.

`-s density`
Generates a random initial pattern with a given density.

`-r rulestring`
Specify an alternate ruleset in B/S format.

Example:
`clife -d 50:50 -s .75 -r B3/S23`
Loads a 50x50 map with a random initialization of density 75% using Conway's ruleset.


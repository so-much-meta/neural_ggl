Note: Just uploading a very old TD/neural net game learning program I wrote
in 2007. There's probably not much use for this at this point, except for curiosity.

A generalized game learner implementation:

Written by so-much-meta


Notes: (5/9/07)
The source code is in *terrible* shape. There are very few comments and magical numbers still hang around. The GTP interface implementation was created very hastily. Extensive use of inlined class functions in header files was done to ensure inlining of code (moving it to .cpp files had caused performance problems).

All logs containing the original data used in my paper can be found in the various logs directories.

The makefile is set to compile specifically for an Athlon64 architecture on a 32-bit OS. Performance on an Athlon64 3400+ is at about 7 seconds to play 300 games of Othello with a (128,100,1) sized network (input,hidden,output).

tictactoe.h is completely commented out as it hadn't been updated since making a few changes to the Game class involving saving the game state onto a "stack"

piano-fft
=========

I have a project I'm working on where eventually I aim to have a device that listens to the sound in the room and lights up my piano/keyboard keys in accordance to the frequency of each key. Kind of like overlaying a graphic equaliser over my keyboard. I'm mainly doing it because I think it would look nice, but it could end up being fun when I jam with people to see what notes they're playing.

The first stage is to work out how FFT works and get a simulated version of what I want working on my PC. The stage after that is actually porting the code to a microcontroller.

Currently I'm playing around with using KissFFT to analyse WAV files and it was working decently, that was until I decided to change everything around.

High Priority TODO:
Get it working again.
Provide another method to run the program than win32.

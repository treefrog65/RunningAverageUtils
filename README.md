# Running Average Utility
A small library used to create a running average of data. Currently used to smooth analog inputs on an Arduino Pro Micro for use as a game controller.

## RunningAverage.h
Allows for any value to be added to the running average.

## RunningAverageHysteresis.h
This version only permits values outside of a certain window to be added to the running average in an attempt to further reduce noise.

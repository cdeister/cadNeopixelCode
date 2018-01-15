%% connect to the feather with the matrix.
comPath = '/dev/cu.usbmodem1451'; %COMX on windows. Change this maybe.
ledMatrix=serial(comPath,'BaudRate',9600);
fopen(ledMatrix);
flushinput(ledMatrix);

%% some example commands, this will set the upper left pixel to red for 5 seconds.
% always set 'o' last!
fprintf(ledMatrix,'i255>');
fprintf(ledMatrix,'r126>');
fprintf(ledMatrix,'p0>');
fprintf(ledMatrix,'q1>');
fprintf(ledMatrix,'d5000>');
fprintf(ledMatrix,'m1>');
fprintf(ledMatrix,'o1>');

%% another example try all red
fprintf(ledMatrix,'i255>');
fprintf(ledMatrix,'b126>');
fprintf(ledMatrix,'r56>');
fprintf(ledMatrix,'p-1>');
fprintf(ledMatrix,'d2500>');
fprintf(ledMatrix,'m2>');
fprintf(ledMatrix,'o1>');


%% clean up; matlab reserves the board.

flushinput(ledMatrix)
fclose(ledMatrix)
delete(ledMatrix)
clear all
close all
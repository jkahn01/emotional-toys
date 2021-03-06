% clear all variables and open figures, create an arduino object
clear all;
close all;
a=arduino('COM12');

rate = 64; %Hz
delay = 1/64;
data_length = 60; %seconds

data = zeros(data_length*rate, 2);
i = 1;

tic
start_time = clock;
last_sample = 0;

while(toc < data_length);
    if(toc-last_sample >= delay);
        last_sample = toc;
        data(i, 1) = last_sample;
        data(i, 2) = a.analogRead(0);
        i = i + 1;
    end
end

% save output to file
start_time = datestr(start_time,'HHMMSS');
filename = strcat('a_out_',start_time,'.csv');
csvwrite(filename,data);
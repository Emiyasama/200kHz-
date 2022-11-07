clear
clc
close all
fs=9000000;             %采样频率
N = 90;
t=0:1/fs:(N-1)/fs;
y= 0.5+0.5*cos(2*pi*200000*t-pi/3);
figure;plot(t,y);xlabel('t/s');ylabel('信号幅度')

df=fs/N;        %分辨率
f=(0:N-1)*df;       %其中每点的频率
Y=fft(y)/N*2;  %真实的幅值

% 计算信号相位
tol = 1e-6;
Y(abs(Y) < tol) = 0;
theta = angle(Y(1:N/2))/pi;

figure(2);
plot(f(1:N/2),theta)
ylabel('相位/pi')
disp("y= 0.5+0.5*cos(2*pi*200000*t-pi/3);");
for i=1:N/2
    if abs(theta(i))~=0
        disp(f(i))
        disp(theta(i)*180)
    end
end
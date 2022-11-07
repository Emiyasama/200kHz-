% % AM调制
% dt=0.001;%时间采样间隔
% fmax=1;  %信源最高频率
% fc=10;   %载波频率
% T=5;     %信号时长
% N=T/dt;  %采样点数
% t=[0:N-1]*dt %采样时间
% mt=sqrt(2)*cos(2*pi*fmax*t)%信源
% %AM_modulation
% A=2;
% s_am=(A+mt).*cos(2*pi*fc*t);
% 
% %power spectrum density 功率谱密度
% [f,Xf]=FFT_SHIFT(t,s_am); %调制信号频谱,这里调用了编写的FFT_SHIFT（）函数
% PSD=(abs(Xf).^2)/T %调制信号功率谱密度
% 
% figure
% subplot(211);plot(t,s_am)%AM信号波形
% hold on;
% plot(t,A+mt,'r--');    %AM包络
% title('AM调制信号及其包络')
% xlabel('t');
% subplot(212);plot(f,PSD)%AM信号功率谱波形
% axis([-2*fc 2*fc 0 1.5*max(PSD)]);
% title('AM调制信号功率谱')
% xlabel('f');

%% 仿真调制与包络
dt = 5.55e-7; %采样间隔
fmax = 2e4; %信源最高频率20k
fc = 2e5;      %载波频率200k
T = 0.0002;     %信号时长
N = T/dt;      %采样点数
t =[0:N-1]*dt; %采样时间
tiao = 1*sin(2*pi*fmax*t-0.5*pi);

A=1;
%画信号源（调制信号）
figure
subplot(3,1,1);
plot(t,A+tiao)
hold on;
title('调制信号')
xlabel('t');

zai=0.5+0.5*sin(2*pi*fc*t);
ym=(A+tiao).*zai;
subplot(3,1,2);
plot(t,zai)
hold on;
title('载波信号')
xlabel('t');

subplot(3,1,3);
plot(t,ym)
hold on;
plot(t,A+tiao,'r--');    %AM包络
title('调幅波及包络')
xlabel('t');

%% 画数据图
t1 = transpose(table2array(boxing(:,3)));
tiaofu = transpose(table2array(boxing(:,4)));
zaibo = transpose(table2array(boxing(:,5)));
fashe = transpose(table2array(boxing(:,6)));
t2 = transpose(table2array(baoluo(:,1)));
baoluo = transpose(table2array(baoluo(:,2)));

figure
subplot(3,1,1);
plot(t1,tiaofu)
hold on
title('调幅波')

subplot(3,1,2);
plot(t1,zaibo)
hold on
title('载波')

subplot(3,1,3);
plot(t1,fashe)
hold on
% plot(t1,tiaofu,'r--');    %AM包络
% hold on
plot(t2,baoluo,'r.-');    %采样包络
title(['发射波'])


%% 解调仿真
t3=transpose(table2array(jietiao(:,2)));
xt=transpose(table2array(jietiao(:,3)));
yt=transpose(table2array(jietiao(:,4)));
ym=transpose(table2array(jietiao(:,5)));
ym_yt=transpose(table2array(jietiao(:,6)));
lvbo=transpose(table2array(jietiao(:,8)));

subplot(4,1,1);
plot(t3,xt)
hold on
plot(t3,lvbo,'r--')
hold on
title('调幅波xt与解调后滤波结果')

subplot(4,1,2);
plot(t3,yt)
hold on
title('载波yt')

subplot(4,1,3);
plot(t3,ym)
hold on
title('发射波ym')

subplot(4,1,4);
plot(t3,ym_yt)
hold on
title('解调时乘法结果ym*yt')

%% 计算相位
fs=900000; %采样频率
N=100;     %采样点数
%频率分辨率为fs/N＝1Hz，下面信号的频率20是1的整数倍20，即为整周期采样
n=0:N-1;% n=0---99
t=n/fs;    %时间数组，n*1/fs
f0=20;     %设定余弦信号频率
x=0.5+0.5*cos(2*pi*f0*t-pi);%生成正弦信号 %FFT是余弦类变换，最后得到的初始相位是余弦信号的初时相位，在这里为0。如果信号
figure(1);          %  调出第一张图                  
%若为x=sin(2*pi*f0*t);则初时相位应该是－90度而非0度。
subplot(211);%  表示subplot（m,n,p）有 m行 n列 第几个图
plot(t,x);%作余弦信号的时域波形
xlabel('t');
ylabel('y');
title('余弦信号 时域波形');
grid;
%进行FFT变换并做频谱图
y=fft(x,N);        %进行fft变换，返回X(f)
mag=abs(y)*2/N;    %求幅值 乘上后面的2/N得到正确幅值
f=(0:length(y)-1)*fs/N;%进行对应的频率转换
subplot(212);
plot(f(1:N),mag(1:N));%做频谱图
xlabel('频率(Hz)');
ylabel('幅值');
title('余弦信号 幅频谱图');
grid;
angle(y(21))*180/pi %求信号初时相位。频率坐标f为[0 0.01 0.02 0.03 0.04 0.05 0.06 ...]，所以谱线y中第6根谱线和信号x对应。


%% 计算相位2
clear
clc
close all
fs=9000000;             %采样频率
N = 90;
t=0:1/fs:(N-1)/fs;
y= 0.5+0.5*cos(2*pi*200000*t-pi/3);
figure;plot(t,y);xlabel('t/s');ylabel('信号幅度')
 
% fft单边谱
df=fs/N;        %分辨率
f=(0:N-1)*df;       %其中每点的频率
Y=fft(y)/N*2;  %真实的幅值
figure(2)
subplot(211)
plot(f(1:N/2),abs(Y(1:N/2)));xlabel('f/Hz');ylabel('单边频谱')
title('单边频谱')

% 计算信号相位
tol = 1e-6;
Y(abs(Y) < tol) = 0;
theta = angle(Y(1:N/2))/pi;

figure;
plot(f(1:N/2),theta)
ylabel('相位/pi')
disp("y= 0.5+0.5*cos(2*pi*200000*t-pi/3);");
for i=1:N/2
    if abs(theta(i))~=0
        disp(f(i))
        disp(theta(i)*180)
    end
end


%% 计算相角3!!!!!!!!!!!!!!!!!!!!!!!成功！！！！！！！！！！！！！！！！！！！！
fs= 900000;             %采样频率
N = 90;

t =transpose(table2array(xiangjiao(:,2)));
y =transpose(table2array(xiangjiao(:,3)));
figure;plot(t,y);hold on;xlabel('t/s');ylabel('信号幅度')

% fft单边谱
df=fs/N;        %分辨率
f=(0:N-1)*df;       %其中每点的频率
Y=fft(y)/N*2;  %真实的幅值
figure(2)
subplot(211)
plot(f(1:N/2),abs(Y(1:N/2)));xlabel('f/Hz');ylabel('单边频谱')
title('单边频谱')

% 计算信号相位
tol = 1e-6;
Y(abs(Y) < tol) = 0;
theta = angle(Y(1:N/2))/pi;

figure;
plot(f(1:N/2),theta)
ylabel('相位/pi')

for i=1:N/2
    if abs(theta(i))~=0
        disp(f(i))
        disp(theta(i)*180)
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%保存%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% ADC采集数据
t =transpose(table2array(ADC(:,2)));
y =transpose(table2array(ADC(:,3)));
figure;plot(t,y);hold on;xlabel('t/s');ylabel('信号幅度')


%% 废稿
% Tyi = 3e-6;
% b=[];
% d=[];
% figure
% for i = 1:48
%     a = (A+sin(2*pi*fmax*i*dt-0.5*pi)).*sin(2*pi*fc*i*dt);
%     b(i)=a;
% end
% for j = 1:5
%     d(j) = b(8*j+1);
% end
% 
% k=[1:48];
% subplot(1,1,1);
% plot(k,b,'.',k,b,'k');
% hold on;
% p=[8,16,24,32,40];
% plot(p,d,'r--');
% title('采样点与包络')
% xlabel('t');
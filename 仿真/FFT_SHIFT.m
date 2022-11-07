function[f,sf]=FFT_SHIFT(t,st)
dt=t(2)-t(1);
T=t(end);
df=1/T;
N=length(st);
f=[-N/2:N/2-1]*df;
sf=fft(st);
sf=T/N*fftshift(sf);

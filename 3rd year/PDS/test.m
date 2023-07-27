%% Gravação de áudio

%micro = audiorecorder(8000,8,1);    % Criação do objeto audiorecorder
%disp("Start speaking.");            
%recordblocking(micro,5);            % Gravação de 5 segundos
%disp("End of recording.");          
%audiorec = getaudiodata(micro);     % Armazenar os dados num array
%plot(audiorec);                     % Gráfico dos dados
%sound = play(micro);                % Ouvir a gravação
%disp('Properties of Sound:');       
%get(sound);                         % Propriedades do som gravado
%audiowrite('som.wav',audiorec,8000);

%% Valores para projeção do filtro
 
[audiorec,Fs]=audioread('som.wav');
fourier = fft(audiorec);
plot(audiorec);
N = 6;                  % Fator de subamostragem
Ny = Fs/2;              % Frequência de Nyquist   
Rp = 40;                % ripple na banda passante
Rs = 60;                % ripple na banda de rejeição
OmegaP = (Ny/N);        % Frequência da banda passante
OmegaS = 1.2*OmegaP;    % Frequência da banda de rejeição
Wp = OmegaP/Ny;
Ws = OmegaS/Ny;
Ts = 1/8000;

%% Filtro de Chebyshev tipo I

[n,Wn] = cheb1ord((2/Ts)*tan(Wp/2),(2/Ts)*tan(Ws/2),Rp,Rs,'s');
%[n,Wn] = cheb1ord((2)*tan(Wp/2),(2)*tan(Ws/2),Rp,Rs,'s');

%% Criação do filtro

[B,A] = cheby1(n,Rp,Wn,'s');                 % Criação do filtro
[Num,Den] = bilinear(B,A,1);                 % Transformação bilinear
filteredSignal = filter(Num,Den,audiorec);
decimado = downsample(audiorec,N);
filteredSignal2 = filter(Num,Den,decimado);
fourier2 = fft(decimado);
fourier3 = fft(filteredSignal);
fourier4 = fft(filteredSignal2);
figure(4);
subplot(4,1,1);stem(audiorec);title('Audio original');
subplot(4,1,2);stem(filteredSignal);title('Audio filtrado');
subplot(4,1,3);stem(filteredSignal2);title('Audio filtrado e decimado');
player = audioplayer(filteredSignal2,Fs/N);
pause(5);
play(player);
figure(5);
plot(audiorec);hold on;plot(filteredSignal,'r');legend('Som original','Som filtrado');
figure(6);
plot(decimado,'g');hold on;plot(filteredSignal2,'y');legend('Som decimado','Som filtrado e decimado');
figure(7);
subplot(7,1,1);plot(abs(fourier));hold on;title('Audio original');
subplot(7,1,2);plot(abs(fourier3));hold on;title('Audio filtrado');
subplot(7,1,3);plot(abs(fourier4));hold on;title('Audio filtrado e decimado');
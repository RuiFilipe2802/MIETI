%% Gravação de áudio

%micro = audiorecorder(8000,8,1);        % Criação do objeto audiorecorder
%disp("Start speaking.");            
%recordblocking(micro,5);                % Gravação de 5 segundos
%disp("End of recording.");          
%audiorec = getaudiodata(micro);         % Armazenar os dados num array
%plot(audiorec);                         % Gráfico dos dados
%sound = play(micro);                    % Ouvir a gravação
%disp('Properties of Sound:');       
%get(sound);                             % Propriedades do som gravado
%audiowrite('som.wav',audiorec,8000);    % Write do ficheiro
%fourier = fft(audiorec);
[audiorec,Fs]=audioread('som.wav');     % Read do ficheiro
pause(5);                               % Pausa para ouvir o som original

%% Ciclo para incrementar o fator de subamostragem

for N=2:2:8
    console = ['N -> ',num2str(N)];
    disp(console);
    %% Parâmetros para a realização do filtro
    
    plot(audiorec);
    Ny = Fs/2;              % Frequência de Nyquist   
    Rp = 40;                % ripple na banda passante
    Rs = 60;                % ripple na banda de rejeição
    OmegaP = (Ny/N);        
    OmegaS = 1.2*OmegaP;    
    Wp = OmegaP/Ny;         % Frequência da banda passante
    Ws = OmegaS/Ny;         % Frequência da banda de rejeição
    Ts = 1/8000;            % Ts -> Período de amostragem

    %% Filtro de Chebyshev tipo I

    [n,Wn] = cheb1ord((2/Ts)*tan(Wp/2),(2/Ts)*tan(Ws/2),Rp,Rs,'s');

    %% Criação do filtro e transformação bilinear

    [B,A] = cheby1(n,Rp,Wn,'s');                  % Criação do filtro
    [Num,Den] = bilinear(B,A,1);                  % Transformação bilinear
    
    %% Filtragem do sinal e decimação
    
    filteredSignal = filter(Num,Den,audiorec);    % Sinal filtrado
    decimado = downsample(audiorec,N);            % Decimação
    filteredSignal2 = filter(Num,Den,decimado);   % Sinal decimado filtrado
    fourier2 = fft(decimado);
    fourier3 = fft(filteredSignal);
    fourier4 = fft(filteredSignal2);
    
    %% Plot de gráficos e criação do áudio
    
    figure(3);
    subplot(3,1,1);stem(audiorec);title('Audio original');
    subplot(3,1,2);stem(filteredSignal);title('Audio filtrado');
    subplot(3,1,3);stem(filteredSignal2);title('Audio filtrado e decimado');
    player = audioplayer(filteredSignal2,Fs/N);
    play(player);
    figure(4);
    plot(audiorec);hold on;plot(filteredSignal,'r');legend('Som original','Som filtrado');
    figure(5);
    plot(decimado,'g');hold on;plot(filteredSignal2,'y');legend('Som decimado','Som filtrado e decimado');
    figure(6);
    subplot(6,1,1);plot(abs(fourier));hold on;title('Audio original');
    subplot(6,1,2);plot(abs(fourier3));hold on;title('Audio filtrado');
    subplot(6,1,3);plot(abs(fourier4));hold on;title('Audio filtrado e decimado');
    pause(5);
end
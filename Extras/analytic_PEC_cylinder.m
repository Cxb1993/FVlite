% Octave file
% Produces data set for plane waves on a PEC cylinder.

N_points = 1000;     % number of points
N_bessel = 50;      % max bessel function order
N_prec   = 1000;    % number of points used to sample bessel functions
phi_max  = 2*pi;    % max phi
H_0      = 1;       % incident field magnitude
omega    = 1e10;    % frequency
c        = 3e8;     % speed of light
k        = omega/c; % wavenumber
R        = 0.125;   % cylinder radius

phi = [0 : phi_max/(N_points-1) : phi_max];
H   = zeros(1,N_points);

rho       = k*R;    
scattered = 0;

bessel=zeros(1,2*N_bessel+1);
hankel=zeros(1,2*N_bessel+1);
bessel_der=zeros(1,2*N_bessel+1);
hankel_der=zeros(1,2*N_bessel+1);

for ii=1:2*N_bessel+1
    bessel(ii)=besselj(ii-N_bessel-1,rho);
    hankel(ii)=besselh(ii-N_bessel-1,1,rho);
    bessel_der(ii)=0.5*(besselj(ii-N_bessel-2,rho)-besselj(ii-N_bessel,rho));
    hankel_der(ii)=0.5*(besselh(ii-N_bessel-2,1,rho)-besselh(ii-N_bessel,1,rho));
endfor

%bessel_der=zeros(1,2*N_bessel+1);
%hankel_der=zeros(1,2*N_bessel+1);
%
%for ii=1:2*N_bessel+1
%    bessel_der(ii) = 0.5*(bessel(ii)-bessel(ii+2));
%    hankel_der(ii) = 0.5*(hankel(ii)-hankel(ii+2));
%endfor
%
%bessel=bessel(2:end-1);
%hankel=hankel(2:end-1);
bessel_term=0;

for ii = 1:N_points
    for jj = 1:2*N_bessel+1
        kk=jj-N_bessel-1;
        bessel_term = bessel(jj)-hankel(jj)*bessel_der(jj)/hankel_der(jj);
        H(ii) = H(ii) + H_0 * (1j)^kk * bessel_term * exp(1j*kk*phi(ii)); 
    endfor
endfor

phi.*=180/pi;
Habs = abs(H);
%HabsMax = max(Habs);
HabsMax=1.0;
Habs = Habs/HabsMax;

file_id=fopen('exact_data.txt','w');
fprintf(file_id,'# phi\n# H field (normalised) \n');
for ii=1:N_points
    fprintf(file_id,'%f\t%f\n',phi(ii),Habs(ii));
endfor
fclose(file_id);

plot(phi,Habs)
%plot(phi,real(H))
%plot(phi,imag(H))
xlabel("Phi /degrees")
ylabel("H field")
axis([0,360])
set(gca,"xgrid","on","ygrid","on","xtick",0:60:360)

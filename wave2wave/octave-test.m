################################################################################
## M-x run-octave    start repl

## C-M-RET send line
## C-, C-RET send paragraph
## C-c C-l send buffer
## C-c M-l clear repl

## run this on a shell to monitor memory usage
## watch -n 5 free -m

################################################################################


################################################################################
### CONFIGURE FFTW
################################################################################



################################################################################
### GLOBALS
################################################################################
global MATERIALS_DIR =  strcat(pwd(),"/materials/");

# it is important to know the original and materials before loading
# them, in order to allocate the proper amount of memory
global ORIGINAL_ADDRESS = (strcat(MATERIALS_DIR, "child-short.wav"));
## global MATERIAL_ADDRESSES =arrayfun(@(x) strcat(MATERIALS_DIR,x{1}),
##                                     {"anvil.wav", "triangle.wav",
##                                      "flexatone.wav", "recoreco.wav"},
##                                     "UniformOutput", false)(:);
global MATERIAL_ADDRESSES =arrayfun(@(x) strcat(MATERIALS_DIR,"anvil[",num2str(x),"].wav"),[-500:4:500], "UniformOutput", false)(:);

global SAMPLERATE = 44100;


################################################################################
### LOADING AND PREPROCESSING AUDIO FILES
################################################################################
function outsig = remove_dc(sig)
  m = sum(sig)/length(sig);
  outsig = sig-m;
endfunction

function outsig = normalize_sig(sig)
  outsig = sig/max(abs(sig));
endfunction

function outsig = preprocess_sig(sig, outsize=0)
  outsig = normalize_sig(remove_dc(sig));
  if (outsize) outsig = postpad(outsig, outsize)(1:outsize);
  endif
endfunction

function sig = load_and_preprocess(filepath, outsize=0)
  sig = auload(filepath);
  sig = preprocess_sig(sig, outsize);
endfunction





################################################################################
### CROSS-CORRELATION
################################################################################

function newsize = padsize(sig)
  newsize = 2^nextpow2(length(sig));
endfunction

function newsize = maxpadsize(sig1, sig2)
  newsize = max(padsize(sig1), padsize(sig2))
endfunction

## function outsig = fftcc(sig1, sig2)
##   if length(sig1)<length(sig2)
##     sig1 = flipdim(sig1);
##   else
##     sig2=flipdim(sig2);
##   endif
##   outsig = fftconv(sig1, sig2);#, maxpadsize(sig1, sig2));
##   # cut the length of the first signal, to provide exact delay
##   ## outsig = outsig(length(sig1):end);
## endfunction

function outsig = fftcc(sig1, sig2)
  outsig = fftconv(sig1, flipdim(sig2));
endfunction

function [cc] = plot_cc(sig1, sig2)
  cc = fftcc(sig1, sig2);
  clf;
  subplot(3,1,1); plot(sig1);xlim([0, length(cc)]);
  subplot(3,1,2); plot(sig2);xlim([0, length(cc)]);
  subplot(3,1,3); plot(cc);xlim([0, length(cc)]);
endfunction


#### TESTING CROSS-CORRELATION WITH SOME NOISE
function pos = test_cc(del)
  range = 1:1:2000;
  noise = 2*rand(1, 1000)-1;
  noise_delayed = [zeros(1, del), noise];
  ##cc = testcc(noise, noise_delayed);
  cc = plot_cc(noise_delayed, noise);
  [_, pos] = max(cc);
endfunction

### HELPFUN FOR STORING 2D indexes into CCM
function x = hashIdx(a,b)
  stridx = strcat(mat2str(a), mat2str(b));
  x = md5sum(stridx, true);
endfunction


## for M samples, maps the indexes of the upper-triangular MxM matrix
## to natural numbers. This is necessary to store them as rows of
## the CCM matrix
function x = hashTriangle(a,b, M)
  x = M+a-b;
  x = (x+1)*x/2+1-a;
endfunction



## ## test hashtriangle:
## M = 5;
## for i=1:M
##   for j=(i+1):M
##     hashTriangle(i,j,M)
##   endfor
## endfor

################################################################################
### LOAD AUDIOS, MAKE CROSS-CORRELATIONS. DICTIONARY APPROACH
################################################################################

## # load original
## fprintf("loading original: %s\n", ORIGINAL_ADDRESS);
## original = load_and_preprocess(ORIGINAL_ADDRESS);



## # load materials
## for iter=1:length(MATERIAL_ADDRESSES)
##   filename = MATERIAL_ADDRESSES{iter};
##   fprintf("loading material: %s\n", filename);
##   materials.(mat2str(iter)) = load_and_preprocess(filename);
## endfor

## # make CCS: the CCs between materials and original
## CCS.origSize = length(original);
## for iter = 1:length(fieldnames(materials))
##   #fprintf("adding CC[S, m%d] into CCS.(%d)\n", iter, iter);
##   CCS.(mat2str(iter)) = fftcc(original, materials.(mat2str(iter)));
## endfor


## # make CCMs: the CCs among materials,whereas idx(m1)<idx(m2)
## for iter1=1:length(fieldnames(materials))
##   m1 = materials.(mat2str(iter1));
##   for iter2=(iter1+1):length(fieldnames(materials))
##     m2 = materials.(mat2str(iter2));
##     idx = hashIdx(iter1, iter2);
##     #fprintf("adding CC[m%d, m%d] into CCM.(%d)\n", iter1, iter2, idx);
##     CCM.(idx) = fftcc(m1, m2);
##   endfor
## endfor



################################################################################
### LOAD AUDIOS, MAKE CROSS-CORRELATIONS. MATRIX APPROACH
################################################################################

# load original
fprintf("loading original: %s\n", ORIGINAL_ADDRESS);
original = load_and_preprocess(ORIGINAL_ADDRESS);



# load materials
materials_max_length = 0;
for iter=1:length(MATERIAL_ADDRESSES)
  filename = MATERIAL_ADDRESSES{iter};
  fprintf("loading material: %s\n", filename);
  materials.(mat2str(iter)) = load_and_preprocess(filename);
  mlength = length(materials.(mat2str(iter)));
  if (materials_max_length<mlength)
    materials_max_length = mlength;
  endif
endfor

N = length(fieldnames(materials));


## allocate CCS matrix of HORIZONTAL VECTORS!!
CCS = zeros(N, materials_max_length+length(original)-1);
CCS_ZERO_IDX = 1+columns(CCS)-length(original); ##the idx for d=0

## fill CCS:  at 1+columns-length(original)
for iter=1:N
  cc = fftcc(original, materials.(mat2str(iter)));
  CCS(iter, 1+columns(CCS)-length(cc):end) = cc;
endfor
## retrieve <S, mi> for di = x:  CCS(i, di+CCS_ZERO_IDX)





## ## allocate CCM matrix of HORIZONTAL VECTORS!!
## CCM = zeros(N*(N-1)/2, 2*materials_max_length-1);
## ## each ZERO INDEX will be different: it cant be centered, because
## ## length(CC) can be here odd or even. So all of them begin at idx=1,
## ## and d=sync will be at idx=length(m2)

## # fill CCM: the CCs among materials,whereas idx(m1)<idx(m2).
## # the d0 of mi,mj will be at CCM(hashTriangle(i,j,M), length(mj))
## for iter1=1:N
##   m1 = materials.(mat2str(iter1));
##   for iter2=(iter1+1):N
##     m2 = materials.(mat2str(iter2));
##     idx = hashTriangle(iter1, iter2, N);
##     cc = fftcc(m1, m2);
##     #fprintf("adding CC[m%d, m%d] into CCM.(%d)\n", iter1, iter2, idx);
##     CCM(idx, 1:length(cc)) = cc;
##   endfor
## endfor

## ## check that it works: this should sound the same
## ## sound(fftcc(materials.("1"), materials.("61")), 44100);
## ## sound(CCM(1,:), 44100)

################################################################################
### ALGORITHM: FILL d VECTOR
################################################################################


N; # is the number of samples
MAX_ITERATIONS = 2000;
d = zeros(2,MAX_ITERATIONS); # sample i: d(1, i)   delay: d(2,i)
#make reconstruction
reconstruction = zeros(length(original), 1); # vert. vector
#original = [original; zeros(length(reconstruction)-length(original), 1)];
iter = 1;
cc_buffer = zeros(1, columns(CCS));
while (iter <= MAX_ITERATIONS)
  # pick random m_id and set it as key in d
  m_id = unidrnd(N);
  d(1,iter) = m_id;
  # calculate cc with existing reconstruction: d0 position at CCS_ZERO_IDX
  m_i = materials.(num2str(m_id));
  m_length = length(m_i);
  cc_buffer(1+columns(CCS)-(length(original)+m_length-1):end) = fftcc(reconstruction, materials.(num2str(m_id)));
  # calculate optimal position for m_i
  [d_val, d_pos] = max(CCS(m_id,:) - cc_buffer);
  # add the corresponding delay to d
  delay = d_pos-CCS_ZERO_IDX;
  d(2,iter) = delay;
  # update reconstruction:
  reconstruction(delay:(delay+m_length-1)) = reconstruction(delay:(delay+m_length-1)) + m_i;
  # print current residual energy and update while index
  residual = original-reconstruction;
  fprintf("\ncurrent E[R]=%f\n", residual' * residual);
  iter = iter+1;
endwhile
audio_filename = strcat("reconstruction", strftime ("%Y-%m-%d-%H:%M:%S", localtime (time ())), ".wav");
wavwrite(normalize_sig(reconstruction), 44100, audio_filename);

fprintf("\nsaved reconstruction into %s\n", audio_filename);
toc()

#wavwrite(original(1:44100*3.5), 44100, "child-short.wav");



################################################################################
### TESTING/PLAYING AUDIO FILES AS VECTORS
### add  global sound_play_utility = "aplay"; to ~/.octaverc if player
### not working
################################################################################


## function [e] = energy(sig)
##   e = sig'*sig;
## endfunction

## function [v] = snd(snd_id):## BUG: THIS DOESNT COMPILE...
##   v=0;
##   if (snd_id==0)
##     global original;
##     v = original;
##   else
##     global materials;
##     v = materials(:,snd_id);
##   endif
## endfunction

## function []= play(snd_id, fs=44100)
##   ##play(0) ## play original
##   if (snd_id==0)
##     global original;
##     sound(original,fs);
##   else
##     global materials;
##     sound(materials(:,snd_id), fs);
##   endif
## endfunction


## old boy







## ## memory usage at begin
## Total memory usage by GNU Octave (VmSize):    478.508 MB
##  RSS Size:                                    53.9961 MB
##  shared pages:                                        34.2617 MB
##  data + stack size:                           197.336 MB
##  Physical Memory (RAM):                       7884.16 MB

#wavwrite(original(1:44100*3.5), 44100, "child-short.wav");



## cardew: stockhausen serves imperialism
## FFM: konferenzen manierismus (CM mahnkopf)
## soundfile++

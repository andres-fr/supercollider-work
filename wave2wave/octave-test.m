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
global ORIGINAL_ADDRESS = (strcat(MATERIALS_DIR, "child.wav"));
global MATERIAL_ADDRESSES =arrayfun(@(x) strcat(MATERIALS_DIR,x{1}),
                                    {"anvil.wav", "triangle.wav",
                                     "flexatone.wav", "recoreco.wav"},
                                    "UniformOutput", false)(:);
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


# LOAD ORIGINAL AND ALLOCATE CORRESPONDING MATRIX OF MATERIALS:
global original = load_and_preprocess(ORIGINAL_ADDRESS);
global materials = zeros(rows(original), rows(MATERIAL_ADDRESSES));
# LOAD EACH MATERIAL INTO A VECTOR OF THE materials MATRIX
for i=1:rows(MATERIAL_ADDRESSES)
  materials(:,i) = load_and_preprocess(MATERIAL_ADDRESSES{i}, rows(original));
endfor


################################################################################
### TESTING/PLAYING AUDIO FILES AS VECTORS
### add  global sound_play_utility = "aplay"; to ~/.octaverc if player
### not working
################################################################################
function [e] = energy(sig)
  e = sig'*sig;
endfunction

function []= play(snd_id, fs=44100)
  ##play(0) ## play original
  if (snd_id==0)
    global original;
    sound(original,fs);
  else
    global materials;
    sound(materials(:,snd_id), fs);
  endif
endfunction


################################################################################
### SANDBOX
################################################################################


## 3) un test sencillo:
##    mezclar 3 materiales A, B, C con delays distintos
##    proponer 10 materiales entre los que estan A,B,C
##    coger los 3 que mas CC dan, y reconstruir
##    escuchar la reconstruccion




function newsize = padsize(sig)
  newsize = ceil(log2(length(sig)));
  newsize = 2**newsize;
endfunction

function newsize = maxpadsize(sig1, sig2)
  newsize = max(padsize(sig1), padsize(sig2))
endfunction

function outsig = fftcc(sig1, sig2)
  if length(sig1)<length(sig2)
    sig1 = flip(sig1);
  else
    sig2=flip(sig2);
  endif
  outsig = fftconv(sig1, sig2);#, maxpadsize(sig1, sig2));
endfunction





################################################################################
### TESTS
################################################################################


#########  TESTING CROSS-CORRELATION WITH SOME NOISE
range = 1:1:2000;
noise = 2*rand(1, 1000)-1;
noise_delayed = [zeros(1, 500), noise];
clf;subplot(3,1,1); plot(noise);xlim([0, 3000]);
subplot(3,1,2); plot(noise_delayed); xlim([0, 3000]);
subplot(3,1,3); plot(fftcc(noise, noise));xlim([0, 3000]);

#######  TEST CONVOLUTION WITH ANVIL AS REVERB FILTER
## sound(anvil,fs);
## sound(child,fs);
test_conv = normalize_sig(fftconv(child,anvil));
wavwrite(test_conv, fs, "test.wav");



anvil*2



### TEST FFT
x=child;
clf;
subplot(2,1,1);
plot(x);
grid;
xlabel("time domain in samples");
ylabel("Signal amplitude");
subplot(2,1,2);
y=fft(x, 2*padsize(x));
plot(abs(y));
xlabel("frequency domain bins");
ylabel("spectral amplitude");


pre_post_pad([1;2;3],19, 20)

function outsig = pre_post_pad(sig, init, totalsize)
  outsig = postpad([zeros(init-1,1);sig], totalsize);
endfunction

function outsig = add_a_to_b(sig_a, sig_b, initpos)
  outsig =  sig_b+pre_post_pad(sig_a, initpos, length(sig_b));
endfunction

add_a_to_b([1;2;3], ones(10,1),3)

### TEST APP
test = child;
test = add_a_to_b(anvil, test, 44100);
test = add_a_to_b(flexatone, test, 2*44100);
test = add_a_to_b(recoreco, test, 3*44100);
wavwrite(test, fs, "test_app.wav");


## calculate respective correlations
cc_anvil = fftcc(test, anvil);
cc_flexatone = fftcc(test, flexatone);
cc_recoreco = fftcc(test, recoreco);

clf;subplot(3,1,1); plot(cc_anvil); xlim([0, length(test)]);
subplot(3,1,2); plot(cc_flexatone);xlim([0, length(test)]);
subplot(3,1,3); plot(cc_recoreco);xlim([0, length(test)]);


[val, ii ] = max(cc_flexatone)

ii-length(flexatone)

44100+29081


### TEST SVD STUFF:

# build a matrix with 3 samples and zero-pad them to the lenght of
# child

S = child;
m1 = pre_post_pad(anvil, 0, length(S));
m2 = pre_post_pad(triangle, 44100, length(S));
m3 = pre_post_pad(flexatone, 2*44100, length(S));
m4 = pre_post_pad(recoreco, 3*44100, length(S));
phi = ;

[U, E, V] = svd(phi);

size(decomp)
decomp(3)

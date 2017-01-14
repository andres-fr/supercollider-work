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
## global MATERIAL_ADDRESSES =arrayfun(@(x) strcat(MATERIALS_DIR,x{1}),
##                                     {"anvil.wav", "triangle.wav",
##                                      "flexatone.wav", "recoreco.wav"},
##                                     "UniformOutput", false)(:);
global MATERIAL_ADDRESSES =arrayfun(@(x) strcat(MATERIALS_DIR,"anvil[",num2str(x),"].wav"),[-120:4:120], "UniformOutput", false)(:);

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
  newsize = ceil(log2(length(sig)));
  newsize = 2**newsize;
endfunction

function newsize = maxpadsize(sig1, sig2)
  newsize = max(padsize(sig1), padsize(sig2))
endfunction

function outsig = fftcc(sig1, sig2)
  if length(sig1)<length(sig2)
    sig1 = flipdim(sig1);
  else
    sig2=flipdim(sig2);
  endif
  outsig = fftconv(sig1, sig2);#, maxpadsize(sig1, sig2));
  # cut the length of the first signal, to provide exact delay
  outsig = outsig(length(sig1):end);
endfunction

function [cc] = testcc(sig1, sig2)
  cc = fftcc(sig1, sig2);
  clf;
  subplot(3,1,1); plot(sig1);xlim([0, length(cc)]);
  subplot(3,1,2); plot(sig2);xlim([0, length(cc)]);
  subplot(3,1,3); plot(cc);xlim([0, length(cc)]);
endfunction


#########  TESTING CROSS-CORRELATION WITH SOME NOISE
## DELAY = 432
## range = 1:1:2000;
## noise = 2*rand(1, 1000)-1;
## noise_delayed = [zeros(1, DELAY), noise];
## cc = testcc(noise, noise_delayed);
## [_, position] = max(cc);
## position


################################################################################
### TESTS
################################################################################


# LOAD ORIGINAL AND ALLOCATE CORRESPONDING MATRIX OF MATERIALS:
global original = load_and_preprocess(ORIGINAL_ADDRESS);
global materials = zeros(rows(original), rows(MATERIAL_ADDRESSES));

# LOAD EACH MATERIAL INTO A VECTOR OF THE materials MATRIX
for i=1:rows(MATERIAL_ADDRESSES)
  materials(:,i) = load_and_preprocess(MATERIAL_ADDRESSES{i}, rows(original));
endfor


save correlations in the triangle matrix and write a getter function



function [ccs] = getCCS(i, j)
  if (i<j)
    ccs=3;
  else
    ccs=4;
  endif
endfunction



### TEST STOCHASTIC ALGO

size(original)
size(materials)



global CCS = zeros(rows(  ))



asdf = testcc(original, materials(:,50));

plot(asdf);


# CREATE AND LOAD CCS MATRIX
global materials = zeros(rows(original), rows(MATERIAL_ADDRESSES));
# LOAD EACH MATERIAL INTO A VECTOR OF THE materials MATRIX
for i=1:rows(MATERIAL_ADDRESSES)
  materials(:,i) = load_and_preprocess(MATERIAL_ADDRESSES{i}, rows(original));
endfor

cc = testcc(original, materials(:,1));
[_, position] = max(cc);
position







## calculate respective correlations
cc_anvil = fftcc(test, anvil);
cc_flexatone = fftcc(test, flexatone);
cc_recoreco = fftcc(test, recoreco);

clf;subplot(3,1,1); plot(cc_anvil); xlim([0, length(test)]);
subplot(3,1,2); plot(cc_flexatone);xlim([0, length(test)]);
subplot(3,1,3); plot(cc_recoreco);xlim([0, length(test)]);



wavwrite(test, fs, "test_app.wav");










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






# this function quickly returns an unique integer for each
# a,b combination. This is helpful for storing the entries
# of a triangular matrix in a dictionary and quickly being
# able to find them
function x = hashIdx(a,b)
  x = 2^a+3^b;
endfunction

function d = putDict(dict, key, val)
  dict.(mat2str(key)) = val;
  d = dict;
endfunction

function result = getDict(dict, key)
  result = dict.(mat2str(key));
endfunction

function d = putCCM(dict,a,b,val)
  if (a<b)
    key = hashIdx(a,b);
  else
    key = hashIdx(b,a);
  endif
  d = putDict(dict, key, val);
endfunction

function result = getCCM(dict, a, b)
  if (a<b)
    key = hashIdx(a,b);
  else
    key = hashIdx(b,a);
  endif
  result = getDict(dict, key);
endfunction

CSS.metadata = "dictionary storing CC between original and materials";

for i=1:ALL_MATERIALS
CSS = putDict(CSS,)

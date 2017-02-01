# load all .debug files, lexicographically
# plot 5 of them, each time enter is pressed plot the next 5...

x1 = dimread("1_...");

#### TESTING CROSS-CORRELATION WITH SOME NOISE
function pos = test_cc(del)
  range = 1:1:2000;
  noise = 2*rand(1, 1000)-1;
  noise_delayed = [zeros(1, del), noise];
  ##cc = testcc(noise, noise_delayed);
  cc = plot_cc(noise_delayed, noise);
  [_, pos] = max(cc);
endfunction

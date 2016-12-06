{Mix(WhiteNoise.ar(1!10)/10)}.plot

s.volume.gui




(
SynthDef("sorpresa", {arg speed=20, granulation=500, freq=1500;
	var gaussNoise = Mix(WhiteNoise.ar(1!10)/10);
	var freqspread = freq*0.6;
	var snd = Pluck.ar(WhiteNoise.ar(0.1), Impulse.kr(gaussNoise*granulation+speed),
		22100.reciprocal, (gaussNoise*freqspread+freq).reciprocal,
		60+(gaussNoise*100), coef:gaussNoise*0.3+0.5);
	Out.ar(0, Splay.ar(snd))
}).add;
w = Synth("sorpresa")
)
w.set(\speed, 10, \granulation, 500, \freq, 1500)
w.free


a.set(\freq;
f(20, 1).plot;

{MouseX.kr(0,10)}.poll;
Poll.kr(Impulse.kr(5), MouseX.kr(), "mouse x");

f = {arg a, b; a+b}
f = {|a,b| a+b}

f.value(3,4)
Impulse.kr(test)

a = [10, 11, 12, 13, 14, 15, 16, 17];
a.choose; // picks one element at random
a.size;
a.wrapAt(9)
a ++ \hi; // a Symbol is a single character
a ++ 'hi'; // same as above
a ++ "hi"; // a String is a collection of characters
a.add(44);
a.insert(5, "wow"); // inserts "wow" at position 5, pushes other items forward (returns new array)
a; // evaluate this a
a.put(2, "oops"); // put "oops" at index 2 (destructive; evaluate line above again
to check)
a.permute(3); // permute: item in position 3 goes to position 0, and vice−versa
a.mirror; // makes it a palindrome
a.powerset; // returns all possible combinations of the array's elements


[1, 2, 3, 4, "oops", 11] + 10;


10.0.sum3rand!
"hello" ! 10;

(
var r, freqs, env;
s.freeAll;
r = rrand(5, 50);
freqs = Array.fill(r, {exprand(50, 1580)});
env = Env([0, 1, 0], [rrand(0.001, 0.2), rrand(0.1,1)]);
freqs.do({arg item, count; {SinOsc.ar(item, 0, 1/r)*EnvGen.kr(env, doneAction:2)}.play});
)





// working with dictionaries

d = Dictionary.new.putPairs([a:5, b:7, c:1, d:0])
d.add(\monkey-> 0);
d.put("abc", 1)
if(d.at("asdffff")==nil,{1.postln},{2.postln})
d.at("abc")
d.atFail("fdsa", {postln("callback!!")})
d.keys
d.values
d.atAll([\monkey, "abc", \b])
d.getPairs




/*(// plays a given buffer with delay, normalization and FFT transp.
SynthDef("f_function",{ |audio_name, delay=0, mul=1, transp=0, out=0|
	var buf, bufdur, sig;
	buf = Select.kr(audio_name, ~materials); // this could be outside of the synth for optimization
	bufdur =Select.kr(material_idx, ~material_durations);// this is needed to free the synth correctly, but not optimal: a whole kr ugen per synth just for retrieving a global float, known at Synths instantiation time...
	sig = PlayBuf.ar(1, buf);
	sig = sig*Line.kr(1,1, delay+bufdur, doneAction:2);
	sig = FFT(LocalBuf(2**15), sig, wintype:1,winsize:(2**14));
	sig = PV_BinShift(sig, 1, transp, 0);
	sig = IFFT(sig)*2;
	sig = DelayN.ar(sig, delay, delay, mul);
	Out.ar(out, sig);
}).add;*/




(// load the server's buffers into client's signals
 // WARNING: this is an asynchronous task, that's why such a quirky syntax.
 // Signals live in the client, avoid playing them! use the buffers instead.
 // Server-Client interaction can be VERY slow, so minimize it
 // ~material_signals.at(5).play
 // ~materials.at(5).play
~get2powSize = {|n, padded=true| 2**(n.log2.roundUp)*if(padded,2,1)};
~zeroPadSignal = {|sig| Signal.newClear(~get2powSize.value(sig.size)).overDub(sig)};
~material_signals = Array.newClear(~materials.size);
~materials.do(
	{|buf, idx|
		buf.loadToFloatArray(action: {|arr| var sig;
			// *2 at the end because half of Signal is zero to avoid circular conv.
			sig = Signal.newClear(~get2powSize.value(arr.size));
			sig.overWrite(arr.as(Signal), 0);
			sig.normalize;
			~material_signals.put(idx, sig);
			});
});
~original_signal = nil; // asynch. calculated in the next line
~original.loadToFloatArray(action:{|arr| var sig;
	sig = Signal.newClear(~get2powSize.value(arr.size));
	sig.overWrite(arr.as(Signal), 0);
	~original_signal = sig;
});
)
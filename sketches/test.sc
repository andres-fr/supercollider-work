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











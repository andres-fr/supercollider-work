
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ABOUT WAVE2LIVE:
////////////////////////////////////////////////////////////////////////////////////////////////////
// The goal of this SC program is to load a .txt sequence of (sampleTime, normFactor, ID) tuples
// to the server in order to achieve sample-precise scheduling of its elements, each of which
// triggers a synth. The corresponding synth for each element will take the specific normFactor and
// ID as parameters, as well as further global variables held in a bus that may be changed at
// control rate. This way, a SC server running this program should be able to play the given list as
// a high-definition sequence, and incorporate efficient and flexible audio transformations at grain
// level, even in a live performance. This is the pseudo-code:
//
// Given a list L = [(d1, k1, m1), (d2, k2, m2) ...]
// Given a GLOBALS list of floats, controlled at control rate
// Given a Synth(k, m, GLOBALS)
// 1) Sort list by "d" and load it to the server as 3 buffers of same length:
//              D = [d1, d2, ...]
//              K = [k1, k2, ...]
//              M = [m1, m2, ...]
// 2) Loop when triggered:
//     idx = 0
//     for sample from 0 to D[-1]:
//         while(sample==D[idx]):
//             new Synth(K[idx], M[idx], GLOBALS)
//             idx += 1
//         wait (1/samplerate)
//
// The SC server is a feasible choice to implement this: it has a broad audiorate functionality that
// enables sample-precise scheduling, and its efficient management of the synths allows it to process
// several thousands in parallel, which is also required.
// It only requires to process the data as integers or floats (M is originally a list of strings),
// and to model the loop and index state as a dataflow-oriented chain of UGens, which may be less
// readable.


3+3



////////////////////////////////////////////////////////////////////////////////////////////////////
/// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////////

(
s.boot;
)
s.plotTree;

(
~cwd = "/home/afr/git/supercollider-work/wave2live/";
~durBus = Bus.control(s, 1);
~durBus.set(0.1);
)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEFINE KERNEL SYNTHS
////////////////////////////////////////////////////////////////////////////////////////////////////

(
// percussive blip
SynthDef("blip",{|amp=0.1, freq=440, dur=0.1| var sig;
	sig = SinOsc.ar(freq)*amp*EnvGen.kr(Env.perc(releaseTime:dur), doneAction:2, timeScale:In.kr(~durBus));
	Out.ar([0,1], sig);
}).add
)


Env.perc(0.05, 1, 1, -4).test.plot;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEFINE FUNCTIONS TO LOAD .TXT FILE TO LIST, AND ADAPT LIST TO SCORE PLAYABLE BY KERNEL
////////////////////////////////////////////////////////////////////////////////////////////////////

(

// import txt to list of [d,k,m] lists, sorted in ascending d. Usage example:
// y=~parseList.(~cwd++"test_list.txt")
// y.collect{|elt| elt*2}
~parseList = {|absolutePath|
	f = File(absolutePath, "r");
	x = f.readAllString;
	f.close;
	x = x.split($\n).select({|line| " *".matchRegexp(line)}); // split by non-empty lines
	x = x.collect{|line| line.split($ ).select({|elt| elt!=""})}; // split by non-empty elements
	x = x.collect{|l| [l[0].asInt, l[1].asFloat, l[2].stripWhiteSpace]}; // cast elements to numbers
	x.sort{|curr,nxt| curr[0]<nxt[0]} // sort by first element
};

~list2score = {|lst, synthName, mapMfreq| var seqlst, scr;
	seqlst = lst.collect{|elt| [elt[0]/s.sampleRate, [\s_new, synthName, s.nextNodeID,0,0,
		"amp", elt[1], "freq", mapMfreq.(elt[2])]]}; //.add([~maxSample/44100, [\c_set,0,0]]);
	seqlst.postln;
	scr = Score.new(seqlst);
};

)


////////////////////////////////////////////////////////////////////////////////////////////////////
/// LOAD LIST TO SCORE
////////////////////////////////////////////////////////////////////////////////////////////////////


(
// load txt to list, and convert list to score
~list = ~parseList.(~cwd++"despacito.txt");
// play or stop score with .play and .stop methods
~score = ~list2score.(~list, "blip", {|x| x.asFloat.midicps});
)




////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIDI CONFIG
////////////////////////////////////////////////////////////////////////////////////////////////////

(

MIDIClient.init;
MIDIIn.connectAll;
~midiDispatcher = {|widget, value|
	var val = value/127;
	switch(widget,
		/*
		0,  { // Slider1
			postln("Slider1: "++val)
		},
		64, { // R1
			postln("R1: "++val)
		},
		48, { // M1
			postln("M1: "++val)
		},
		32, { // S1
			postln("S1: "++val) },
		*/
		16, { // Knob1
			postln("Knob1: "++val);
			~durBus.set(val*10+0.01)},
		41, { // Play
			postln("Play: "++val);
			~score.play},
		42, { // Stop
			postln("Stop: "++val);
			~score.stop},
		postln("midiDispatcher: not configured "++[widget,value]);
	);
};
~midiController = MIDIFunc.cc({|val, num, chan, src| ~midiDispatcher.(num, val)}, (0..127));
)

~midiController.free;


~score.play
~score.stop
















////////////////////////////////////////////////////////////////////////////////////////////////////
/// SANDBOX
////////////////////////////////////////////////////////////////////////////////////////////////////





(
// a SynthDef
SynthDef(\test, { | out, freq = 440, amp = 0.1, nharms = 10, pan = 0, gate = 1 |
    var audio = Blip.ar(freq, nharms, amp);
    var env = Linen.kr(gate, doneAction: 2);
    OffsetOut.ar(out, Pan2.ar(audio, pan, env) );
}).add;
)

(
Pbind(\instrument, \test,
	\freq, Pseq(~l1, 1),
	\amp,   Pseq(~l2, 1),
	\dur, 0.1).play;
)

Pseq



f = {Out.ar(0, BPF.ar(in: In.ar(55), freq: MouseY.kr(1000, 5000), rq: 0.1))}.play;
n = {Out.ar(55, WhiteNoise.ar(0.5))}.play;

r = {Out.ar(0, FreeVerb.ar(In.ar(55, 2), mix: 0.5, room: 0.9, mul: 0.4))}.play;
// Now run this second ('feed the busy tone into the reverb bus')
a = {Out.ar(55, SinOsc.ar([800, 880], mul: LFPulse.ar(2)))}.play;
a.free;









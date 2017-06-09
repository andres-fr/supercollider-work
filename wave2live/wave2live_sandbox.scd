
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







(
s.boot;
)


////////////////////////////////////////////////////////////////////////////////////////////////////
/// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////////
(
~cwd = "/home/afr/git/supercollider-work/wave2live/";
~dur = 0.1;
)







////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. LOAD THE LIST IN THE .TXT FILE INTO A SCORE THAT CAN BE SAVED AND LOADED FROM DISK
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

~list2score = {|lst, synthName, mapMfreq|
	var seqlst = lst.collect{|elt| [elt[0]/s.sampleRate, [\s_new, synthName, s.nextNodeID,0,0,
		"amp", elt[1], "freq", mapMfreq.(elt[2])]]}; //.add([~maxSample/44100, [\c_set,0,0]]);
	seqlst.postln;
	Score.new(seqlst);
};

)


(//MAIN
~list = ~parseList.(~cwd++"test_list.txt");
~maxSample = ~list.last[0];
~maxIdx = ~list.size;
~score = ~list2score.(~list, "blip", {|x| x.split($.)[0].split($-)[1].asInt.midicps});
~score.play
)


//MAIN
~list = ~parseList.(~cwd++"despacito.txt");
~maxSample = ~list.last[0];
~maxIdx = ~list.size;
~score = ~list2score.(~list, "blip", {|x| x.asFloat.midicps});
~score.play





// write a sample file for testing
(
var f, g;
TempoClock.default.tempo = 1;
g = [
    [0.1, [\s_new, \helpscore, 1000, 0, 0, \freq, 440]],
    [0.2, [\s_new, \helpscore, 1001, 0, 0, \freq, 660],
        [\s_new, \helpscore, 1002, 0, 0, \freq, 880]],
    [0.3, [\s_new, \helpscore, 1003, 0, 0, \freq, 220]],
    [1, [\c_set, 0, 0]] // finish
    ];
f = File("score-test","w");
f.write(scr.asCompileString);
f.close;
z = Score.newFromFile("score-test");






(
~list = ~parseList.(~cwd++"test_list.txt");
~maxSample = ~list.last[0];
~maxIdx = ~list.size;
//~bufs = ~loadListToBufs.(~list, {|s| var k; k=s.split($.)[0].split($-)[1].asInt.midicps});
~playList = {|lst|
	var seq = ~list.collect{|elt| [elt[0]/s.sampleRate, [\s_new, "blip", s.nextNodeID,0,0,
		"amp", 0.1* elt[1], "freq", elt[2].split($.)[0].split($-)[1].asInt.midicps,
		"dur", 2]]};//.add([~maxSample/44100, [\c_set,0,0]]);
	Score.new(seq).play};

~playList.(~list)

)




////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2. DEFINE THE SYNTH: IT SHOULD ACCEPT AT LEAST A NORMALIZATION AND AN ID/FREQUENCY PARAMETER
////////////////////////////////////////////////////////////////////////////////////////////////////

(
// percussive blip
SynthDef("blip",{|amp=0.1, freq=440, dur=0.1| var sig;
	sig = SinOsc.ar(freq)*amp*EnvGen.kr(Env.perc(releaseTime:dur), doneAction:2);
	Out.ar(0, sig);
}).add
)
















////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. DEFINE THE SEQUENCER:
////////////////////////////////////////////////////////////////////////////////////////////////////

(

// helper function for UGen comparison
~is    = { arg a, b; BinaryOpUGen('==', a, b) };
~isNot = { arg a, b; BinaryOpUGen('!=', a, b) };

SynthDef("sequencer",{|dbuf, kbuf, mbuf| var sample, trig, sig;
	sample = Integrator.ar(DC.ar(1));
	trig = ~is.(sample.wrap(0,44100), 0);
	//sig = SinOsc.ar(440)*0.5*EnvGen.ar(Env.perc(releaseTime:~dur), gate:trig, doneAction:2);
	sig = Synth.new("blip");
	Out.ar(0, sig);
	// for debugging
	//values     = [startFrame, phaseK];
	SendReply.ar(trig, cmdName: '/test', values: sample.wrap(0, 44100));
	//sample.wrap(0,44100).poll(label: "send");
}).add
)


Synth.new("sequencer", ["dbuf", 0, "kbuf", 0, "mbuf", 0])
w = Synth.new("blip", ["freq", 440, "dur", 0.1, "amp", 1])


(
~listen = OSCFunc(path: "test", func: {|msg| msg.postln});
)

~listen.free;


Pfsm

Routine

play{Impulse.ar(2)!2}


PauseSelf

Quant

Pseries

PProto






////////////////////////////////////////////////////////////////////////////////////////////////////
/// RUN!
////////////////////////////////////////////////////////////////////////////////////////////////////
(

~list = ~parseList.(~cwd++"test_list.txt");
~maxSample = ~list.last[0];
~maxIdx = ~list.size;
~bufs = ~loadListToBufs.(~list, {|s| var k; k=s.split($.)[0].split($-)[1].asInt.midicps});

)


{ Integrator.ar(LFPulse.ar(300, 0.2, 0.1), MouseX.kr(0.001, 0.999, 1)) }.play



// ~bufs[2].getToFloatArray(action: {|x| x.postln})

// { Stepper.kr(someTrigger??, 0,0,~maxIdx,1) }.plot(5,minval:0,maxval:100);
(
{
    var a;
    a = Line.ar(-2, 2, 0.01);
	//a.wrap2
}.plot
)

{Line.ar}.plot

{SinOsc.ar()}.plot

{ FSinOsc.ar(1000).wrap2(Line.kr(0,1.01,8)) }.scope;




////////////////////////////////////////////////////////////////////////////////////////////////////
/// SANDBOX
////////////////////////////////////////////////////////////////////////////////////////////////////


// https://composerprogrammer.com/teaching/supercollider/sctutorial/3.4%20Server-side%20Sequencing%20and%20Triggers.html


PulseCount // counts triggers: can be useful to model "idx"


(// plays a given buffer with delay and normalization
SynthDef("f_function",{ |buf, bufdur, delay=0, mul=1, out=0|
	var sig = PlayBuf.ar(1, buf);
	sig = sig*Line.kr(1,1, delay+bufdur, doneAction:2);
	sig = DelayN.ar(sig, delay, delay, mul);
	Out.ar(out, sig);
}).add;
)

(// function for playing buffers in ~audio with delay+normalization
 // ~playBuffer.value("anvil", 0, 0.5, 0)
~playBuffer = {|audio_name, delay, mul, transp=nil| var tmp;
	audio_name = audio_name++if((transp==nil) || (transp==0), {""},{"["++transp++"]"});
	tmp = ~audios.at(audio_name);
	if (tmp!=nil,
		{Synth("f_function", [\buf, tmp.at("buf"), \bufdur, tmp.at("dur"),
			\delay, delay, \mul, mul])},
		{postln("audio not found!: "++audio_name)})};
)


















// w = Synth.new("blip", ["freq", 440, "dur", 1, "amp", -0.1])
// w.free

b = s.makeBundle(1, {Synth.new("blip", ["dur", 0.5, "freq", 1000.rand])}, b);


b=nil
s.makeBundle(nil, {}, b)

//1)
s.sendBundle(1, [\s_new, "blip", s.nextNodeID]);
// 2)
k = Synth.basicNew("blip", s)
s.sendBundle(1, k.newMsg());
// 3)
s.makeBundle(1, {Synth("blip", s);})


Score


















// load list of [d,k,m] lists to three separate D,K,M server-side buffers. Since m is a list of str,
// and the server admits numbers only, a mapMlist function has to be provided that performs the
// mapping from strings to numbers. Usage example:
// ~bufs = ~loadListToBufs.(~parseList.(~cwd++"test_list.txt"),
//                          {|s| var k; k=s.split($.)[0].split($-)[1].asInt})
// recover a buffer to the client: ~bufs[2].getToFloatArray(action: {|x| x.postln})
~loadListToBufs = {|list, mapMlist| var len, dbuf, kbuf, mbuf;
	len = list.size;
	// allocate buffers
	dbuf = Buffer.alloc(s, len, 1);
	kbuf = Buffer.alloc(s, len, 1);
	mbuf = Buffer.alloc(s, len, 1);
	// fill buffers
	dbuf.loadCollection(list.collect{|elt| elt[0]}, action:{"loaded D!".postln});
	kbuf.loadCollection(list.collect{|elt| elt[1]}, action:{"loaded K!".postln});
	mbuf.loadCollection(list.collect{|elt| mapMlist.(elt[2])}, action:{"loaded M!".postln});
	[dbuf,kbuf, mbuf]
};

)



(
~list = ~parseList.(~cwd++"test_list.txt");
~maxSample = ~list.last[0];
~maxIdx = ~list.size;
//~bufs = ~loadListToBufs.(~list, {|s| var k; k=s.split($.)[0].split($-)[1].asInt.midicps});
~playList = {|lst|
	var seq = ~list.collect{|elt| [elt[0]/s.sampleRate, [\s_new, "blip", s.nextNodeID,0,0,
		"amp", 0.1* elt[1], "freq", elt[2].split($.)[0].split($-)[1].asInt.midicps,
		"dur", 2]]};//.add([~maxSample/44100, [\c_set,0,0]]);
	Score.new(seq).play};

~playList.(~list)

)

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
1)


*/


////////////////////////////////////////////////////////////////////////////////////////////////////
/// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////////

(
s.options.maxNodes_(8192*2);
s.options.memSize_(2*1024*1024); // in KB, so this is 2GB
s.reboot;
)

s.volume.gui;
s.plotTree;


(
~cwd = "/home/afr/git/supercollider-work/wave2live/";
~w2wPath = ~cwd++"w3w";
~durBus = Bus.control(s, 1);
~durBus.set(0.1);
)


////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEFINE KERNEL SYNTHS
////////////////////////////////////////////////////////////////////////////////////////////////////

(
// percussive blip
// Env.perc(0.05, 1, 1, -4).test.plot;
SynthDef("blip",{|amp=0.1, freq=440, dur=0.1| var sig;
	sig = SinOsc.ar(freq)*amp*EnvGen.kr(Env.perc(releaseTime:dur), doneAction:2,
		timeScale:In.kr(~durBus));
	Out.ar([0,1], sig);
}).add
)




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
		*/
		7, { // Slider8
			val = val*34-30;
			postln("s.volume="++val);
			s.volume = val;
		},
		16, { // Knob1
			val = val*10+0.1;
			postln("~durBus="++val);
			~durBus.set(val)},
		41, { // Play
			postln("Play: "++val);
			~score.play},
		42, { // Stop
			postln("Stop: "++val);
			~score.stop},
		{postln("midiDispatcher: not configured "++[widget,value])};
	);
};
~midiController = MIDIFunc.cc({|val, num, chan, src| ~midiDispatcher.(num, val)}, (0..127));
)

~midiController.free;


~score.play
~score.stop

~score.endTime







/////////////////////////////////////////////////////////////////////////////////////////////////////
/// INTERACTION WITH WAV2WAV
/////////////////////////////////////////////////////////////////////////////////////////////////////


( // calls w2w
~w2w = {|orig, materials, iters, ratio, callbackFnResPID, verbosity=50 |
	var flags, result, cmd, p_id;
	flags = " -s "++orig++" -m "++materials++" -i "++iters++" -r "++ratio++" -f "++verbosity;
	cmd = ~w2wPath++flags;
	cmd.postln;
	cmd.unixCmd{|res, pid| callbackFnResPID.(res,pid)};
};
)
//~w2w.(~cwd++"audios/aperghis.wav", ~cwd++"audios/marimba/", 500, 40,{|res,pid| postln(res)});






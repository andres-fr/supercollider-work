/////////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
1) continue expanding the chi2 kernel family: try to clone some DX7 instrument with it.



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
~durBus.set(1);
~envBus = Bus.control(s, 1);
~envBus.set(0);
)



////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEFINE CHI2 FUNCTION GENERATORS
////////////////////////////////////////////////////////////////////////////////////////////////////


(
// Use Octave to pick an array of values for the gamma function. Usage example:
// see ~chi2Fn
~gammaFn = {|...vals| var cmd, result;
	if(vals.isEmpty, nil,
		cmd = "octave -q --eval 'gamma("++vals++")'";
		result = cmd.unixCmdGetStdOut.split($=)[1].split($ ).select({|elt|
			"[0-9]+".matchRegexp(elt)}).asFloat;
		result;
	)
};

// this function calculates the chi-2 PDF for "df" degrees of freedom
// in the given range. To calculate the "integral", the range step has
// to be taken into account as the differential. Usage example:
// ~chi2Fn.(4.56, (0.000001, 0.100001 .. 100)).plot
// (2, 3 .. 15).collect{|k| ~chi2Fn.(k, (0.000001, 0.010001 .. 50))}.plot;
~chi2Fn = {|df, range| var r, numerator, denominator;
	r = df/2;
	numerator = {|x| (x**(r-1))*exp(-0.5*x)};
	denominator = ~gammaFn.(r) * (2**r);
	range.collect{|x| numerator.(x)} / denominator;
};

// this function can be used to generate a hi-resolution 1-second envelope that
// follows the chi2 distribution, which may be used to generate analyisis signals.
// IMPORTANT: the df index should lie between 1 and 60, where 1 is approx. an
// exponential distribution and 60 a gaussian. Also, it isn't fast enough
// for real-time generation with granular synths, and the resulting envelope
// is a very big object. So for real-time synthesis, it is recommended to
// use ~chiEnvOptimizedPairs (see below) to generate an ahead-of-time collection
// of avaliable envelopes. Usage example:
// ~envelopes = [2,3,4,5,7,9,12,18, 25, 35, 45,60].collect{|k| ~chiEnv.(k)};
// ~envelopes.plot;
// {SinOsc.ar()*EnvGen.ar(~envelopes.choose, doneAction:2)}.play // envs are too big!
~chiEnvPairs = {|df| var topdf, topidx, maxSeconds, epsilon, step, range;
	topidx = 120;
	topdf = if(df>(topidx/2), (topidx/2), df);
	maxSeconds = 1;
	epsilon = 0.000001;
	step = 0.01;
	range = (epsilon, step+epsilon .. topidx);
	Array.series(size:range.size, start:0, step:(1/(range.size))) +++~chi2Fn.(topdf, range);
};

// This takes longer to generate, but the resulting env is much more sparse, which helps
// the server and has no audible impact in the result.
// The idea is that the parts with higher values contain more information and deserve
// more resolution than the lower parts. So this function generates the high-resolution
// distribution, and then takes random samples from it until the desired domain size
// is achieved. Then the function is recalculated for the sparse domain. Important:
// wchoose requires that pdf.sum=1, so the values have to be multiplied by the
// step size of pdf'range. Usage example:
// ~envelopes = [2,3,4,5,7,9,12,18, 25, 35, 45,60].collect{|k| ~chiEnvOptimized.(k)};
// ~envelopes.plot;
// {SinOsc.ar()*EnvGen.ar(~envelopes.choose, doneAction:2)}.play
~chiEnvOptimizedPairs = {|df, sparseSize=50|
	var topidx, topdf, maxSeconds, epsilon, step, range, pdf, sparse;
	topidx = 120;
	topdf = if(df>(topidx/2), (topidx/2), df);
	maxSeconds = 1;
	// calculate the hi-resolution pdf
	epsilon = 0.000001;
	step = 0.01;
	range = (epsilon, step+epsilon .. topidx);
	pdf = ~chi2Fn.(topdf, range)*step;
	// fill sparse domain with a set comprehension:
	sparse = Set[epsilon, range.last];
	while({sparse.size<sparseSize}, {
		sparse.add(range.wchoose(pdf));
	});
	// sort sparse domain and generate new pairs with it, to feed Env.pairs:
	sparse = sparse.as(Array).sort;
	(sparse*maxSeconds/(sparse.last))+++~chi2Fn.(topdf, sparse);
};


// Usage example:
// ~saveChiOptimizedPairsToBinary.((2.5, 2.6 .. 60),
//                                  ~cwd++"supercollider_envOptPairsArray_2.5_to_60_by_0.1.bin")
~saveChiOptimizedPairsToBinary = {|indexes, outPath| var pairs;
	pairs = indexes.collect{|k| ~chiEnvOptimizedPairs.(k, 300)};
	pairs.writeBinaryArchive(outPath);
};


// For this function to work, the pairs have to be calculated and saved to binary file at least once,
// as follows:
//   ~envOptPairs = (2.5, 2.6 .. 60).collect{|k| ~chiEnvOptimizedPairs.(k, 300)};
//   ~envOptPairs.writeBinaryArchive(~cwd++"supercollider_envOptPairsArray_2.5_to_60_by_0.1.bin")
// and then can be called like this:
// ~envelopes = ~loadSomeChiEnvelopes.()
~loadSomeChiEnvelopes={var pairs, idxs,envs, envgens;
	pairs = Array.readBinaryArchive(~cwd++"supercollider_envOptPairsArray_2.5_to_60_by_0.1.bin");
	// 50 elements between 1 and 500
	idxs = Array.geom(size: 100, start: 1, grow: 1.1).asInt.reject{|x| x>500}.as(Set).as(Array).sort;
	idxs.collect{|i| Env.pairs(pairs[i], \cub)};
};
)



////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEFINE KERNEL SYNTHS
////////////////////////////////////////////////////////////////////////////////////////////////////

(
// a sinusoidal with a percussive envelope
// Env.perc(0.05, 1, 1, -4).test.plot;
SynthDef("blip",{|amp=0.1, freq=440| var sig;
	sig = SinOsc.ar(freq)*amp*EnvGen.kr(Env.perc, doneAction:2,
		timeScale:In.kr(~durBus));
	Out.ar([0,1], sig);
}).add;
)

(

// a sinusoidal with chi2 envelope, capable of generating morlet wavelets. Usage examples:
// (
// isolated tones
// Synth("chi2", ["amp", 0.5, "freq", 440, "env", ~envelopes[40]])
// example with patterns
// Pbind(\instrument, "chi2",
// 	\freq, Prand(Array.geom(size:120, start: 100, grow:1.0212366), inf),
// 	\dur, Pfunc({exprand(0.5, 0.01)}),
// 	\env, Pfunc({~envelopes[50.rand]})).play;
// )
~envelopes = ~loadSomeChiEnvelopes.();
SynthDef("chi2", {|amp=1, freq=440| var sig, env, envctl;
	env = ~envelopes.[0];
	envctl = \env.kr(env.asArray);
	sig = SinOsc.ar(freq)*amp*EnvGen.kr(envctl, timeScale:In.kr(~durBus), doneAction:2);
	Out.ar([0,1], sig);
}).add;
)











////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEFINE FUNCTIONS TO LOAD .TXT FILE TO LIST, AND ADAPT LIST TO SCORE PLAYABLE BY KERNEL SYNTH
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


~scoreChi = ~list2score.(~list, "chi2", {|x| x.asFloat.midicps});

~chi2synth = {|amp, freq, envIdx|
	Synth("chi2", ["amp", amp, "freq", freq, "envIdx", ~envelopes[envIdx]])
};

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
		0,  { // Slider1
			val = (val*500).asInt;
			postln("~envBus: "++val);
			~envBus.set(val)},
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






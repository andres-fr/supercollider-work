(//server config and start
Server.default = s = Server.internal;
s.options.memSize = 1500*1024;// up to 1500MB RAM allocable
s.reboot;
//s.volume.gui;
//s.plotTree;
//s.meter;
//s.quit;
)


///////////////////////////////////////////////////////////////////////////////////
/// TODO
///////////////////////////////////////////////////////////////////////////////////


BUG: the NRT transposer adds a delay to every audio file (probably equal to the FFT-window size).
     a good strategy to correct that may be to use another .ar UGEN to detect the beginning of sound

BUG: PlaySig casts sequences like ["anvil", 0, 1, 0] to -> "anvil[0]" which doesnt exist... decide
     what to do with untransposed files!!

BUG?? crossCorrelation outputs peak 1 sample sooner??



// signal normalization criterium: once known where to
// put a sample, the right intensity has to be calculated

// once done that, build a stereophonic setup with a hard-coded loop to test
// a few material placements, the result should make sense. If not, debug!!

// if the result does make sense, it is the time for the freq. heuristic:
//  a) let the loop run for a big number of iterations, and get an estimate of the maximum tolerated
//  b) test some heuristics within the maximum and analyze the CC-curves to measure their performance
//  c) take the best heuristic and let it run for longer than usual!!

// if the results are satisfactory enough, phase1 is done. If not, maybe time to
// optimize or try more complex stuff (see txt file)









///////////////////////////////////////////////////////////////////////////////////
/// GENERAL CONFIG AND DATA MODEL
///////////////////////////////////////////////////////////////////////////////////
( // addresses
~working_dir = File.getcwd++"/git/supercollider-work/correlator/";
~materials_dir = ~working_dir++"materials/";
// this implies that everything in the "materials_dir" is an audio file to be loaded
~get_materialdir_contents = {PathName.new(~materials_dir).entries.collect({|x| x.fullPath});};
~get_materialdir_names = {PathName.new(~materials_dir).entries.collect({|x| x.fileNameWithoutExtension});};
~material_addresses = ~get_materialdir_contents.value;
)


(
// THE GLOBAL DATASTRUCTURE USED TO STORE EVERYTHING
// this section would be MUCH MORE meaningful as class definition, but hey, sclang
~audios = Dictionary.new;

// systematic function to fill the ~audios dict
~loadAudioData = {|fullPath| var d, buf;
	// create dictionary as d
	d = Dictionary.new;
	// add buffer, duration in seconds and signal (NON-PADDED TO SAVE MEMORY) to d
	buf = Buffer.read(s, fullPath, action:{|b|
		d.put("buf", b);
		d.put("dur", b.duration);
		buf.loadToFloatArray(action: {|arr| d.put("sig", arr.as(Signal))});
	});
	// add d to the global dict
	~audios.put(PathName.new(fullPath).fileNameWithoutExtension, d);
};

// load everything in the "materials" folder
~material_addresses.do({|x| ~loadAudioData.value(x)});

// test everything went fine
//~audios.at("anvil").at("buf").play
)










///////////////////////////////////////////////////////////////////////////////////
/// BUFFERS: THE SERVER SIDE
///////////////////////////////////////////////////////////////////////////////////
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

(// define the sequence player
~seq_player = {|seq| seq.do({|x| ~playBuffer.value(x[0], x[1], x[2], x[3])})}
)

(// TEST: create and play a sequence
~f_sequence = [];
~f_sequence = ~f_sequence.add(["anvil", 0, 1, 0]);
//upwards
~f_sequence = ~f_sequence.add(["anvil", 1, 1, 100]);
~f_sequence = ~f_sequence.add(["anvil", 2, 1, 200]);
~f_sequence = ~f_sequence.add(["anvil", 3, 1, 300]);
//downwards
~f_sequence = ~f_sequence.add(["anvil", 1.3, 1, -100]);
~f_sequence = ~f_sequence.add(["anvil", 2.2, 1, -200]);
~f_sequence = ~f_sequence.add(["anvil", 3.1, 1, -300]);
// play!
~seq_player.value(~f_sequence)
)







///////////////////////////////////////////////////////////////////////////////////
/// FILE MANAGEMENT: CREATE TRANSPOSED AUDIO FILES
///////////////////////////////////////////////////////////////////////////////////


(// this function takes a path to a soundfile, transposes it with PV_Binshift and saves it
 // to a .wav file in the given path. if a callback_fn is passed, will be executed after. Example:
 // ~transposeFileNRT.value(~materials.at(0).path, -300, "home/whatever/asdf.wav", {~playBufFromPath.value(~addNrToPath.value(~materials.at(0).path,-300)++".wav")})
// WARNING: do not use this function, use ~createTransposedAudio instead! this doesn't
// check if the file already exists, nor adapts the resultpath, or includes the result
// into the ~audios dictionary... in OOP this wouldn't be public
~transposeFileNRT = {|addr, transp, resultpath, callback_fn=nil|
	fork {
		var sf, dur, frames, channels, samplerate, oscpath, score, resultbuf, cond;
		// 1) RESOLVE FILENAMES AND SUCH
		sf = SoundFile.openRead(addr);
		dur = sf.duration;
		frames = sf.numFrames;
		channels = sf.numChannels;
		samplerate = sf.sampleRate;
		sf.close;
		oscpath = PathName.tmp +/+ UniqueID.next ++ ".osc";
		resultbuf = Buffer.new(s, frames, channels);
		// 2) DEFINE SCORE
		score = Score([
			[0, resultbuf.allocMsg],
			[0, [\d_recv, SynthDef(\transpose_buffer, {
				var sig = SoundIn.ar(0); // will come from NRT input file
				sig = FFT(LocalBuf(2**13), sig, wintype:1, winsize:(2**20));
				sig = PV_BinShift(sig, 1, transp, 1);
				sig = IFFT(sig)*2;
				BufWr.ar(sig, resultbuf.bufnum, Phasor.ar(0, BufRateScale.kr(resultbuf), 0, BufFrames.kr(resultbuf)));
			}).asBytes;]],
			[0, Synth.basicNew(\transpose_buffer, s, 1000).newMsg],
			[dur, resultbuf.writeMsg(resultpath, headerFormat: "WAV", sampleFormat: "float")]
		]);
		// 3) EXECUTE SCORE
		cond = Condition.new;
		// osc file path, output path, input path - input is soundfile to analyze
		score.recordNRT(oscpath, "/dev/null", addr, sampleRate: sf.sampleRate,
			options: ServerOptions.new
			.verbosity_(-1)
			.numInputBusChannels_(channels)
			.numOutputBusChannels_(channels)
			.sampleRate_(samplerate),
			action: { cond.unhang }  // this re-awakens the process after NRT is finished
		);
		cond.hang;  // wait for completion
		// 3) CALLBACK REGION AFTER NRT FINISHED
		File.delete(oscpath);
		resultbuf.free;
		//postln("saved transposition to "++resultpath);
		callback_fn.value;
	};
};
)




(// function that uses ~transposeFileNRT: checks for name consistency,
 // and creates the transposed audio adding it ASYNCHRONOUSLY to ~audios at the end.
// ~createTransposedAudio.value("anvil", -100);
// ~audios.at("anvil[-100]").at("buf").play
~createTransposedAudio = {|audio_name, transp| var bufpath, newname, newpath;
	// check if transp is zero: in that case do nothing
	if(transp==0,{postln("zero transposition: do nothing")},{
		// check if audio_name exists! otherwise plot a warning (PathName will crash)
		~audios.atFail(audio_name, {println("unexisting audio: "++audio_name)});
		// now check if name[transp] already exists
		bufpath = PathName.new(~audios.at(audio_name).at("buf").path);
		newname = bufpath.fileNameWithoutExtension++"["++transp++"]";
		if(~get_materialdir_names.value.detect({|x| x==newname})!=nil,
			// if exists, do nothing
			{postln("material "++newname++" already exists! do nothing")},
			{// if everything went as expected:
				newpath =bufpath.pathOnly++newname++".wav";
				~transposeFileNRT.value(bufpath.asAbsolutePath, transp, newpath, {
					("created and added "++newname++" to ~audios!").postln;
					~loadAudioData.value(newpath)
				});
		})
	})
};
)


( // TEST NRT TRANSPOSITION, BATCH TRANSPOSE SOME SAMPLES:

/*var addr = ~materials.at(5).path;
var transp = -345;
~transposeFileNRT.value(addr, transp, {~playBufFromPath.value(~addNrToPath.value(addr, transp)++".wav")})*/

var m = ["anvil", "flexatone", "triangle", "bowsuspend", "crotales", "recoreco", "child"];
[-400, -300, -200, -100, 100, 200, 300, 400].do({|freq|
	m.do({|name|
		~createTransposedAudio.value(name, freq)
	})
});
)






///////////////////////////////////////////////////////////////////////////////////
/// SIGNALS: THE CLIENT SIDE
///////////////////////////////////////////////////////////////////////////////////

(// some basic functions
~get2powSize = {|n, extra_padded=true| 2**(n.log2.roundUp)*if(extra_padded,2,1)};
~zeroPadSignal = {|sig, extra_padded=true|
	Signal.newClear(~get2powSize.value(sig.size, extra_padded)).overDub(sig)};

// "efficient" way to play any signal: ~playSig.value(~audios.at("anvil").at("sig"))
~playSig ={|sig| Buffer.loadCollection(s, sig, action:{
	|buf| {PlayBuf.ar(buf.numChannels, buf, doneAction:2)}.play})};
)

(// FFT-transpose a signal (naive, dont use!)
// ~playSig.value(~transposeSignalFFT.value(~audios.at("anvil").at("sig"), 1024))
~rotateList = {|lst, n| (lst++(0!abs(n))).rotate(n)[0..lst.size-1]};
~transposeSignalFFT = {|sig, n| var imag, cosTable, complex, resynth;
	sig = ~zeroPadSignal.value(sig);
	imag = Signal.newClear(sig.size); // zeros
	cosTable = Signal.fftCosTable(sig.size); // required
	complex = fft(sig, imag, cosTable); // calculated complex fft
	// here should happen the PV
	complex.real = ~rotateList.value(complex.real, n).as(Signal);
	complex.imag = ~rotateList.value(complex.imag, n).as(Signal);
	//
	resynth = complex.real.ifft(complex.imag, cosTable);
	resynth.real
}
)



( // receives a sequence of the form [[mat_name,second,mult,transp], ...] and
  // returns the result of combining the corresponding signals
  // ~playSig.value(~render_sequence.value([["anvil", 0, 1, 300], ["anvil", 1, 0.05, -300]]))
~render_sequence = {|seq| var final_sig, last_elt, total_dur, tmp_sig, sample_rate;
	sample_rate = s.sampleRate;
	last_elt = seq.last; // just in case last is O(n), to avoid duplicity
	total_dur = (~audios.at(last_elt.at(0)).at("dur"))+(last_elt.at(1));
	final_sig = Signal.newClear(total_dur*sample_rate.roundUp);
	total_dur.postln;
	seq.do({|x|
		tmp_sig = x.at(0)++"["++x.at(3)++"]"; // get transp.name
		tmp_sig = ~audios.at(tmp_sig).at("sig"); // get signal
		tmp_sig = tmp_sig * x.at(2);  // normalize signal
		final_sig.overDub(tmp_sig, (x.at(1)*sample_rate).asInt)
	});
	final_sig;
};
)



(// ~seq_player_sig.value([["anvil", 0, 1, 300], ["anvil", 1, 0.05, -300]]);
~seq_player_sig = {|seq| ~playSig.value(~render_sequence.value(seq))};
)


(// FFT-cross-correlation between 2 signals:
 // At each sample t, the output corresponds with sig2 bein fixed, and
 // sig1 being delayed t samples. Therefore, the position of the peak
 // value corresponds to the delay that sig2 presented against sig1
 // The following example illustrates this, it should show a peak at
 // sample 123, because b is simply a with 123 samples delay:
 // a = Signal.fill(1000, { rrand(-1.0, 1.0)});
 // //a = Signal.sineFill(1000,[1],[0]); // this is another option
 // b = (0!123++a).as(Signal);
 // c = ~crossCorrelation.value(a,b,true);
 // c.integral
 // c.maxIndex   // WARNING: THIS RETURNS 122, ONE SAMPLE MUST BE ADDED??
~crossCorrelation = {|sig1, sig2, plot=false, extrapad=true| var imag, cosTable, ff1,ff2, cc, outsize;
	sig1 = ~zeroPadSignal.value(sig1, extrapad);
	sig2 = ~zeroPadSignal.value(sig2, extrapad);
	imag = Signal.newClear(max(sig1.size, sig2.size)); // zeros
	cosTable = Signal.fftCosTable(imag.size); // required
	if(sig1.size!=imag.size,{sig1 = imag.as(Signal).overDub(sig1)});
	if(sig2.size!=imag.size,{sig2 = imag.as(Signal).overDub(sig2)});
	// reverse one, and fft'em
	ff1 = fft(sig1.reverse, imag, cosTable);
	ff2 = fft(sig2, imag, cosTable);
	// elementwise multiply
	cc = ff1*ff2;
	// IFFT and plot
	outsize = (imag.size/2).asInt;
	cc = cc.real.ifft(cc.imag, cosTable);
	cc = cc.real[0..outsize];
	if(plot, {[sig1.reverse[0..outsize], sig2[0..outsize],
		cc].flop.flat.plot("cross-correlation", Rect(0, 0, 500, 500), numChannels:3);});
	//return value
	cc
})


// another CC test
/* ~crossCorrelation.value(~audios.at("anvil").at("sig"),
	((~audios.at("anvil[100]").at("sig")!5).flat).as(Signal),true, extrapad:true);
*/








///////////////////////////////////////////////////////////////////////////////////
/// THE ALGORITHM: HEURISTICS, PROTOCOLS, OPTIMIZATIONS...
///////////////////////////////////////////////////////////////////////////////////



( var original, result, materials, f_seq, terminate_condition, i;

// 1) the original signal, the sequence that the algo constructs, and the allowed materials
original = ~audios.at("child").at("sig"); // child is the signal to reconstruct
result = Signal.newClear(original.size);
materials = ~audios.keys.reject({|x| x.contains("child")}); // child not allowed as material
f_seq = [];

// 2) the convergence criterium: signal
terminate_condition = {|i| i>10}; // abs(current_global_cc - CC(S, sum(f_list))) < EPSILON


//2) the algo generate
i = 0;
while ({i<10},{

	// interpreter crashes when calculating a long CC...
	i = i+1;
});


)


//
// (
// ~crossCorrelation.value(~audios.at("anvil").at("sig"),
// 	~audios.at("child").at("sig"),
// true, extrapad:true);
// )








(//server config and start
Server.default = s = Server.internal;
s.options.memSize = 200*2014;// 200MB RAM allocable
s.reboot;
//s.volume.gui;
//s.plotTree;
//s.meter;
//s.quit;
)


///////////////////////////////////////////////////////////////////////////////////
/// TODO
///////////////////////////////////////////////////////////////////////////////////

substitute every appearance of roundPow2 with zeroPadSignal

basic DS: change the Array-based storage for a Dictionary based one:
  -each material filename should be a key, and the value is another Dictionary,
   in which the keys are transp. integers and the values are the corresponding BUFFERS
  -the original files are with a zero, each NRT created file should be added as well to the dictionary
  -add support to the NRT module: ignore files if they exist or if transp=0
  -Signals: useful for correlation only! think about the optimal storage/algor. politics


// After that, it comes the normalization criterium: once known where to
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
/// BUFFERS: THE SERVER SIDE
///////////////////////////////////////////////////////////////////////////////////

( // addresses
~working_dir = File.getcwd++"/supercollider-work/";
~materials_dir = ~working_dir++"materials/";
~material_addresses = ["crotales.wav", "flexatone.wav", "recoreco.wav", "triangle.wav", "bowsuspend.wav", "anvil.wav"];

)

(// load the audios into buffers (test: ~materials.at(5).play)
~material_durations = [];
~materials = ~material_addresses.collect({|x| Buffer.read(s,~materials_dir++x,
	action: {|b| ~material_durations = ~material_durations.add(b.duration)}).normalize});
~original_duration = nil; // asynch. calculated in the next line
~original = Buffer.read(s, ~working_dir++"child.wav", action: {|b| ~original_duration = b.duration});
)

(// plays a given buffer with delay, normalization and FFT transp.
SynthDef("f_function",{ |material_idx=0, delay=0, mul=1, transp=0, out=0|
	var buf, bufdur, sig;
	buf = Select.kr(material_idx, ~materials); // this could be outside of the synth for optimization
	bufdur =Select.kr(material_idx, ~material_durations);// this is needed to free the synth correctly, but not optimal: a whole kr ugen per synth just for retrieving a global float, known at Synths instantiation time...
	sig = PlayBuf.ar(1, buf);
	sig = sig*Line.kr(1,1, delay+bufdur, doneAction:2);
	sig = FFT(LocalBuf(2**15), sig, wintype:1,winsize:(2**14));
	sig = PV_BinShift(sig, 1, transp, 0);
	sig = IFFT(sig)*2;
	sig = DelayN.ar(sig, delay, delay, mul);
	Out.ar(out, sig);
}).add;
)


(// define the sequence player
~seq_player = {|lst| lst.do({|x| Synth.new("f_function", [[\material_idx, \delay, \mul, \transp],x].flop.flat)})} // +++ , ++
)

(// create a sequence and add 3 elements
~f_sequence = [];
~f_sequence = ~f_sequence.add([5, 0, 1, 0]);
//upwards
~f_sequence = ~f_sequence.add([5, 1, 1, 500]);
~f_sequence = ~f_sequence.add([5, 2, 1, 1000]);
~f_sequence = ~f_sequence.add([5, 3, 1, 2000]);
//downwards
~f_sequence = ~f_sequence.add([5, 1.3, 1, -550]);
~f_sequence = ~f_sequence.add([5, 2.2, 1, -840]);
~f_sequence = ~f_sequence.add([5, 3.1, 1, -1100]);
)

(// play the sequence!
~seq_player.value(~f_sequence)
)

(
SynthDef("transpose_buffer", {|addr, transp=0| var buf, sig;
	buf = Buffer.read(s, addr);
	sig = PlayBuf.ar(1, buf, doneAction:2);
	Out.ar(0,sig);
}).add;
)







///////////////////////////////////////////////////////////////////////////////////
/// FILE MANAGEMENT: CREATE TRANSPOSED AUDIO FILES
///////////////////////////////////////////////////////////////////////////////////

(// substitutes extension for nr in square brackets and returns whole
 // test: ~addNrToPath.value(~materials.at(0).path, 200)++".wav"
~addNrToPath = {|path, nr| var tmp;
	tmp = PathName.new(path);
	tmp.pathOnly++tmp.fileNameWithoutExtension++"["++nr++"]";
};
)

(// function for testing purposes.Warning: it doesn't free the buffer!
~playBufFromPath = {|path| {PlayBuf.ar(1, Buffer.read(s,path), doneAction:2)}.play}
)


(// this function takes a path to a soundfile, transposes it with PV_Binshift and saves it
 // to a .wav file in the same folder. if a callback_fn is passed, will be executed after. Example:
 // ~transposeFileNRT.value(~materials.at(0).path, -300, {~playBufFromPath.value(~addNrToPath.value(~materials.at(0).path,-300)++".wav")})
~transposeFileNRT = {|addr, transp, callback_fn=nil|
	fork {
		var sf, dur, frames, channels, samplerate, resultpath, oscpath, score, resultbuf, cond;
		// 1) RESOLVE FILENAMES AND SUCH
		sf = SoundFile.openRead(addr);
		dur = sf.duration;
		frames = sf.numFrames;
		channels = sf.numChannels;
		samplerate = sf.sampleRate;
		sf.close;
		resultpath = ~addNrToPath.value(addr, transp)++".wav";
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
		postln("saved transposition to "++resultpath);
		callback_fn.value;
	};
};
)
/*( // TEST NRT TRANSPOSITION: check the materials folder to find the generated files
var addr = ~materials.at(5).path;
var transp = -345;
~transposeFileNRT.value(addr, transp, {~playBufFromPath.value(~addNrToPath.value(addr, transp)++".wav")})
)*/




///////////////////////////////////////////////////////////////////////////////////
/// SIGNALS: THE CLIENT SIDE
///////////////////////////////////////////////////////////////////////////////////

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



(// "efficient" way to play a signal: ~playSig.value(~material_signals.at(5))
~playSig ={|sig| Buffer.loadCollection(s, sig, action:{
	|buf| {PlayBuf.ar(buf.numChannels, buf, doneAction:2)}.play})};
)



(// FFT-transpose a signal (naive, dont use!)
~rotateList = {|lst, n| (lst++(0!abs(n))).rotate(n)[0..lst.size-1]};
~transposeSignalFFT = {|sig, n| var imag, cosTable, complex, resynth;
	n.postln;
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



(// define the sequence player for signals
~seq_player_sig = {|sampledur, lst| var sig, tmp;
	sig = Signal.newClear(sampledur);
	lst.do({|x|
		tmp = ~material_signals.at(x.at(0));
		tmp = ~transposeSignalFFT.value(tmp, x.at(3));
		sig.overDub(tmp, x.at(1)*s.sampleRate.asInt)});
	~playSig.value(sig)};
)




(// FFT-cross-correlation between 2 signals:
 // At each sample t, the output corresponds with sig2 bein fixed, and
 // sig1 being delayed t samples. Therefore, the position of the peak
 // value corresponds to the delay that sig2 presented against a.
 // The following example illustrates this, it should show a peak at
 // sample 123, because b is simply a with 123 samples delay:
 // a = Signal.fill(1000, { rrand(-1.0, 1.0)});
 // //a = Signal.sineFill(1000,[1],[0]); // this is another option
 // b = (0!123++a).as(Signal);
 // c = ~crossCorrelation.value(a,b);
 // a.plot; b.plot; c[0..1000].plot;
 // c.integral
 // c.maxIndex   // WARNING: THIS RETURNS 122, ONE SAMPLE MUST BE ADDED
~crossCorrelation = {|sig1, sig2, plot=false| var imag, cosTable, ff1,ff2, cc, outsize;
	sig1 = ~zeroPadSignal.value(sig1);
	sig2 = ~zeroPadSignal.value(sig2);
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




(//server config and start
Server.default = s = Server.internal;
s.options.memSize = 1500*1024;// up to 1500MB RAM allocable
s.reboot;
//s.quit;
)

(
//s.volume.gui;
//s.plotTree;
s.meter;
//{SinOsc.ar([440, 500,],0,[0.2,0.2])}.play
)

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// GLOBALS
/////////////////////////////////////////////////////////////////////////////////////////////////////

( // addresses
~working_dir = "/home/afr/IMPULS/WORKSPACE/"; // File.getcwd++
~audio_dir = ~working_dir++"AUDIO/";
~wav2wav = ~working_dir++"wav2wav_v1"
)



/////////////////////////////////////////////////////////////////////////////////////////////////////
/// INTERACTION WITH WAV2WAV
/////////////////////////////////////////////////////////////////////////////////////////////////////

(
//1) generate the analysis files. Example:
// ~preprocessCMD.("child-short.wav", ["anvil[0].wav","anvil[100].wav","anvil[-100].wav"], 60, "test_child").unixCmd{|res, pid| a=res};
~preprocessCMD = {|origName, matNamesList, downRatio, projectName| var matPaths;
	matNamesList.do{|name| matPaths = matPaths ++ ~audio_dir ++ name ++ " "};
	~wav2wav++" -a preprocess -s "++~audio_dir++origName++" -m "++matPaths++"-r "++downRatio++" -p "++projectName;
};

//2) calculate an optimized reconstruction and save as a .txt list. Example:
// ~generate_listCMD.("test_child", 600).unixCmd{|res, pid| a=res};
~generate_listCMD = {|projectName, iterations|
	~wav2wav++" -a generate_list -p "++projectName++" -i "++iterations;
};

//3) generate the .wav file from the list. Example:
//~generate_wavCMD.("test_child", 60, 600).unixCmd{|res, pid| a=res};
~generate_wavCMD = {|projectName, ratio, iterations|
	~wav2wav++" -a generate_wav -p "++projectName
	++" -d reconstruction_"++iterations++"iterations_"++ratio++"ratio.txt";
};
)



/////////////////////////////////////////////////////////////////////////////////////////////////////
/// INPUT+FEATURE EXTRACTOR (SERVER SIDE)
/////////////////////////////////////////////////////////////////////////////////////////////////////
~circularBuf = Buffer.alloc(s, 5*441);


(
SynthDef("test",{ |bufId, noiseGate=0.1| var blobBoundary, blobIntegral, suitableOriginal, suitableMaterial, lastIdx, newIdx, elapsedSamples, in, env, phasor, buf, sig, cond1, cond2, cond3, cond4, cond5, cond6, cond7, entropy;
	#blobBoundary, lastIdx = LocalIn.ar(2);
	elapsedSamples = Integrator.ar(DC.ar(1), 1-blobBoundary);
	in = SoundIn.ar;
	in = Compander.ar(in,in, thresh:noiseGate, slopeBelow:5);
	phasor = Phasor.ar(end:BufFrames.kr(bufId));
	newIdx = phasor;
	buf = BufWr.ar(in, phase:phasor);
	env = LagUD.ar(in.abs, lagTimeU:0.1, lagTimeD:0.1); // env follower
	cond1 = env < 0.01; // if envelope is low
	cond2 = HPZ1.ar(env) < 0.2; // if derivative of envelope is low
	blobIntegral = Integrator.ar(env, 1-blobBoundary);
	cond3 = blobIntegral/elapsedSamples > 0.0001; // if blob has enough surface
 	blobBoundary = cond1&cond2&cond3; // if (cond1 ... ):
	// calculate entropy
	entropy = -1*env/blobIntegral* Integrator.ar((env/blobIntegral).log2, 1-blobBoundary);
	// conditions for original
	cond4 = elapsedSamples>=1000 & elapsedSamples<44100; // suitable size
	cond5 = entropy < 0.5; // suitable entropy
	suitableOriginal = cond4&cond5;
	// conditions for material
	cond6 = elapsedSamples>=44100 & elapsedSamples<BufFrames.kr(bufId); // suitable size
	cond7 = entropy >= 0.5; // suitable entropy
	suitableMaterial = cond6&cond7;
	// send accepted blobs
	//SendReply.ar(blobBoundary&suitableOriginal, '/original', [lastIdx, newIdx, entropy]);
	SendReply.ar(blobBoundary&suitableMaterial, '/material', [lastIdx, newIdx, entropy]);
	// reinitialize blob
	lastIdx = Latch.ar(phasor, blobBoundary);
	LocalOut.ar([blobBoundary, lastIdx]);
	// debugging
/*	SendReply.ar(cond1, '/cond1', 1);
	SendReply.ar(cond2, '/cond2', 1);
	SendReply.ar(cond3, '/cond3', 1);
	SendReply.ar(cond4, '/cond4', 1);
	SendReply.ar(cond5, '/cond5', 1);
	SendReply.ar(cond6, '/cond6', 1);
	SendReply.ar(cond7, '/cond7', 1);*/
	Out.ar([0,1], in*0.1);
}).add;
)



~circularBuf.play

(

x= Synth("test", [\bufId, ~circularBuf.bufnum]);
a= OSCFunc({|msg| msg.postln}, '/cond1');
b= OSCFunc({|msg| msg.postln}, '/cond2');
c= OSCFunc({|msg| msg.postln}, '/cond3');
d= OSCFunc({|msg| msg.postln}, '/cond4');
e= OSCFunc({|msg| msg.postln}, '/cond5');
f= OSCFunc({|msg| msg.postln}, '/cond6');
g= OSCFunc({|msg| msg.postln}, '/cond7');
p= OSCFunc({|msg| msg.postln}, '/original');
q= OSCFunc({|msg| msg.postln}, '/material');
)

x.set(\noiseGate,0.1);

x= Synth("test", [\bufId, ~circularBuf.bufnum]);
o= OSCFunc({|msg| msg.postln}, '/asdf');

1
o.()

Timer
Integrator



x={SoundIn.ar*2}.play

{SinOsc.ar*1}.play


LagUD
Z1
Abs

LagUD // env. follower
OneZero // derivative

Latch // memory cell (integers as states
Gate
ToggleFF
SetResetFF
Schmidt // thresholding (higher & lower)

RecordBuf // easier but buf position has to becalculatedout of sync?

BufWr // controlled by a phasor, info is there


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// CLASSIFIER (CLIENT SIDE)
/////////////////////////////////////////////////////////////////////////////////////////////////////

(
~bufEntropy = {|bufId| var x;
	[1,2,a];
};
~test.(10);
)





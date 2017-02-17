s.boot;

(
~bufDur        = 4.0;         // total circular buf duration in seconds
~minBlobDur    = 0.2;         // minimum blob duration in seconds
~maxBlobDur    = ~bufDur / 2; // maximum blob duration in seconds
~sr            = 44100;       // sample rate in Hz
~lowThresh     = -20.dbamp;   // lower envelope threshold
~highThresh    = -12.dbamp;   // upper envelope threshold

~bufCircFrames = (~bufDur     * ~sr).asInteger;
~bufLinFrames  = (~maxBlobDur * ~sr).asInteger;
~bufCirc       = Buffer.alloc(s, ~bufCircFrames);
~bufLin        = Buffer.alloc(s, ~bufLinFrames);

// helper function for UGen comparison
~is    = { arg a, b; BinaryOpUGen('==', a, b) };
~isNot = { arg a, b; BinaryOpUGen('!=', a, b) };
)

(
~analysis = play {
	var buf, bufFrames, in, phase, phaseK, state, env, startFrame,
	    cond1, gate1, cond2, cond3, cond4, condSend, elapsed, values,
	    isLow, isNotLow, isHigh, debug;

	#state, startFrame = LocalIn.kr(numChannels: 2);
	debug      = \debug.kr;
	buf        = ~bufCirc.bufnum;
	bufFrames  = BufFrames.kr(buf);
	in         = SoundIn.ar;
	phase      = Phasor.ar(rate: 1, start: 0, end: bufFrames);
	phaseK     = A2K.kr(phase);
	BufWr.ar(in, buf, phase);

	env        = A2K.kr(Lag.ar(in.squared, 0.1));
	isLow      = env <  ~lowThresh;
	isNotLow   = env >= ~lowThresh;
	isHigh     = env >  ~highThresh;
	cond1      = ~is.(state, 0) & isLow;    // transition to state 1
	cond2      = ~is.(state, 1) & isNotLow; // transition to state 2
    cond3      = ~is.(state, 2) & isHigh;   // transition to state 3
    cond4      = ~is.(state, 3) & isLow;    // transition to state 4

	env.poll(Impulse.kr(4) & debug, "env");

	elapsed    = Sweep.kr(trig: cond2, rate: 1);

	condSend   = cond4 & (elapsed > ~minBlobDur) & (elapsed < ~maxBlobDur);

	values     = [startFrame, phaseK];
	SendReply.kr(condSend, cmdName: '/blob', values: values);
	values.poll(condSend & debug, "send");

	state      = (state + (cond1 | cond2 | cond3 | cond4)) % 4;
	startFrame = Latch.kr(phaseK, cond2);
	LocalOut.kr([state, startFrame]);

	cond1.poll(cond1 & debug, "state 1");
	cond2.poll(cond2 & debug, "state 2");
	cond3.poll(cond3 & debug, "state 3");
	cond4.poll(cond4 & debug, "state 4");
};
)

// for debugging
~analysis.set(\debug, 1);

(
~listen = OSCFunc(path: '/blob', func: { arg msg;
	msg.postln;
});
)

// create test send
(
play {
	Out.ar(NumOutputBuses.ir, WhiteNoise.ar(Line.ar(0, 2, 2, doneAction: 2)))
};
)

// stop
(
~analysis.free;
~listen.free;
)
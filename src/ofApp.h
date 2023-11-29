#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "PulseOsc.h"

class ofApp : public ofBaseApp, ofxMidiListener {

public:
	void setup();
	void draw();
	void exit();
	void ofSoundStreamSetup(ofSoundStreamSettings& settings);
	void audioOut(ofSoundBuffer& buffer);
	void newMidiMessage(ofxMidiMessage& message);
	void audioSetup();
	void midiSetup();
	void refresh();
	void updateParameter(int control);
	float normalizeMidi(int control);
	void getSample();
	float getPan(int controlA, int controlB, float sampleA, float sampleB);
	float getDuty(int control, int controlA, int controlB, float sampleA, float sampleB);
	float getFrequency(int control, int controlA, int controlB, float sampleA, float sampleB);
	float getAmplitude(int control, int controlA, int controlB, float sampleA, float sampleB);
	float getArgument(float center, float maximumIndex, int controlA, int controlB, float sampleA, float sampleB);
	inline float mix3(float a, float b, float c);
	void setUniforms();
	double getColor(double sampleTotal);
	int sampleRate;
	int bufferSize;
	int channels;
	ofSoundStreamSettings settings;
	ofSoundStream stream;
	float nyquist;
	float startPan;
	int port;
	ofxMidiIn midiIn;
	array<float, 33> controls;
	array<float, 33> parameters;
	float sampleA;
	float sampleB;
	float sampleC;
	double minimumDouble;
	double sampleATotal;
	double sampleBTotal;
	double sampleCTotal;
	double samplesElapsed;
	array<float, 2> panA;
	array<float, 2> panB;
	array<float, 2> panC;
	array<float, 2> sample;
	int width;
	int height;
	int x;
	int y;
	ofVec2f window;
	ofVec3f rgb;
	ofFbo buffer0;
	ofFbo buffer1;
	ofShader shader;
	pulseOsc oscillatorA;
	pulseOsc oscillatorB;
	pulseOsc oscillatorC;
};
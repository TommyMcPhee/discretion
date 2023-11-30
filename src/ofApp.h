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
	float unipolarControl(int control);
	void getSample();
	float getPan(int controlA, int controlB, float sampleA, float sampleB);
	float getDuty(int control, int controlA, int controlB, float sampleA, float sampleB);
	float getFrequency(int control, int controlA, int controlB, float sampleA, float sampleB);
	float getAmplitude(int control, int controlA, int controlB, float sampleA, float sampleB);
	float getArgument(float center, float maximumIndex, int controlA, int controlB, float sampleA, float sampleB);
	inline float mix3(float a, float b, float c);
	void setUniforms();
	float getXY(double total);
	float getZ(double sampleTotal);
	ofVec3f getVec(int control);
	ofVec3f getPanVec(int control);
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
	float dutyA;
	float dutyB;
	float dutyC;
	float frequencyA;
	float frequencyB;
	float frequencyC;
	float amplitudeA;
	float amplitudeB;
	float amplitudeC;
	float sampleA;
	float sampleB;
	float sampleC;
	double minimumDouble;
	double samplesElapsed;
	double dutyATotal;
	double dutyBTotal;
	double dutyCTotal;
	double frequencyATotal;
	double frequencyBTotal;
	double frequencyCTotal;
	double sampleATotal;
	double sampleBTotal;
	double sampleCTotal;
	float length;
	float feedbackIncrement;
	float feedbackPhasor;
	float feedback;
	array<float, 2> panA;
	array<float, 2> panB;
	array<float, 2> panC;
	array<float, 2> lastSample;
	array<float, 2> sample;
	int width;
	int height;
	int x;
	int y;
	ofVec2f window;
	ofVec3f xTranslate;
	ofVec3f yTranslate;
	ofVec3f zTranslate;
	ofVec3f xDuty;
	ofVec3f yDuty;
	ofVec3f xFrequency;
	ofVec3f yFrequency;
	ofVec3f xAmplitude;
	ofVec3f yAmplitude;
	ofVec3f xPan;
	ofVec3f yPan;
	ofFbo buffer0;
	ofFbo buffer1;
	ofShader shader;
	pulseOsc oscillatorA;
	pulseOsc oscillatorB;
	pulseOsc oscillatorC;
};
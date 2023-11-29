#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	audioSetup();
	for (int a = 0; a < 2; a++) {
		panA[a] = startPan;
		panB[a] = startPan;
		sample[a] = startPan;
	}
	oscillatorA = pulseOsc(0.5, 800, 0.0, 0.5, sampleRate);
	oscillatorB = pulseOsc(0.5, 800, 0.0, 0.5, sampleRate);
	oscillatorC = pulseOsc(0.5, 800, 0.0, 0.5, sampleRate);
	midiSetup();
	shader.load("discretionShader");
	buffer0.allocate(ofGetScreenWidth(), ofGetScreenHeight());
	buffer0.clear();
	buffer1.allocate(ofGetScreenWidth(), ofGetScreenHeight());
	buffer1.clear();
	buffer0.begin();
	ofClear(0, 0, 0, 255);
	buffer0.end();
	buffer1.begin();
	ofClear(0, 0, 0, 255);
	buffer1.end();
	x = 0.0;
	y = 0.0;
	for (int a = 0; a < controls.size(); a++) {
		controls[a] = 0.0;
		updateParameter(a);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	refresh();
	buffer0.begin();
	shader.begin();
	setUniforms();
	buffer1.draw(x, y);
	shader.end();
	buffer0.end();
	buffer1.begin();
	buffer0.draw(x, y);
	buffer1.end();
	buffer0.draw(x, y);
}

//--------------------------------------------------------------
void ofApp::exit() {

}


void ofApp::ofSoundStreamSetup(ofSoundStreamSettings& settings) {

}

void ofApp::audioOut(ofSoundBuffer& buffer) {
	float average = 0.0;
	for (int a = 0; a < buffer.getNumFrames(); a++) {
		getSample();
		for (int b = 0; b < channels; b++) {
			buffer[a * channels + b] = sample[b];
			average += sample[b];
		}
	}
	average /= ((float)bufferSize * 2);
}

void ofApp::newMidiMessage(ofxMidiMessage& message) {
	int control = message.control;
	if (message.status == MIDI_CONTROL_CHANGE && control <= controls.size()) {
		controls[control] = (float)message.value - 63.5;
		updateParameter(control);
	}
}

void ofApp::audioSetup() {
	sampleRate = 48000;
	bufferSize = 256;
	channels = 2;
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.bufferSize = bufferSize;
	settings.numOutputChannels = channels;
	settings.setApi(ofSoundDevice::Api::MS_DS);
	stream.setup(settings);
	nyquist = (float)sampleRate * 0.5;
	startPan = sqrt(0.5);
	minimumDouble = std::numeric_limits<double>::min();
	sampleATotal = minimumDouble;
	sampleBTotal = minimumDouble;
	sampleCTotal = minimumDouble;
	samplesElapsed = 0.0;
}

void ofApp::midiSetup() {
	port = 0;
	midiIn.openPort(port);
	midiIn.addListener(this);
}

void ofApp::refresh() {
	width = (float)ofGetWidth();
	height = (float)ofGetHeight();
	buffer0.allocate(width, height);
	buffer1.allocate(width, height);
	window.set(width, height);
	ofClear(0, 0, 0, 255);
}

void ofApp::updateParameter(int control) {
	if (control >= 1 && control <= 6 || control >= 9 && control <= 14 || control >= 17 && control <= 22 || control >= 25 && control <= 30) {
		if (control >= 4 && control <= 6) {
			parameters[control] = nyquist * normalizeMidi(control);
		}
		if (control != 0 && control <= 3) {
			parameters[control] = normalizeMidi(control) * 0.5 + 0.5;
		}
		if (control >= 9 && control <= 14 || control >= 17 && control <= 22 || control >= 25 && control <= 30) {
			parameters[control] = normalizeMidi(control);
		}
	}
	else {
		parameters[control] = (controls[control] + 63.5) / 127.0;
	}
}

float ofApp::normalizeMidi(int control) {
	return pow(controls[control] / 64.0, 3.0);
}

void ofApp::getSample() {
	samplesElapsed++;
	panA[0] = getPan(15, 16, sampleB, sampleC);
	panB[1] = 1.0 - panA[0];
	panB[0] = getPan(23, 24, sampleA, sampleC);
	panB[1] = 1.0 - panB[0];
	panC[0] = getPan(31, 32, sampleB, sampleC);
	panC[1] = 1.0 - panC[0];
	oscillatorA.setDuty(getDuty(1, 11, 13, sampleB, sampleC));
	oscillatorA.setFreq(getFrequency(4, 19, 21, sampleB, sampleC));
	oscillatorA.setAmp(getAmplitude(7, 27, 29, sampleB, sampleC));
	oscillatorB.setDuty(getDuty(2, 9, 14, sampleA, sampleC));
	oscillatorB.setFreq(getFrequency(5, 17, 22, sampleA, sampleC));
	oscillatorB.setAmp(getAmplitude(8, 25, 30, sampleA, sampleC));
	oscillatorC.setDuty(getDuty(3, 10, 12, sampleA, sampleC));
	oscillatorC.setFreq(getFrequency(6, 18, 20, sampleA, sampleB));
	oscillatorC.setAmp(getAmplitude(0, 26, 28, sampleA, sampleC));
	sampleA = oscillatorA.getSample();
	sampleB = oscillatorB.getSample();
	sampleC = oscillatorC.getSample();
	sampleATotal += sampleA;
	sampleBTotal += sampleB;
	sampleCTotal += sampleC;
	for (int a = 0; a < 2; a++) {
		sample[a] = mix3(sampleA * panA[a] / startPan, sampleB * panB[a] / startPan, sampleC * panC[a] / startPan);
	}
}

float ofApp::getPan(int controlA, int controlB, float sampleA, float sampleB) {
	return (parameters[controlA] * sampleA) + (parameters[controlB] * sampleB) * 0.5 + 0.5;
}

float ofApp::getDuty(int control, int controlA, int controlB, float sampleA, float sampleB) {
	float centerWidth = parameters[control];
	float maximumWidthIndex = abs(0.5 - centerWidth) / 2.0;
	return getArgument(centerWidth, maximumWidthIndex, controlA, controlB, sampleA, sampleB);
}

float ofApp::getFrequency(int control, int controlA, int controlB, float sampleA, float sampleB) {
	float centerFrequency = parameters[control];
	float maximumFrequencyIndex = (nyquist - abs(centerFrequency)) / 2.0;
	return centerFrequency + (parameters[controlA] * sampleA * maximumFrequencyIndex) + (parameters[controlB] * sampleB * maximumFrequencyIndex);
}

float ofApp::getAmplitude(int control, int controlA, int controlB, float sampleA, float sampleB) {
	float centerAmplitude = parameters[control];
	float maximumAmplitudeIndex = abs(0.5 - centerAmplitude) / 2.0;
	return getArgument(centerAmplitude, maximumAmplitudeIndex, controlA, controlB, sampleA, sampleB);
}

float ofApp::getArgument(float center, float maximumIndex, int controlA, int controlB, float sampleA, float sampleB) {
	return center + (parameters[controlA] * sampleA * maximumIndex) + (parameters[controlB] * sampleB * maximumIndex);
}

inline float ofApp::mix3(float a, float b, float c) {
	return (a + b + c) / 3.0;
}

void ofApp::setUniforms() {
	shader.setUniform2f("window", window);
	rgb.set(getColor(sampleATotal), getColor(sampleBTotal), getColor(sampleCTotal));
	shader.setUniform3f("rgb", rgb);
}

double ofApp::getColor(double sampleTotal) {
	return (sampleTotal + 1.0) / (2.0 * samplesElapsed);
}
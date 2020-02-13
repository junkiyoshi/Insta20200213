#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	// ������exsample/sound�ɉ����������T���v��������܂��B
	// �����soundPlayerFFTExample���Q�l�ɂ��܂����B
	// https://github.com/openframeworks/openFrameworks/tree/patch-release/examples/sound

	// �T�E���h�f�[�^���w�肵�܂��B
	// [ProjectDirectory]/bin/data ����̑��΃p�X�Ŏw��\�ł��B
	// ��L�̈Ӗ���������Ȃ���΁A��΃p�X(�h���C�u����̃t���p�X)�Ŏw�肷��Α��v�ł��B
	this->sound.load("sound/This_is_Not_Jazz.mp3");	// https://openframeworks.cc/documentation/sound/ofSoundPlayer/#show_load
	this->sound.play();									// https://openframeworks.cc/documentation/sound/ofSoundPlayer/#show_play

	// �擾�������ʂ��i�[����z��̏��������܂��B
	// �l�����̂܂܊i�[���� fft_value
	// �l���ۂ߂Ċi�[���� fft_smoothed
	this->n_bands_to_get = 128;
	for (int j = 0; j < this->n_bands_to_get; j++) {

		this->fft_value.push_back(0.f);
		this->fft_smoothed.push_back(0.f);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSoundUpdate();	// https://openframeworks.cc/documentation/sound/ofSoundPlayer/#show_ofSoundUpdate

	// ���g���X�y�N�g���T���v�����擾���܂��B(�������e�͏ڂ�������܂���...)
	// ���ʂ��|�C���^�Ƃ��ĕԂ��Ă��܂����A�w�肵�������̗v�f��������float�z��̃|�C���^�ł��B
	float* val = ofSoundGetSpectrum(this->n_bands_to_get);	// http://openframeworks.jp/documentation/sound/ofSoundPlayer.html#!show_ofSoundGetSpectrum
	for (int i = 0; i < this->n_bands_to_get; i++) {
		
		// fft_value�́A�擾�l�����̂܂ܑ���B
		this->fft_value[i] = val[i];

		// fft_smoothed�́A����̎����Ă���l�����������āA�擾�l�Ɣ�r�A�l���傫�����������܂��B
		// ����ɂ���āAfft_smoothed�̒l�́A���炩�Ɍ������铮���ɂȂ�܂��B
		this->fft_smoothed[i] *= 0.98f;
		if (this->fft_smoothed[i] < val[i]) {

			this->fft_smoothed[i] = val[i];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// fft_value��fft_smoothed�̒l���g���ĕ`�ʂ��s���܂��B
	// ����͒l���{�b�N�X��`�ʂ�����A�ʒu�Ƃ��č̗p���Ă��܂��B
	
	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);

	ofColor color;
	for (int i = 0; i < 7; i++) {

		float x = i * 100 - 300;
		float value = this->fft_value[i];
		float tmp_value = 0;

		while (value > tmp_value) {

			ofSetColor(39);
			ofFill();
			ofDrawBox(glm::vec3(x, tmp_value * 100, 0), 75, 20, 75);

			ofSetColor(239);
			ofNoFill();
			ofDrawBox(glm::vec3(x, tmp_value * 100, 0), 75, 20, 75);

			tmp_value += 0.2;
		}

		color.setHsb(ofMap(i, 0, 7, 0, 255), 180, 255);
		ofSetColor(color);
		ofFill();
		ofDrawBox(glm::vec3(x, this->fft_smoothed[i] * 150, 0), 75, 25, 75);

		ofSetColor(39);
		ofNoFill();
		ofDrawBox(glm::vec3(x, this->fft_smoothed[i] * 150, 0), 75, 25, 75);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
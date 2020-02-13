#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	// 公式のexsample/soundに音を扱ったサンプルがあります。
	// 今回はsoundPlayerFFTExampleを参考にしました。
	// https://github.com/openframeworks/openFrameworks/tree/patch-release/examples/sound

	// サウンドデータを指定します。
	// [ProjectDirectory]/bin/data からの相対パスで指定可能です。
	// 上記の意味が分からなければ、絶対パス(ドライブからのフルパス)で指定すれば大丈夫です。
	this->sound.load("sound/This_is_Not_Jazz.mp3");	// https://openframeworks.cc/documentation/sound/ofSoundPlayer/#show_load
	this->sound.play();									// https://openframeworks.cc/documentation/sound/ofSoundPlayer/#show_play

	// 取得した結果を格納する配列の初期化します。
	// 値をそのまま格納する fft_value
	// 値を丸めて格納する fft_smoothed
	this->n_bands_to_get = 128;
	for (int j = 0; j < this->n_bands_to_get; j++) {

		this->fft_value.push_back(0.f);
		this->fft_smoothed.push_back(0.f);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSoundUpdate();	// https://openframeworks.cc/documentation/sound/ofSoundPlayer/#show_ofSoundUpdate

	// 周波数スペクトルサンプルを取得します。(私も内容は詳しくありません...)
	// 結果がポインタとして返ってきますが、指定した引数の要素数を持つfloat配列のポインタです。
	float* val = ofSoundGetSpectrum(this->n_bands_to_get);	// http://openframeworks.jp/documentation/sound/ofSoundPlayer.html#!show_ofSoundGetSpectrum
	for (int i = 0; i < this->n_bands_to_get; i++) {
		
		// fft_valueは、取得値をそのまま代入。
		this->fft_value[i] = val[i];

		// fft_smoothedは、現状の持っている値を小さくして、取得値と比較、値が大きい方を代入します。
		// これによって、fft_smoothedの値は、滑らかに減少する動きになります。
		this->fft_smoothed[i] *= 0.98f;
		if (this->fft_smoothed[i] < val[i]) {

			this->fft_smoothed[i] = val[i];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// fft_valueとfft_smoothedの値を使って描写を行います。
	// 今回は値をボックスを描写する個数、位置として採用しています。
	
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
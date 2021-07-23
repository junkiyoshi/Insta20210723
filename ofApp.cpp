#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableDepthTest();

	this->font_size = 80;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);

	this->noise_param_list.push_back(ofRandom(1000));
}


//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 30 < 20) {

		auto noise_param = this->noise_param_list.back() + ofMap(ofGetFrameNum() % 30, 0, 20, 0.02, 0.001);
		this->noise_param_list.push_back(noise_param);
	}
	else {

		auto noise_param = this->noise_param_list.back();
		this->noise_param_list.push_back(noise_param);
	}

	while (this->noise_param_list.size() > 5) {

		this->noise_param_list.erase(this->noise_param_list.begin());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	string word = "TOKYO OLYMPIC 2020+1";
	int sample_count = 50;
	int span = 200;
	vector<glm::vec3> base_location = {
		glm::vec3(-250, -250, 0), glm::vec3(250, -250, 0),
		glm::vec3(-250, 250, 0), glm::vec3(250, 250, 0),
	};

	for (int k = 0; k < base_location.size(); k++) {

		for (int radius = 230; radius <= 230; radius += 100) {

			auto noise_param = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
			for (int i = 0; i < word.size(); i++) {

				ofPath chara_path = this->font.getCharacterAsPoints(word[i], true, false);
				vector<ofPolyline> outline = chara_path.getOutline();

				ofFill();
				ofSetColor(0);
				ofBeginShape();
				for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

					if (outline_index != 0) { ofNextContour(true); }

					auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
					for (auto& vertex : vertices) {

						auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 320);
						auto rotation = glm::rotate(glm::mat4(), (i * 13.5f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
						location = glm::vec4(location, 0) * rotation;

						auto noise_location = glm::vec4(this->font_size * 0.5, this->font_size * -0.5, 320, 0) * rotation;
						auto angle_x = ofMap(ofNoise(noise_param.x, noise_location.x * 0.0005, this->noise_param_list[0]), 0, 1, -PI * 1.5, PI * 1.5);
						auto angle_y = ofMap(ofNoise(noise_param.x, noise_location.y * 0.0005, this->noise_param_list[0]), 0, 1, -PI * 1.5, PI * 1.5);
						auto angle_z = ofMap(ofNoise(noise_param.x, noise_location.z * 0.0005, this->noise_param_list[0]), 0, 1, -PI * 1.5, PI * 1.5);

						auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
						auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
						auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

						location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;
						location = glm::normalize(location) * radius;
						location += base_location[k];

						ofVertex(location);
					}
				}
				ofEndShape();

				for (int m = 0; m < this->noise_param_list.size(); m++) {

					ofNoFill();
					ofSetColor(255);
					ofBeginShape();
					for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

						if (outline_index != 0) { ofNextContour(true); }

						auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
						for (auto& vertex : vertices) {

							auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 320);
							auto rotation = glm::rotate(glm::mat4(), (i * 13.5f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
							location = glm::vec4(location, 0) * rotation;

							auto noise_location = glm::vec4(this->font_size * 0.5, this->font_size * -0.5, 320, 0) * rotation;
							auto angle_x = ofMap(ofNoise(noise_param.x, noise_location.x * 0.0005, this->noise_param_list[m]), 0, 1, -PI * 1.5, PI * 1.5);
							auto angle_y = ofMap(ofNoise(noise_param.x, noise_location.y * 0.0005, this->noise_param_list[m]), 0, 1, -PI * 1.5, PI * 1.5);
							auto angle_z = ofMap(ofNoise(noise_param.x, noise_location.z * 0.0005, this->noise_param_list[m]), 0, 1, -PI * 1.5, PI * 1.5);

							auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
							auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
							auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

							location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;
							location = glm::normalize(location) * radius;
							location += base_location[k];

							ofVertex(location);
						}
					}
					ofEndShape(true);
				}
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
// Project : CS 330
// Program : Simple movement
// Purpose : Implement simple movement update algorithms(section 3.1).
// Author  : Yingren Wang
// Created : 2020-2-2

#include <utility>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <fstream>

using namespace std;

// declare class Kinematic
class Kinematic {
	pair<float, float> position;
	float orientation = 0.0;
	pair<float, float> velocity;
	float rotation = 0.0;

public:
	// set and get functions
	void setPosition(pair<float, float>p) {
		position = p;
	}
	void setVelocity(pair<float, float> v) {
		velocity = v;
	};
	void setOrientation(float o) {
		orientation = o;
	}
	void setRotation(float r) {
		rotation = r;
	}
	pair<float, float> getPosition() {
		return position;
	}
	pair<float, float> getVelocity() {
		return velocity;
	}
	float getOrientation() {
		return orientation;
	}
	float getRotation() {
		return rotation;
	}
};

// Declare class SteeringOutput
class SteeringOutput {
	pair<float, float> linear;
	float angular = 0.0;

public:
	// set and get functions
	pair<float, float> getLinear() {
		return linear;
	}
	float getAngular() {
		return angular;
	}
	void setLinear(pair<float, float> l) {
		linear = l;
	}
	void setAngular(float a) {
		angular = a;
	}
};

// function to update kinematic and steering depending on the physics setting and deltaTime
void update(SteeringOutput& steering, Kinematic& kinematic, float time, bool physics) {
	// if physics == true, HS calculation, else regular calculation
	float half_t_sq = 0.0;
	if (physics == true) {
		half_t_sq = 0.5 * time * time;
	}
	
	// update position and orientation
	pair<float, float> position;
	position.first = kinematic.getPosition().first
		+ (kinematic.getVelocity().first * time)
		+ (steering.getLinear().first * half_t_sq);
	position.second = kinematic.getPosition().second
		+ (kinematic.getVelocity().second * time)
		+ (steering.getLinear().second * half_t_sq);

	kinematic.setPosition(position);

	float orientation 
		= kinematic.getOrientation() 
		+ (kinematic.getRotation() * time) 
		+ (steering.getAngular() * half_t_sq);
	kinematic.setOrientation(orientation);


	// update velocity and rotation
	pair<float, float> velocity;
	velocity.first = kinematic.getVelocity().first
		+ (steering.getLinear().first * time);
	velocity.second = kinematic.getVelocity().second
		+ (steering.getLinear().second * time);
	float rotation
		= kinematic.getRotation() + (steering.getAngular() * time);

	kinematic.setVelocity(velocity);
	kinematic.setRotation(rotation);
}

int main() 
{
	// initialization
	Kinematic kinematic;
	kinematic.setPosition(make_pair(0, 60));
	kinematic.setVelocity(make_pair(8, 2));
	kinematic.setOrientation(0);
	kinematic.setRotation(0);
	SteeringOutput steeringOutput;
	steeringOutput.setAngular(0.00);
	steeringOutput.setLinear(make_pair(0, 1));
	
	float time = 0;
	float deltaTime = 0.25;
	bool physics = false;
	bool align = false;

	// open file to write in
	ofstream myfile;
	myfile.open("CS 330 20S, P1, Wang, Trajectory 4.txt");
	if (!myfile) {
		cout << "Trouble opening the Trajectory 4 file" << endl;
	}
	else {
		cout << "Successfully opened the Trajectory 4 file" << endl;
	}

	// comment/uncomment line 139 depending on whether to use HS calculation or not
	 physics = true;
	 // align = true;

	while (time < 100) {
		
		cout << time << "," << kinematic.getPosition().first << "," << kinematic.getPosition().second << ","
			<< kinematic.getVelocity().first << "," << kinematic.getVelocity().second << ","
			<< steeringOutput.getLinear().first << "," << steeringOutput.getLinear().second << ","
			<< kinematic.getOrientation() << "," << kinematic.getRotation() << ","
			<< steeringOutput.getAngular() << "\n";
		myfile << time << "," << kinematic.getPosition().first << "," << kinematic.getPosition().second << ","
			<< kinematic.getVelocity().first << "," << kinematic.getVelocity().second << ","
			<< steeringOutput.getLinear().first << "," << steeringOutput.getLinear().second << ","
			<< kinematic.getOrientation() << "," << kinematic.getRotation() << ","
			<< steeringOutput.getAngular() << "\n";

		time = time + deltaTime;
		update(steeringOutput, kinematic, deltaTime, physics);

		if (align) {
			float newO = atan2(kinematic.getVelocity().second, kinematic.getVelocity().first);
			kinematic.setOrientation(newO);
		}
		
		// update linear and angular
		pair<float, float> linear;
		linear.first = (kinematic.getPosition().first / (-100) * deltaTime) * 9;
		linear.second = (kinematic.getPosition().second / (-100) * deltaTime) * 3;

		steeringOutput.setLinear(linear);
		steeringOutput.setAngular((50 - time) * 0.0001);

	}

	return 0;
}

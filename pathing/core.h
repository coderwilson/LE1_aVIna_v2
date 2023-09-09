#pragma once
#include <iostream>
#include <cmath>
#include "../config/global_objects.h"
#include "checkpoints.h"

class path_class {
private:
	int checkpoint = 0;
	double dist = 999.9;

	double tar_dist(float ax, float ay, float bx, float by) {
		return sqrt(pow(by - ay, 2) + pow(bx - ax, 2));
	}
	bool to_checkpoint_engage(float target[2], int forward) {
		float* player = memory.shep_coords();
		float* cam = memory.cam_coords();
		float player_x = player[1]; // player x from memory
		float player_y = player[0]; // player y from memory
		float north = cam[3]; // camera north value
		float east = cam[4]; // Camera east value

		// Start of function
		float tx = target[1];
		float ty = target[0];
		float m1, m2;
		float pX = player_x;
		float pY = player_y;
		if (north != 0.0) {
			m1 = east / north;
		}
		else {
			m1 = (float)9999.9;
		}
		if (tx - pX == 0) {
			m2 = 99999.9;
		}
		else {
			m2 = (ty - pY) / (tx - pX);
		}
		float angle = atan((m1 - m2) / (1 + (m1 * m2)));
		float Ly = -sin(angle);
		float Lx = cos(angle);
		int x_sign = (int)copysign(1, Ly);
		float sums_up = abs(Lx) + abs(Ly);
		if (sums_up == 0) {
			sums_up = (float)0.000001;
		}
		Lx /= sums_up;
		Ly /= sums_up;
		bool test = abs(Lx) > abs(Ly);
		if (abs(Lx) > abs(Ly)) {
			Ly = (float)copysign(Ly / Lx, 1);
			Lx = (float)copysign(1, x_sign);
		}
		else {
			Lx = (float)copysign(Lx / Ly, x_sign);
			Ly = 1;
		}

		//Next, check direction of movement
		float player_angle = atan2(east, north);
		float dest_angle = -atan2((tx - pX), (ty - pY));
		double angle_diff = (double)abs(player_angle - dest_angle);
		double min_angle = (double)(3.1415926535 * 0.5);
		double max_angle = (double)(3.1415926535 * 1.5);
		bool reverse = false;  // for debugging only
		if (max_angle > angle_diff && angle_diff > min_angle) {
			Ly *= -1;
			Lx *= -1;
			reverse = true;  // for debugging only
		}
		xbox.set_movement(Lx, Ly);  // This line of code sets the movement on the controller.

		float h_cam = 0;
		float v_cam = 0;
		if (forward == 1) {

			// Horizontal adjustment (left/right)
			if (Lx > 0.15) h_cam = 0.9;
			else if (Lx > 0.05) h_cam = 0.3;
			else if (Lx < -0.15) h_cam = -0.9;
			else if (Lx < -0.05) h_cam = -0.3;

			// Vertical adjustment (up/down)
			if (cam[5] < -0.1) v_cam = 0.6;
			else if (cam[5] > 0.1) v_cam = -0.6;
			
			// Send command to the controller.
		}

		// If forward == -1, the right stick is controlled elsewhere, probably for combat.
		// Otherwise, direct camera forward, or set to neutral.
		if (forward != -1) xbox.set_aim(h_cam, v_cam);

		// Return value if we are within 30 units of distance from the checkpoint. AKA reached checkpoint.
		if ((int)tar_dist(tx, ty, player_x, player_y) < 30) return true;
		return false;

	}

public:

	int engage(int checkpoint = 0, int forward = 1) {
		try {
			// std::cout << "Movement should be starting.\n";
			float* target = get_checkpoint(checkpoint);
			if (to_checkpoint_engage(target, forward) == true) return 0;
			// std::cout << "Movement end.\n";
			return 1;
		}
		catch (int e) {
			// void(e);
			return -1;
		}
	}
};
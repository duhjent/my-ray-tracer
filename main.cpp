#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <cmath>
#include <opencv2/opencv.hpp>

#define W_WIDTH 1024
#define W_HEIGHT 768
#define FOV M_PI_2

struct Vec3f {
	float q;
	float w;
	float e;

	Vec3f normalize() const {
		float len = sqrtf(q * q + w * w + e * e);
		return Vec3f{ q / len, w / len, e / len };
	}

	Vec3f minus(const Vec3f* other) const {
		return Vec3f{ q - other->q, w - other->w, e - other->e };
	}

	float dotProduct(const Vec3f* other) const {
		return q * other->q + w * other->w + e * other->e;
	}
};

struct Sphere {
	Vec3f position;
	float radius;

	bool rayIntersect(const Vec3f* origin, const Vec3f* direction,
		float* t0) const {
		Vec3f L = position.minus(origin);
		float tca = L.dotProduct(direction);
		if (tca < 0) {
			return false;
		}
		float d2 = L.dotProduct(&L) - (tca * tca);
		if (d2 > radius * radius) {
			return false;
		}
		float thc = sqrtf(radius * radius - d2);
		(*t0) = tca - thc;
		float t1 = tca + thc;
		if (*t0 > t1) {
			float t = *t0;
			(*t0) = t1;
			t1 = t;
		}
		if (*t0 < 0) {
			(*t0) = t1;
			if (*t0 < 0) {
				return false;
			}
		}

		return true;
	}
};

Vec3f castRay(const Vec3f* orig, const Vec3f* dir, const Sphere* sphere) {
	float t0;
	if (sphere->rayIntersect(orig, dir, &t0)) {
		return Vec3f{ 0.4f, 0.4f, 0.3f };
	}
	return Vec3f{ 0.2f, 0.7f, 0.8f };
}

void renderSphere(const Sphere* sphere, float* screenVec) {
	Vec3f origin = Vec3f{ 0, 0, 0 };
	for (int j = 0; j < W_HEIGHT; j++) {
		for (int i = 0; i < W_WIDTH; i++) {
			float x = (2 * (i + .5f) / (float)W_WIDTH - 1) * tanf(FOV / 2.0) *
				W_WIDTH / (float)W_HEIGHT;
			float y = -(2 * (j + .5f) / (float)W_HEIGHT - 1) * tanf(FOV / 2.0);
			Vec3f direction = Vec3f{ x, y, -1 }.normalize();
			Vec3f color = castRay(&origin, &direction, sphere);
			screenVec[(3 * W_WIDTH * j) + (3 * i) + 2] = color.q;
			screenVec[(3 * W_WIDTH * j) + (3 * i) + 1] = color.w;
			screenVec[(3 * W_WIDTH * j) + (3 * i) + 0] = color.e;
		}
	}
}

int main() {
	float* screenVec = new float[W_WIDTH * W_HEIGHT * 3];
	Sphere sphere = Sphere{ Vec3f{-3, 6, -13}, 5 };

	renderSphere(&sphere, screenVec);

	// for (int j = 0; j < W_HEIGHT; j++) {
	//   for (int i = 0; i < W_WIDTH; i++) {
	//     screenVec[(3 * W_WIDTH * j) + (3 * i) + 2] = (float)j / W_HEIGHT; // R
	//     screenVec[(3 * W_WIDTH * j) + (3 * i) + 1] = (float)i / W_WIDTH;  // G
	//     screenVec[(3 * W_WIDTH * j) + (3 * i)] = 0;                       // B
	//   }
	// }

	cv::Mat screen(W_HEIGHT, W_WIDTH, CV_32FC3);
	memcpy(screen.data, screenVec, W_WIDTH * W_HEIGHT * 3 * sizeof(float));

	delete[] screenVec;

	cv::imshow("image", screen);
	cv::waitKey(0);
	return 0;
}

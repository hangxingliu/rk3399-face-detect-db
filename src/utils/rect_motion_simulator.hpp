#ifndef UTILS_RECT_MOTION_SIMULATOR_HPP
#define UTILS_RECT_MOTION_SIMULATOR_HPP

class RectMotionSimulator_Status {
public:
	bool enable = false;
	double time = 0;
	double x = 0,y = 0,w = 0,h = 0;

	RectMotionSimulator_Status(double time): RectMotionSimulator_Status(false, time,0,0,0,0) {}
	RectMotionSimulator_Status(bool enable, double time, double x, double y, double w, double h) {
		this->enable = enable;
		this->time = time;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	static RectMotionSimulator_Status getDisable(double time) {
		RectMotionSimulator_Status status(time);
		return status;
	}
};

class RectMotionSimulator {
	bool enable0 = false;
	double time0 = 0;
	double x0, y0, w0, h0;

	bool enable1 = false;
	double time1 = 0;
	double x1, y1, w1, h1;

	void move1to0() {
		enable0 = enable1;
		time0 = time1;
		x0 = x1; y0 = y1;
		w0 = w1; h0 = h1;
	}

public:
	void addStatus(bool enable, double time, double x, double y, double w, double h) {
		move1to0();

		enable1 = enable;
		time1 = time;
		x1 = x; y1 = y;
		w1 = w; h1 = h;
	}
	void addStatus(RectMotionSimulator_Status status) {
		addStatus(status.enable, status.time, status.x, status.y, status.w, status.h);
	}
	RectMotionSimulator_Status getStatus(double time) {
		if(!enable1) // no motion status
			return RectMotionSimulator_Status::getDisable(time);
		if(!enable0) // same with status1
			return RectMotionSimulator_Status(true, time, x1, y1, w1, h1);

		double zoom = (time - time1) / (time1 - time0);

		return RectMotionSimulator_Status(true, time,
			x0 + (x1 - x0) * zoom,
			y0 + (y1 - y0) * zoom,
			w0 + (w1 - w0) * zoom,
			h0 + (h1 - h0) * zoom);
	}
};

#endif // UTILS_RECT_MOTION_SIMULATOR_HPP

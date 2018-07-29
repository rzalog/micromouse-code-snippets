// Most of these variables are globals, or class
// variables, don't lose track of them!

void get_sensor_feedback() {
	// cache to avoid race conditions ;) (thanks CS33)
	int right = encoders.right();
	int left = encoders.left();
	
	distance_travelled = (right + left) / 2;
	angle_travelled = left - right;
}

void update_motor_pwm() {
	x_error = goal_distance - distance_travelled;
	w_error = goal_angle - angle_travelled;
	
	// If using systick, dt is just your systick time (1ms)
	// Otherwise, have to manually calculate
	pwm_x = KpX * x_error + KdX * (x_error - x_error_old) * dt;
	pwm_w = KpW * w_error + KdW * (w_error - w_error_old) * dt;
	
	x_error_old = x_error;
	w_error_old = w_error;
	
	// Don't go too fast!
	adjust_pwms(&pwm_x, &pwm_w);
	
    // DO THIS LAST! After everything else works,
    // put this in to "finish" the PID
	if (too_slow(pwm_x, pwm_w)) {
		if (has_been_going_slow_for_too_long()) {
			finish_pid();
		}
	}
	else {
		// notice the change in sign!
		motors.set_right_pwm(pwm_x + pwm_w);
		motors.set_left_pwm(pwm_x - pwm_w);
	}
}
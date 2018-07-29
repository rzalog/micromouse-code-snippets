// Start here. All this can be run in systick!
// This will just keep going straight forever.

void get_sensor_feedback() {
	angle_travelled = encoders.left() - encoders.right();
}

void update_motor_pwm() {
	w_error = goal_angle - angle_travelled;
	
	// If using systick, dt is just your systick time (1ms)
	// Otherwise, have to manually calculate
	pwm_w = KpW * w_error + KdW * (w_error - w_error_old) * dt;
	
	w_error_old = w_error;
	
	// Don't go too fast!
	adjust_pwm(&pwm_w);
	
	motors.set_right_pwm(pwm_x + pwm_w);
	motors.set_left_pwm(pwm_x - pwm_w);
}

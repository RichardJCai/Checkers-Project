const float wheelRadius = 22.5;		// mm
const float convert = PI / 180 * wheelRadius;

float angToDist(float angle, float c = convert) {
	return angle * c;
}

float distToAng(float distance, float c = convert) {
	return distance / c;
}

void move(float distance, int power) {
	// do all calculations in terms of degrees
	nMotorEncoder[motorA] = 0;
	distance = distToAng(distance);
	motor[motorA] = power;
	while (nMotorEncoder[motorA] < distance) {}
	motor[motorA] = 0;
	return;
}

void gradMove(float distance, int maxPow, int startPow) {
	int currentPos = 0, power = 0;

	nMotorEncoder[motorA] = 0;
	distance = distToAng(distance);
	float mid = distance / 2;

	while (currentPos < distance) {
		currentPos = nMotorEncoder[motorA];
		power = (-abs(currentPos - mid)/mid + 1) * (maxPow - startPow) + startPow;
		displayString(0, "pos = %d", angToDist(currentPos));
		displayString(1, "powr = %d", power);
		motor[motorA] = power;
	}
	motor[motorA] = 0;
}

void movePD(float distance, float kp, float kd) {
	nMotorEncoder[motorA] = 0;
	distance = distToAng(distance);

	float error = distance, prevError;
	while (nMotorEncoder[motorA] < distance) {
		prevError = error;
		error = distance - nMotorEncoder[motorA];

		displayString(0, "prevErr = %f", prevError);
		displayString(1, "err = %f", error);
		displayString(2, "dEnc = %d", nMotorEncoder[motorA]);
		displayString(3, "prop = %f", kp * error);
		displayString(4, "deriv = %f", kd * (error - prevError));
		displayString(5, "power = %f", kp * error + kd * (error - prevError));

		motor[motorA] = kp * error + kd * (error - prevError);
		wait1Msec(100);
	}
	motor[motorA] = 0;
}

task main()
{
	gradMove(1500, 75, 10);
}

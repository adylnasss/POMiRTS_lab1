
#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	fb_regulator.dt = 0.01f;
	fb_motor.dt = 0.01f;
	fb_motor2.dt = 0.01f;
	fb_motor.ke = 0.02f;
	fb_motor.Tm = 0.08f;
	fb_motor2.ke = 0.002f;
	fb_motor2.Tm = 0.08f;
	fb_regulator.max_abs_value = 50;
	fb_regulator.k_i = 0.80f;
	fb_regulator.k_p = 0.16f;
	fb_regulator.iyOld = 0.0f;
	
	fb_motor.w = 0.0f;
	fb_motor.integrator.out = 0.0f;
	fb_motor2.w = 0.0f;
	fb_motor2.integrator.out = 0.0f;
	counter = 0;
	speed = 0.0f;
	fb_regulator.integrator.out = 0.0f;
	Enable = 1;
}

void motor_regulated() {
	if (!Enable){
		fb_regulator.u = 0.0f;
		fb_regulator.iyOld = 0.0f;
		fb_regulator.integrator.out = 0.0f;
		fb_motor.u = 0.0f;
		FB_Motor(&fb_motor);
	}
	else{
		fb_regulator.e = speed - fb_motor.w;
		FB_Regulator(&fb_regulator);
		fb_motor.u = fb_regulator.u;
		FB_Motor(&fb_motor);
	}
}

void motor_unregulated() {
	if (!Enable) {
		fb_motor2.u = 0.0f;
		fb_motor2.w = 0.0f;
		fb_motor2.phi = 0.0f;
		FB_Motor(&fb_motor2);
		return;
	}
	else {
		fb_motor2.u = speed * fb_motor2.ke;
		FB_Motor(&fb_motor2);
	}
}

void stupench() {
	if (!Enable) {
		counter = 0;
		speed = 0.0f;
	}
	else{
		counter++;
		if (counter < 500)
			speed = 0.0f;
		else if (counter < 1000)
			speed = 800.0f;
		else {
			speed = 0.0f;
			counter = 0;
		}
	}
}

void _CYCLIC ProgramCyclic(void)
{
	stupench();
	motor_regulated();
	motor_unregulated();
}

void _EXIT ProgramExit(void)
{
}


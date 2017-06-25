#include "PID.h"
#include <cmath>
#include <iostream>
using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	PID::Kp = Kp;
	PID::Ki = Ki;
	PID::Kd = Kd;
	p_error = d_error = i_error = 0.0;

	//Twiddle parameters
    dp = {0.1*Kp, 0.1*Ki, 0.1*Kd};
    //dp = {0.1*Kp, 0.1*Ki};
	step = 1;
	param_index = 2;
    //param_index = 1;
	n_settle_steps = 100;
	n_eval_steps = 100;
	total_error = 0;
	best_error =std::numeric_limits<double>::max();
	//tried_adding = false;
	//tried_subtracting = false;
    tried_adding = {false,false,false};
    tried_subtracting = {false,false,false};
    //{tried_adding for Kp,tried_subtracting for Kp,false,false,false,false}
}

void PID::UpdateError(double cte) {

	if (step == 1) {
        p_error = cte;
	    }
    d_error = cte - p_error;
    p_error = cte;
    i_error += cte;
    if (step % (n_settle_steps + n_eval_steps) > n_settle_steps){
        total_error += pow(cte,2);
    }
    cout << "step: " << step << endl;
    cout << "total error: " << total_error << endl;
    cout << "best error: " << best_error << endl;

    //every certain steps twiddle the pid gain
    if (step % (n_settle_steps + n_eval_steps) == 0){
		if (total_error<best_error) {
			//there is improvement!
			best_error = total_error;
            //if it is first time, don't do it
			if (step != n_settle_steps + n_eval_steps) {
			  	dp[param_index] *= 1.1;
			 }
			//tried_adding = tried_subtracting = false;
            tried_adding[param_index]=tried_subtracting[param_index] = false;
        }
        if (!tried_adding[param_index] && !tried_subtracting[param_index]) {
            UpdatePIDGain(param_index, dp[param_index]);
            // next parameter, in lecture twiddle calls err = run(A) however we could not explicitly call run() as we get the error from the simulator, not simple run(), hence we don't loop the param_index
            tried_adding[param_index] = true;
            param_index = (param_index + 1) % 3;
            }
        else if (tried_adding[param_index] && !tried_subtracting[param_index]) {
            UpdatePIDGain(param_index, -2 * dp[param_index]);
            tried_subtracting[param_index] = true;
            // next parameter
            param_index = (param_index + 1) % 3;
            }
        else {
            // p[i] = p[i];
            UpdatePIDGain(param_index, dp[param_index]);
            dp[param_index] *= 0.9;
            tried_adding[param_index] = tried_subtracting[param_index] = false;
            // next parameter
            param_index = (param_index + 1) % 3;
            
        }

		  total_error = 0;
		  cout << "new parameters" << endl;
		  cout << "P: " << Kp << ", I: " << Ki << ", D: " << Kd << endl;
	  }

	  step++;




}

double PID::TotalError() {
	return total_error;
}
void PID::UpdatePIDGain(int index, double amount) {
    if (index == 0) {
        Kp += amount;
    }
    else if (index == 1) {
        Ki += amount;
    }
    else if (index == 2) {
        Kd += amount;
    }
    else {
        std::cout << "AddToParameterAtIndex: index out of bounds";
    }
}

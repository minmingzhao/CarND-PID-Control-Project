# CarND-Controls-PID
This is my PID project for Term 2 Self-Driving Car Engineer Nanodegree Program. This project is to tune PID (proportional, integral, derivative) gains to control the vehicle steering angle and throttle according to CTE (cross track error) based on course-provided [term2 simulator](https://github.com/udacity/self-driving-car-sim/releases). 

## Explanation of PID
### Describe the effect each of the P, I, D components had in your implementation.
P gain, or proportional gain, is the most effective gain when monitoring the its effect. It is proportional to the present values of the error. For example, if the error is large and positive, the control output will also be large and positive. The watchout of P gain is that it does not account for the bias, and may lead to overshoot. 

I gain, or integral gain, is to acount for the past values of the error. It is to avoid bias in PD controller. For example, if the current output is not sufficiently strong, the integral of the error will accumulate over time, and the controller will respond by applying a stronger action.

D gain, or derivative gain, is to account for the trend of the error based on its current rate of change, to help reduce the overshoot introduced by P gain. 

### Describe how the final hyperparameters were chosen.
I believe the initial tuning has to be manual so as to have a valid CTE from the simulator. Firstly I only focus on the steering value and leave throttle value at default 0.3. With basic understanding of the effects of P,I,D, we firstly identify whether if P gain is too large or too small based on the effectiveness of the steering correction. Then add some D gain to avoid the large overshoot. 

Then I use twiddle to fine tune the parameter. I realized I could not directly implement twiddle algorithm from Prof. Thrun's course as I don't have the explicit `err = run()` function. Hence some tweak is needed in the actual twiddle implementation. I select 100 steps for tuning the twiddle, and let another 100 steps for evaluation of the tuning while we acculmulate the total_error. However the initial manual tuning plays an important role. Final P_gain = 0.11, I_gain = 0, D_gain = 3.185. 

Also I implemented PID for throttle control. Apparently the throttle should be symmetric to either positive or negative CTE, since either too left or too right should both brake the vehicle. Hence I take `fabs(cte)` instead of normal `cte` as an input to the `UpdateError` function. Ideally we may need to have separate `UpdateError` function for throttle but actually here it is not too bad on simulator. The final throttle P_gain = 0.136, I_gain = 0, D_gain = 0.0728. 

The final optimized PID control could be like the [final optimized video](https://github.com/minmingzhao/CarND-PID-Control-Project/blob/master/result_movie/optimized_PID_60-80mph-1lap-480p.mov). 

I also studied the effect of having different / wrong PID on this simulator: 
[Large_P_gain.mov](https://github.com/minmingzhao/CarND-PID-Control-Project/blob/master/result_movie/Large_P_gain.mov): big overshoot observed if large P_gain (=1) presented. 

[VeryLarge_D_gain.mov](https://github.com/minmingzhao/CarND-PID-Control-Project/blob/master/result_movie/VeryLarge_D_gain.mov): will constantly change steering wheel which leads to very low vehicle speed and almost no effective steering action. 

[Only_PI_Gain.mov](https://github.com/minmingzhao/CarND-PID-Control-Project/blob/master/result_movie/Only_PI_Gain.mov): no D gain will constantly overshoots. 

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

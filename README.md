# cpp_training

## Instructions

1. Clone the project repo
```
git clone https://github.com/thinhong/cpp_training.git
```

2. Change current directory into the project folder, build an executable file using `make`
```
cd cpp_training/
make
```

3. Run the executable file
```
./DiscreteTimeModel
```

A message will appear asking you to set path to input file, output folder and output file name.
```
Enter path to input file: 
Set path to the folder you want to save output file (ex: /home/Documents): 
```
* Path can be absolute path or relative path
* The output folder will be created if it is not existed
* The output will be saved as `output.csv` in your given output folder
* If `output.csv` already exists in the output folder, it will be overwritten 

## Config file
```
{
    "daysFollowUp": 10,
    "errorTolerance": 0.01,
    "timeStep": 1,
    "initialValues": {"S": 999, "I": 1, "R": 0, "V": 0},
    "parameters": {"beta": 1.2, "N": 1000},
    "transitions": {
        "S -> 0.3 * I": {"distribution": "mathExpression", "expression": "beta * S * I / N"}, 
        "S -> 0.7 * V": {"distribution": "frequency", "frequency": 2},
        "V -> R": {"distribution": "exponential", "rate": 1.5}, 
        "I -> R": {"distribution": "gamma", "scale": 2, "shape": 3}
    }
}
```
* `daysFollowUp`: suppose the unit of time is days, this define the number of days for follow-up
* `errorTolerance`: a float number that determine when a probability reach (1 - errorTolerance) it will be rounded to 1.0
* `timeStep`: if a day is binned into 100 smaller time bins, a time step will be 0.01 day, define here `"timeStep": 0.01`
* `initialValues`: number of individuals in a compartment at the first time step
* `parameters`: if we define a compartment transition using math expression, define all the parameters here
* `transitions`:
    * `S -> I`: compartment S transitions to compartment I
    * `S -> I` then `S -> V`: compartment S transitions to compartment I, then the rest of S transitions to I
    * `S -> 0.3 * I`, `S -> 0.7 * V`: S concurrently transitions to I and V, of which 30% moves to I and 70% moves to V
* `distribution`: distribution of waiting time, could be one of the followings
    * `gamma`: parameters are `scale` and `shape`
    * `weibull`: parameters are `scale` and `shape`
    * `exponential`: parameter is `rate`
    * `mathExpression`: put any math expression to the `mathExpression` parameter, the expression can contains compartment names and other parameters that have been predefined in `parameters`
    * `frequency`: parameter is `frequency`, set a fixed number of individuals moving per time step
    * `transitionProb`: parameter is `transitionProb`, a fix rate transition moving per time step
    * `custom`: parameter is `waitingTime`, put a vector contains the waiting time distribution, the values can be percentages or frequencies, for example `{"distribution": "custom", "waitingTime": [1, 2, 3, 4]}`

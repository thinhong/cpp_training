## All parameters
1. `daysFollowUp`: number of days follow-up = length of the total vector (sum of all subCompartmentValues)
2. `populationSize`: sum of S, I, R, 
3. `transRate`: 
4. `errorTolerance`: 
5. `infectiousComps`: 

## Compartment

Each compartment should be constructed inside a couple of braces `{}` with 6 parameters:
* `name`: name of this compartment
* `distribution`: distribution of this compartment, details below
* `initialValue`: number of individuals within this compartment at the initial stage (t = 0)
* `linkedCompartment`: a vector of name of input or output compartments linked to this compartment
* `isIn`: a vector of boolean value corresponding to each compartment in the linkedCompartment above, to determine that corresponding compartment is input or output compartment (`true` = input compartment, `false` = output compartment) 
* `linkedWeight`: a vector of numbers indicate the "weight" of the corresponding compartment, view example below

**Example**
```
{
    "name":"E",
    "distribution":{"name":"weibull","scale":4.0,"shape":3.0},
    "initialValue":0.0,
    "linkedCompartment":["S","A"],
    "isIn":[true,false],
    "linkedWeight":[1.0,1.0]
}
```
This `S -> E -> A`

```
{
    "name":"A",
    "distribution":{"name":"gamma","scale":1.0,"shape":1.0},
    "initialValue":0.0,
    "linkedCompartment":["E","A_r","I"],
    "isIn":[true,false,false],
    "linkedWeight":[1.0,0.35,0.65]
}
```

## Distribution
### 1. Bernoulli distribution
The first and last compartments must be set with Bernoulli distribution.

**Parameters**
```
"name":"bernoulli","successRate":0.0
```

### 2. Gamma distribution 

**Parameters**
```
"name":"gamma","scale":1.0,"shape":1.0
```

### 3. Weibull distribution

**Parameters**
```
"name":"weibull","scale":1.0,"shape":1.0
```

### 4. Exponential distribution

**Parameters**
```
"name":"exponential","rate":1.0
```

### 5. Custom distribution
#### 5.1. One probability value
This option allows user to set one value of probability (the simulation process is similar to `eular` method in `deSolve` package).

**Parameters**
```
"name":"custom","transitionProb":[0.005]
```

#### 5.2. Cumulative probabilites
This option reads a vector of probabilities and treats them as cumulative probabilities.

**Parameters**
```
"name":"custom","transitionProb":[0.005,0.05,0.5,1.0]
```
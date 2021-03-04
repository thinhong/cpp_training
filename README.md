# cpp_training

Steps to try out the project

1. Clone the project repo. Noted that the size is quite large due to the .git folder since I committed and debug many times and it logs all the history. The program itself is not large. You can download the zip file instead because it does not contain the .git folder.
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
Lam suggested us to change the way to set path to input parameters file and output results, now you can type them with keyboard. After running the executable file with `./DiscreteTimeModel`, a message will appear asking you to set path to input file, output folder and output file name.
```
Enter full path to input file (ex: /home/Documents/config.json): 
Set path to the folder you want to save output file (ex: /home/Documents): 
Set your output file name (ex: results.csv): 
```
An example `config.json` is available in the `config` folder.
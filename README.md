# my_grep
Imitating the grep command within the Linux OS. This C program can take multiple files and search for a pattern that you provide. 
It searches each file using a thread making the program multi threaded. 
Say you provide 3 files and want to seach the pattern "Tasty". The program will create 3 threads.
Each thread will individually look through a file each. It will output the lines that contain the pattern to stdout.
Each thread will record how many lines it read and add a to a counter that will count the total number of times that pattern was met throughout every file. 
So if it found "Tasty" 3 times in each file it will relay that info the stdout and also increment a counter to 9 for the total number of times it saw that pattern
across the 3 files.

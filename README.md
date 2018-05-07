This is a project I made for fun after my first C class in school. It's pretty cool, but I wasted a lot of memory! (oops) Perhaps one day I will re-implement this a little better in c++.

# RhymeSchemer
Markov Chain based rhyming poetry generator

(1.) Set up
Download all the files ( c source, bible, dictionary ) and keep track of the directory where you put them.
Open up the .c file in your text editor of choice
Edit the macros "dictionary" and "engdoc" to point to the dictionary and bible files respectively.
Save the file and close it.

(2.) Compile
Compile the .c file by your method of choice. ( gcc RhymeSchemer.c -o rhymeschemer )
I have compiled without errors with netbeans on my PC and on the
linux machines at my school.

(3.) Run
Run it like any other c program ( ./rhymeschemer )
It should take a few seconds (<15) to load in the files and then prompt you "p for poem , q for quit: "
Type p and then enter
Enter the number of lines you want the poem to be - for instance a limmerick has 9 lines
For each line enter the syllables, and the rhyme word. - for instance "10 duck" would
produce a 10 syllable line that rhymes with duck.

Either enter p to generate another poem, or q to quit.

(4.) Sample run
p for poem , q for quit: p
How many lines should the poem be? (<= maxlines)  : 9
Line 1, Syllables then rhyme word: 

1 duck

Line 2, Syllables then rhyme word: 

2 duck

Line 3, Syllables then rhyme word: 

3 duck

Line 4, Syllables then rhyme word: 

4 duck

Line 5, Syllables then rhyme word: 

5 duck

Line 6, Syllables then rhyme word: 

6 duck

Line 7, Syllables then rhyme word: 

7 duck

Line 8, Syllables then rhyme word: 

8 duck

Line 9, Syllables then rhyme word: 

9 duck

STRUCK 

AND STRUCK 

THE EUNUCH 

ALSO WILL PLUCK 

TO SEE THE SON SUCK 

LIE NOT BEEN AN EUNUCH 

HER AND SAY WHEN I WILL PLUCK 

AND SHALL SCOFF AT ONCE BUT ZADOK 

AWAY THE COMMON BREAD AND BARAK 

p for poem , q for quit: q


(5.) Potential Errors
"Error opening file" - Make sure the macro dictionary points to the dictionary.txt file
"Error opening engdoc file" - same as above but for engdoc

Program seems to be in infinite loop or exits on segfault - Sorry, this is due to the obscene memory usage.
I will fix this in the next release.


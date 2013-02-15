##Spellchecker
An interactive spellchecker written in c++.  It fixes any combination of the following spelling mistakes:

1. Upper and lower case mixins
2. Repeated letters
3. Incorrect vowels

##Compile with
g++ -Wall -o spellcheck spellcheck.cpp

##Run with
./spellcheck /usr/share/dict/words

##Sample Output

```
> shoooop
shop
> griiivoty
gravity
> JamPIng
jumping
> shoasdfasdf
NO SUGGESTION
```

An alternate solution can be found at https://github.com/namtsui/spellcheck/blob/master/src/spellcheck.cc


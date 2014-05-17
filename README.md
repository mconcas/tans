TANS
====

This repository is a collection of classes and macros for a vertexer
simulation. These are going to run using the
[ROOT](http://root.cern.ch/drupal) framework for data analysis.

Below I will take note the of various standard definitions, simulation
structures, as a reminder for myself and a read-key for eventually
observers.

Standard declarations
---------------------

*   Indentation 3 blank spaces.
*   Ruler: 80 chars
*   Idioms: ClassName, Methods, Stdoutput: EN.
               Comments, Function arguments: EN


Rationale: Class hierarchy
--------------------------

The structure is thought to be object-oriented and coded in C++.
Thus one can easily benefit of an higher level approach to the problem.
Instead to wrote a conspicuous set of macros, hardly customizable (just editing
their code itself) and with a lower granularity, I've opted for an open modular
structure. It has multiple positive aspects, specially in debug phase, when bug
arises.

TANS
====

This repository is a collection of classes and macro for a vertexer 
simulation. These are going to run using the
[ROOT](http://root.cern.ch/drupal) framework for data analysis. 

Below I will take note the various standard definitions, simulation 
structures, as a reminder for myself and a read-key for eventually 
observers.

Standard declarations
---------------------

*   Indentation 3 blank spaces.
*   Ruler: 70 chars
*   Languages: ClassName, Methods, Stdoutput: IT.
               Comments, Function arguments: EN


Rationale: Class hierarchy
--------------------------

The main goal is to create a sort of «backbone» consisting in a set of
trivial classes like *Punto Class, Direzione Class, whatever...* in 
order to result as versatile as possible.

From there will be derived physics classes  (e.g. *Vertice Class*), 
keeping in mind that this is a quite trivial example, so a nice 
purposal could be to structure the code thinking in building modular 
classes and physics «wrapper» easily customizable.




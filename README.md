TANS
====

This repository is a classes and macro collection for a trivial vertex 
simulation. These are supposed to run with 
[ROOT](http://root.cern.ch/drupal), the [CERN](https://cern.ch) 
framework for data analysis.

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

The main idea is to create a geometrical backbone, intruducing almost
trivial classes as *Punto Class* and more in order to remain as 
versatile as possible.

From there physics classes will be derived (e.g. *Vertice Class*), 
keeping in mind that this is a quite trivial example, so a purposal 
could be to structure the code and the topology thinking to build 
modular classes and «physics» wrapper easily customizable.




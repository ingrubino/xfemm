% femm_manual_tutorial_example.m
%
%
% This script recreates the Magnetostatics example provided in the FEMM 4.2
% Users manual using mfemm functions. 
%
% In this example, the solution for the field of an air-cored coil is
% considered. 
%
% The coil to be analyzed is show in the diagram below:
%
%                                     1 inch
%                               <----------------->                              
%                                        � 
%                                        �
%                                        x    
%                                  x     �     x                             
%                              x         �         x                         
%                           x           x�x           x                        
%                         x         x    �    x         x                       
%                       x        x       �       x        x                      
%                      x_______ x        �        x _______x                     
%                      x       |         �         |       x     ^                
%                      x       |x        �        x|       x     �                
%                      |x      | x       �       x |      x|     �                
%                      |  x    |    x    �    x    |    x  |     �                
%                      |    x  |   .    x�x   .    |  x    |     �                
%                      |       x         �         x       |     � 2 inch        
%                      |    .  |   x    .�.    x   |  .    |     �             
%                      |  .    |    .    x    .    |    .  |     �              
%                      |.      | .       �       . |      .|     �               
%                      |       |.        �        .|       |     �                
%                      |_______|         �         |_______|     v                   
%                      x        .        �        .        x                      
%                       x        .       �       .        x                       
%                         x         .    �    .         x                         
%                           x           .�.           x                            �                                     
%                              x         �         x                           
%                                  x     �     x                               
%                                        x                                     
%                                        �                                  
%                                        �                                  
%                                        �                                  
%                      <----------------------------------->
%                                     3 inch
%
% The coil has an inner diameter of 1 inch; an outer diameter of 3 inches;
% and an axial length of 2 inches. The coil is built out of 1000 turns of
% 18 AWG copper wire. For the purposes of this example, we will consider
% the case in which a steady current of 1 Amp is flowing through the wire.
% In FEMM (and therefore mfemm), one models a slice of the axisymmetric
% problem. By convention, the r = 0 axis is understood to run vertically,
% and the problem domain is restricted to the region where r ? 0 . In this
% convention, positive-valued currents flow in the into-the-page direction.
%
% 

% Create a new mfemm problem structure
%
% newproblem_mfemm creates a new FemmProblem structure for the type of
% problem specifiec in the first argument. This argument is either a string
% or scalar value. If a string it can be either 'planar' or
% 'axisymmetric', or any unambiguous shorter starting substring, e.g.
% 'axi','pl' or even just 'a' or 'p'. If a scalar value, the value zero
% chooses a planar type simulation. Any other value selects an axisymmetric
% simulation. Here we use 'axi' to choose and axisymmetric type.
%   
% We also set the problem frequency to zero and the length units to be
% inches using optional parameter-value pairs.
FemmProblem = newproblem_mfemm('axi', ...
                               'Frequency', 0, ...
                               'LengthUnits', 'inches');
                           
% The created FemmProblem structure contains two field, ProbInfo,
% containing the problem type specification, and Materials. The materials
% structure contains an array of structures containing the material
% specifications that will be used in the problem. newproblem_mfemm
% automatically puts the material 'Air' in this array by default when
% creating the problem. We will add more materials to this array later in
% the example for use in the model.
disp(FemmProblem)

% The full problem specification 
disp(FemmProblem.ProbInfo)

% The next task is to create boundaries for the solution region.
% Fundamentally, finite element solvers mesh and find a solution over a
% finite region of space that contains the objects of interest. We must
% define what this region is. In this case, we will choose our solution
% region to be a sphere with a radius of 4 inches.

% To create the sphere we need only two nodes which can then be linked by
% a semi-circular arc and a straight segment to make a semicircle. 

% first define the node locations, which are above and below the centerline
% of the cylindrical coil.
outernodes = [0,  4; 
              0, -4 ];
          
% Now add these nodes to the FemmProblem structure using the function
% addnodes_mfemm. addnodes_mfemm is typical in that it can return up to
% three arguments. The first of these is the modified FemmProblem structure
% with the new nodes added. The second argument is a vector containning the
% matlab array indices of the created nodes in an array of node structures,
% while nodeids are the zero-based node ids which are always equal to the
% indices - 1. Segments are created bewtwenn node ids, which is why we want
% to store them for later use.
[FemmProblem, nodeinds, nodeids] = addnodes_mfemm(FemmProblem, outernodes(:,1), outernodes(:,2));

% Next we add a straght segment between the two nodes we just created
[FemmProblem, nodeinds, nodeids] = addsegments_mfemm(FemmProblem, nodeids(1), nodeids(2));

% And then the arc segment between the same two nodes
[FemmProblem, nodeinds, nodeids] = (FemmProblem, outernodes(:,1), outernodes(:,2));





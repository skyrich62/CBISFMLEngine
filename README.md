# CBISFMLEngine
A game engine for SFML

This is a simplified game engine built up from SFML.  The engine manages game state, drawable and transformable entities, and manages resources.

It requires SFML, at least version 2.5.0.

The central object is the IEntity.  This is an base class for an sf::Drawable and sf::Transformable object.  The user should provide overrides for the following functions:

   * drawThis() -- specialized code to draw the IEntity
   * getLocalBounds() -- Return the Axis-Aligned Boundary Box for this IEntity.
   * updateThis() -- optional.  Perform any specialised update to the IEntity.

IEntity object can have child IEntity objects, (which can have their children and so on).  In this way a set of objects which should be moved, transformed, and drawn
together can be managed easily.  Children are drawn relative to their parent.  Additionally, each IEntity is assigned a z-order which specifies the drawing order.
Lower z-order objects are drawn first.

The engine provides the following IEntity specializations:

   * SpriteEntity - this manages a sprite
   * TextEntity - This manages text
   * CircleEntity - This manages a simple circle or triangle object.
   * RectangleEntity - This manages a simple rectangle
   * ConvexEntity -- This manages a convex shape.
   * ShapeEntity -- This is a base class for the various shape IEntity objects, (Circle, Rectangle, Convex).
   
In addition to the IEntity, there is the ISystem abstract class.  IEntity objects are assigned to any number of ISystem objects.  Each ISystem performs some
function on the IEntity object.  The Engine predefines the following ISystem derived classes:

   * MovementSystem -- This moves IEntity objects by assigning velocity and acceleration, rotational as well as vector movement is handled.
   * CollisionSystem -- This manages collision detection ahd provides for callback functions to handle the detected collisions.
   * DrawingSystem -- This manages drawing the IEntity objects by calling the "draw()" member function.
   
There is an EventManager object which manages SFML events and associated handlers.  Handlers are registered with the EventManager, which automatically
dispatches them when the associated event is detected.

Please see the documentation, (in the header files in Doxygen format), and the example projects for information.
   

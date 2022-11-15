# Design Patterns

## Creational Patterns

Creational design patterns provide various object creation mechanisms, which increase flexibility and reuse of existing code.

### Factory Method

### Abstract Factory

### Builder

### Prototype

### Singleton

## Structual Patterns

Structural design patterns explain how to assemble objects and classes into larger structures, while keeping these structures flexible and efficient.

### Adapter

### Bridge

### Composite

### Decorator

### Facade

### Flyweight

### Proxoy

## Behavioral Patterns

Behavioral design patterns are concerned with algorithms and the assignment of responsibilities between objects.

### Chains of Responsibility

### Command

### Iterator

### Mediator

### Memento

### Observer

### State

### Strategy

### Template Method

### Visitor

Visitor is a behavioral design pattern that lets you separate algorithms from the objects on which they operate. Visitor is a powerful version of the Command pattern. Its objects can execute operations over various objects of different classes.

![Visitor Structure](design_patterns/visitor.png)

1. The **Visitor** interface declares a set of visiting methods that can take concrete elements of an object structure as arguments. These methods may have the same names if the program is written in a language that supports overloading, but the type of their parameters must be different.

2. Each **Concrete Visitor** implements several versions of the same behaviors, tailored for different concrete element classes.

3. The **Element** interface declares a method for accepting visitors. This method should have one parameter declared with the type of the visitor interface.

4. Each **Concrete Element** must implement the acceptance method. The purpose of this method is to redirect the call to the proper visitor’s method corresponding to the current element class. Be aware that even if a base element class implements this method, all subclasses must still override this method in their own classes and call the appropriate method on the visitor object.

5. The **Client** usually represents a collection or some other complex object. Usually, clients aren’t aware of all the concrete element classes because they work with objects from that collection via some abstract interface.

```cpp
class ConcreteElementA;
class ConcreteElementB;

// The Visitor interface declares a set of visiting methods that
// correspond to element classes. The signature of a visiting
// method lets the visitor identify the exact class of the
// element that it's dealing with.
class Visitor {
 public:
  virtual void Visit(const ConcreteElementA*) const {}
  virtual void Visit(const ConcreteElementB*) const {}
};

class ConcreteVisitor: public Visitor {
 public:
  virtual void Visit(const ConcreteElementA* element) const override {
    // Visitor methods know the concrete type of the element
    // it works with.
    element->FeatureA();
  }

  virtual void Visit(const ConcreteElementB* element) const override {
    element->FeatureB();
  }
};

// The element interface declares an `Accept` method that takes
// the base visitor interface as an argument.
class Element {
 public:
  virtual void Accept(const Visitor*) const {}
};

// Each concrete element class must implement the `Accept`
// method in such a way that it calls the visitor's method that
// corresponds to the element's class.
class ConcreteElementA: public Element {
 public:
  void FeatureA() const {}

  virtual void Accept(const Visitor* visitor) const override {
    visitor->Visit(this);
  }
};

class ConcreteElementB: public Element {
 public:
  void FeatureB() const {}

  virtual void Accept(const Visitor* visitor) const override {
    visitor->Visit(this);
  }
};
```

* Use the Visitor when you need to perform an operation on all elements of a complex object structure (for example, an object tree).
* Use the Visitor to clean up the business logic of auxiliary behaviors.
* Use the pattern when a behavior makes sense only in some classes of a class hierarchy, but not in others.
*
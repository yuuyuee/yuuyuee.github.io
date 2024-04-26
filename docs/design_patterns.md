# Design Patterns

[TOC]

## Creational Patterns

Creational design patterns provide various object creation mechanisms, which increase flexibility and reuse of existing code.

### Factory Method

```cpp

class Creator

```

#### Simple Factory (Parametrized Factory Method)

```cpp

```

### Abstract Factory

```cpp

```

#### Parameterized Abstract Factory

```cpp

```

### Builder

```cpp

```

### Prototype

```cpp

```

### Singleton

```cpp

```

## Structual Patterns

Structural design patterns explain how to assemble objects and classes into larger structures, while keeping these structures flexible and efficient.

### Adapter

```cpp

```

### Bridge

```cpp

```

### Composite

```cpp

```

### Decorator

```cpp

```

### Facade

```cpp

```

### Flyweight

```cpp

```

### Proxoy

```cpp

```

## Behavioral Patterns

Behavioral design patterns are concerned with algorithms and the assignment of responsibilities between objects.

### Chains of Responsibility

### Command

```cpp
class Receiver {
 public:
  void Operation(int a, int b, int c) {
    // The Receiver classes contain some important business logic.
    // They know how to perform all kinds of operations, associated with carrying out a request.
  }
};

// The Command interface declares a method for executing a command.
class Command {
 public:
  virtual ~Command() {}

  virtual void Execute() = 0;
};

class ConcreteCommand: public Command {
 public:
  ConcreteCommand(Receiver* receiver, int a, int b, int c)
    : receiver_(receiver), a_(a), b_(b), c_(c) {}
  virtual ~ConcreteCommand() {}

  // Delegate to methods of a receiver.
  virtual void Execute() {
    receiver_->Operation(a_, b_, c_);
  }

 private:
  Receiver* receiver_;
  int a_, b_, c_;
};

class Invoker {
 public:
  Invoker(): command_(nullptr) {}
  ~Invoker() {
    if (command_)
      delete command_;
  }

  void SetCommand(Command* command) {
    command_ = command;
  }

  void ExecuteCommand() {
    if (command_)
      command_->Execute();
  }

 private:
  Command* command_;
};

int main() {
  Receiver receiver;
  Invoker invoker;
  invoker.SetCommand(new ConcreteCommand(&receiver, 1, 2, 3));
  invoker.ExecuteCommand();
  return 0;
}
```

### Iterator

### Mediator

```cpp
class Mediator;

class Component {
 protected:
  Mediator* mediator_;

 public:
  Component(): mediator_(nullptr) {}

  void SetMediator(Mediator* mediator) {
    mediator_ = mediator;
  }
};

class Mediator {
 public:
  virtual ~Mediator() {}
  virtual void Notify(Component*, char i) const = 0;
};

class ConcreteComponentA: public Component {
 public:
  virtual ~ConcreteComponentA() {}

  void Operator() {
    mediator_->Notify(this, 'A');
  }
};

class ConcreteComponentB: public Component {
 public:
  virtual ~ConcreteComponentB() {}

  void Operator() {
    mediator_->Notify(this, 'B');
  }
};

class ConcreteMediator: public Mediator {
 private:
  ConcreteComponentA* ca_;
  ConcreteComponentB* cb_;

 public:
  ConcreteMediator(ConcreteComponentA* ca, ConcreteComponentB* cb)
      : ca_(ca), cb_(cb) {}
  virtual ~ConcreteMediator() {}

  virtual void Notify(Component* c, char i) const {
    if (i == 'A') {
      std::cout << "Notify '" << i << "' from " << c << std::endl;
    }

    if (i == 'B') {
      std::cout << "Notify '" << i << "' from " << c << std::endl;
    }
  }
};

int main() {
  ConcreteComponentA ca;
  ConcreteComponentB cb;
  ConcreteMediator mediator(&ca, &cb);
  ca.SetMediator(&mediator);
  cb.etMediator(&mediator);
  ca.Operator();
  return 0;
}
```

### Memento

```cpp

```

### Observer

```cpp

```

### State

```cpp

```

### Strategy

```cpp

```

### Template Method

```cpp

```

### Visitor

```cpp
class ConcreteElementA;
class ConcreteElementB;

// The Visitor interface declares a set of visiting methods that
// correspond to element classes. The signature of a visiting
// method lets the visitor identify the exact class of the
// element that it's dealing with.
class Visitor {
 public:
  virtual ~Visitor() {}

  virtual void Visit(const ConcreteElementA*) const = 0;
  virtual void Visit(const ConcreteElementB*) const = 0;
};

class ConcreteVisitor: public Visitor {
 public:
  virtual ~ConcreteVisitor() {}

  virtual void Visit(const ConcreteElementA* element) const {
    // Visitor methods know the concrete type of the element
    // it works with.
    element->FeatureA();
  }

  virtual void Visit(const ConcreteElementB* element) const {
    element->FeatureB();
  }
};

// The element interface declares an `Accept` method that takes
// the base visitor interface as an argument.
class Element {
 public:
  virtual ~Element() {}
  virtual void Accept(const Visitor*) const = 0;
};

// Each concrete element class must implement the `Accept`
// method in such a way that it calls the visitor's method that
// corresponds to the element's class.
class ConcreteElementA: public Element {
 public:
  virtual ~ConcreteElementA() {}

  void FeatureA() const {}

  virtual void Accept(const Visitor* visitor) const {
    visitor->Visit(this);
  }
};

class ConcreteElementB: public Element {
 public:
  virtual ~ConcreteElementB() {}

  void FeatureB() const {}

  virtual void Accept(const Visitor* visitor) const {
    visitor->Visit(this);
  }
};
```

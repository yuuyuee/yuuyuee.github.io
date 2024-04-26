# Design Patterns

[TOC]

## Creational Patterns

Creational design patterns provide various object creation mechanisms, which increase flexibility and reuse of existing code.

### Factory Method

```cpp
// 分离类的构造与使用
class Product {
 public:
  virtual ~Product() {}

  virtual void Operation() const = 0;
};

class ConcreteProduct: public Product {
 public:
  virtual ~ConcreteProduct() {}

  virtual void Operation() const {}
};

class Creator {
 public:
  virtual ~Creator() {}

  virtual Product* CreateProduct() const = 0;
};

class ConcreteCreator: public Creator {
 public:
  virtual ~ConcreteCreator() {}

  virtual Product* CreateProduct() const { return new ConcreteProduct; }
};

void Client(Creator* creator) {
  Product* product = creator->CreateProduct();
  product->Operation();
}

int main() {
  ConcreteCreator creator;
  Client(&creator);
  return 0;
}

```

#### Simple Factory (Parametrized Factory Method)

```cpp

class SimpleFactory {
 public:
  Product* CreateProduct(int id) {
    switch(id) {
      case 0: return new ConcreateProduct;
      default: raise InvalidArgument();
    }
  }
};

int main() {
  SimpleFactory creator;
  Product* product = creator.Create(0);
  product->Operation();
  return 0;
}

```

### Abstract Factory

```cpp

```

### Builder

```cpp
// 分离类的复杂构造过程
class Builder {
 public:
  virtual ~Builder() {}

  virtual void Reset() = 0;
  virtual void SetupA() = 0;
  virtual void SetupB() = 0;
};

class ConcreteBuilder: public Builder {
 public:
  virtual ~ConcreateBuilder() {}

  virtual void Reset();
  virtual void SetupA();
  virtual void SetupB();

  Product* GetProduct();
};

class Director {
 public:
  Director(Builder* builder): builder_(builder) {}

  void SetBuilder(builder* builder) { builder_ = builder; }
  void Make(int type) {
    switch (type) {
      case 0: builder_->SetupA(); break;
      default: raise InvalidArgument();
    }
  }

  Product* GetProduct() { return builder_->GetProduct(); }
 private:
  Builder builder_;
};

int main() {
  ConcreateBuilder builder;
  Director director(&builder);
  director.Make(0);
  Product* product = builder.GetProduct();
  product->Operation();

  return 0;
}

```

### Prototype (Clone)

```cpp
// 复制已有的对象来创建对象，生成构造代价大的类的场景
class Prototype {
 public:
  virtual ~Prototype() {}

  virtual Prototype* Clone() = 0;
};

class ConcreatePrototype: public Prototype {
 public:
  virtual ~ConcreatePrototype() {}
  ConcreatePrototype(Prototype* ptr) {}

  virtual Prototype* Clone() { return new ConcreatePrototype(this); }
};

class PrototypeRegistry {
 public:
  void AddItem(int id, Prototype* ptr);
  Prototype* GetById(int id);
};

int main() {
  ConcreatePrototype* ptr = new ConcreatePrototype(nullptr);
  PrototypeRegistry registry;
  registery.AddItem(0, ptr);
  ConcreatePrototype* copy = registery.GetById(0).Clone();
  return 0;
}

```

### Singleton

```cpp
template <typename Tp>
class Singleton {
 public:
  static Tp* Instance() {
    std::call_once(flag, Init);
    return ptr;
  }

 private:
  static void Init() { ptr = new Tp; }
  static std::once_flag flag;
  static Tp* ptr;
};

template <typename Tp>
std::once_flag Singleton<Tp>::flag;

template <typename Tp>
Tp* Singleton<Tp>::ptr = nullptr;

```

## Structual Patterns

Structural design patterns explain how to assemble objects and classes into larger structures, while keeping these structures flexible and efficient.

### Adapter (Wrapper)

```cpp
// 让不兼容的接口能够互相合作

class Interface {
 public:
  virtual ~Interface() {}

  virtual void Operation(void* data) = 0;
};

class Service {
 public:
  void SpecialOperation(void* special_data);
};

class Adapter: public Interface {
 public:
  virtual ~Adapter() {}

  virtual void Operation(void* data) {
    void* special_data = ConvertToServiceFormat(data);
    adaptee.SpecialOperation(special_data);
  }

 private:
  Service adaptee;
};

```

### Bridge

```cpp
// 可将一个大类或一系列紧密相关的类拆分为抽象和实现两个独立的层次结构， 从而能在开发时分别使用

class Device {
 public:
  virtual ~Device() {}

  virtual bool IsEnabled() const = 0;
  virtual void Enable() = 0;
  virtual void Disable() = 0;
  virtual int GetVolume() const = 0;
  virtual void SetVolume(int percent) = 0;
  virtual int GetChannel() const = 0;
  virtual void SetChannel(int channel) = 0;
};

class TV: public Device {
 public:
  virtual ~Ratio() {}

  virtual bool IsEnabled() const;
  virtual void Enable();
  virtual void Disable();
  virtual int GetVolume() const;
  virtual void SetVolume(int percent);
  virtual int GetChannel() const;
  virtual void SetChannel(int channel);
};

class Remote {
 public:
  Remote(Device* device): device_(device) {}

  void TogglePower() {
    if (device_->IsEnabled())
      device_->Disable();
    else
      device_->Enable();
  }

  void VolumeDown() {
    int volume = device_->GetVolume();
    device_->SetVolume(volume == 0 ? 0 : (volume - 1));
  }

  void VolumeUp() {
    int volume = device_->GetVolume();
    device_->SetVolume(volume == 0 ? 0 : (volume + 1));
  }

  void ChannelDown() {
    int channel = device_->GetChannel();
    device_->SetChannel(channel == 0 ? MAX_CHANNEL : (channel - 1));
  }

  void ChannelUp() {
    int channel = device_->GetChannel();
    device_->SetChannel(channel == 0 ? MAX_CHANNEL : (channel + 1));
  }

 protected:
  Device* device_;
};

class AdvancedRemote: public Remote {
 public:
  AdvancedRemote(Device* device): Remote(device) {}

  void Mute() {
     device_->SetVolume(0);
  }

  void CCTV_6() {
    device_->SetChannel(CCTV_6_CHANNEL_ID);
  }
};

int main() {
  TV tv;
  AdvancedRemote remote(&tv);
  remote.CCTV_6();
  return 0;
}

```

### Composite (Object tree)

```cpp
// 可以使用它将对象组合成树状结构， 并且能像使用独立对象一样使用它们

class Graph {
 public:
  virtual ~Graph();

  virtual void Move(int x, int y) = 0;
  virtual void Draw() = 0;
};

class Dot: public Graph {
 public:
  virtual ~Dot() {}

  virtual void Move(int x, int y);
  virtual void Draw();
};

class Line: public Dot {
 public:
  virtual ~Line() {}

  virtual void Move(int x, int y);
  virtual void Draw();
};

class CompoundGraph: public Graph {
 public:
  virtual ~CompoundGraph() {}

  void Add(Graph* graphic);
  void Remove(Graph* graphic);

  virtual void Move(int x, int y);
  virtual void Draw();

 private:
  std::vector<Graph*> graphics_;
};

int main() {
  CompoundGraph compound_graph;
  Dot dot;
  Line line;
  compound_graph.Add(&dot);
  compound_graph.Add(&line);
  compound_graph.Move(0, 0);
  compound_graph.Draw();
  return 0;
}

```

### Decorator (Wrapper)

```cpp
// 允许你通过将对象放入包含行为的特殊封装对象中来为原对象绑定新的行为

class Stream {
 public:
  virtual ~Stream() {}

  virtual void Write(void* data) = 0;
  virtual void* Read() = 0;
};

class FileStream: public Stream {
 public:
  FileStream(const char* filename);
  virtual ~FileStream() {}

  virtual void Write(void* data);
  virtual void* Read();
};

class StreamDecorator: public Stream {
 public:
  StreamDecorator(Stream* stream): wrappee_(stream) {}
  virtual ~StreamDecorator() {}

  virtual void Write(void* data) { wrappee_->Write(data); }
  virtual void* Read() { return wrappee_->Read(); }

 private:
  Stream* wrappee_;
};

class EncryptionStream: public StreamDecorator {
 public:
  EncryptionStream(stream* stream): StreamDecorator(stream) {}
  virtual ~EncryptionStream() {}

  void* Encrypt(void* data);
  void* Decrapt(void* data);

  virtual void Write(void* data) {
    void* data = Encrypt(data);
    StreamDecorator::Write(data);
  }

  virtual void* Read() {
    void* data = StreamDecorator::Read();
    return Decrapt(data);
  }
};

```

### Facade

```cpp
// 为程序库、 框架或其他复杂类提供一个简单的接口

```

### Flyweight  (Cache)

```cpp
// 摒弃了在每个对象中保存所有数据的方式， 通过共享多个对象所共有的相同状态， 让你能在有限的内存容量中载入更多对象

```

### Proxoy

```cpp
// 能够提供对象的替代品或其占位符。 代理控制着对于原对象的访问， 并允许在将请求提交给对象前后进行一些处理
// 代理类拥有被代理类的控制权已经生命周期管理

class Stream {
 public:
  virtual ~Stream() {}

  virtual void Write(void* data) = 0;
  virtual void* Read() = 0;
};

class FileStream: public Stream {
 public:
  FileStream(const char* filename);
  virtual ~FileStream() {}

  virtual void Write(void* data);
  virtual void* Read();
};

// proxy class
class EncryptionStream: public Stream {
 public:
  EncryptionStream(stream* stream): stream_(stream) {}
  virtual ~EncryptionStream() {}

  void* Encrypt(void* data);
  void* Decrapt(void* data);

  virtual void Write(void* data) {
    void* data = Encrypt(data);
    stream_->Write(data);
  }

  virtual void* Read() {
    void* data = stream_->Read();
    return Decrapt(data);
  }

 private:
  Stream* stream_;
};

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

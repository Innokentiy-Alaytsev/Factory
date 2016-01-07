# Simple Factory Library

Classes in this library are used for building objects factories.

Usage algorithm is as follows:
  1. Declare interface that would be implemented by all classes for which it's desired to create
  instatces with use of factory. This interface must extend IProducible.

  2. Instantiate Factory class right after declaration of interface using interface as template
  argument:
  ```
  class NewInterface:
	  public IProducible {
  public:
	template< class NewInterface > static std::shared_ptr< NewInterface > createObject();
	static QString typeId ();

	virtual void someNewMethod () = 0;
	virtual void anotherNewMethod () = 0;
  }

  template class Factory< NewInterface >;
  ```

  3. Define classes that implements desired interface (NewInterface in example).

  4. Each of defined classes must be registered as produced by factory. Registration is performed
  by deffinition of variable of type AddProduction somewhere in class implementation file, for
  convinience, at the beginning of this file:
  SomeClass.h:
  ```
  class SomeClass:
	  public NewInterface {
  public:
	template< class NewInterface > static std::shared_ptr< NewInterface > createObject();
	static QString typeId ();

	virtual void someNewMethod () override;
	virtual void anotherNewMethod () override;
  }
  ```
  SomeClass.cpp:
  ```
  #include "SomeClass.h"

  AddProduction< NewInterface, SomeClass > addSomeClass;

  SomeClass::SomeClass() {
	// ...
  }

  // ...
  ```

  Described approach to classes registration and to overall factory implementation was choisen
  in order to simplify programm structure and to eliminate the need in any king of common factory
  initialization function. If such common function exists than user should edit it upon addition of
  every new producible class. An approach that does not need any kind of such common point seems
  to be better.

  Provided implementation is almost static, which means that it's imposible to add new classes in
  runtime. However there are a lot of use cases where there is no need for runtime classes addition.
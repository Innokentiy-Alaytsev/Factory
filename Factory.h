/*
  The MIT License (MIT)

  Copyright (c) 2016 Innokentij Alajcev (Иннокентий Алайцев)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef FACTORY_H
#define FACTORY_H

#include <memory>

#include <typeinfo>


/*!
  @defgroup Factory Objects factory

  Classes in this group are used for building objects factories.

  Usage algorithm is as follows:
  1. Declare interface that would be implemented by all classes for which it's desired to create
  instatces with use of factory. This interface must extend IProducible.

  2. Instantiate Factory class right after declaration of interface using interface as template
  argument:
  @code{.cpp}
  class NewInterface:
	  public IProducible {
  public:
	template< class NewInterface > static std::shared_ptr< NewInterface > createObject();
	static QString typeId ();

	virtual void someNewMethod () = 0;
	virtual void anotherNewMethod () = 0;
  }

  template class Factory< NewInterface >;
  @endcode

  3. Define classes that implements desired interface (NewInterface in example).

  4. Each of defined classes must be registered as produced by factory. Registration is performed
  by deffinition of variable of type AddProduction somewhere in class implementation file, for
  convinience, at the beginning of this file:
  SomeClass.h:
  @code{.cpp}
  class SomeClass:
	  public NewInterface {
  public:
	template< class NewInterface > static std::shared_ptr< NewInterface > createObject();
	static QString typeId ();

	virtual void someNewMethod () override;
	virtual void anotherNewMethod () override;
  }
  @endcode
  SomeClass.cpp:
  @code{.cpp}
  #include "SomeClass.h"

  AddProduction< NewInterface, SomeClass > addSomeClass;

  SomeClass::SomeClass() {
	// ...
  }

  // ...
  @endcode

  Described approach to classes registration and to overall factory implementation was choisen
  in order to simplify programm structure and to eliminate the need in any king of common factory
  initialization function. If such common function exists than user should edit it upon addition of
  every new producible class. An approach that does not need any kind of such common point seems
  to be better.

  Provided implementation is almost static, which means that it's imposible to add new classes in
  runtime. However there are a lot of use cases where there is no need for runtime classes addition.

  @{
*/


/**
  @def FACTORY_USE_STD_LIB

  Use to enable std types usage by factory.
*/
//#define FACTORY_USE_STD_LIB

/**
  @def FACTORY_USE_QT

  Use to enable Qt types usage by factory.
*/
#define FACTORY_USE_QT

/**
  @def FACTORY_USE_STD_LIB

  Use to enable usage of variadic templates by factory. This make possible deffinition of object
  creation function with any nubber of arguments.
*/
//#define FACTORY_USE_VARIADIC_TEMPLATES

#ifdef FACTORY_USE_STD_LIB
	#include <cassert>

	#include <map>
	#include <string>

	#include <iostream>

	#define StringType std::string
	#define DictType std::map
	#define warningOutput std::cerr
	#define factoryAssert assert
#else
	#ifdef FACTORY_USE_QT
		#include <QHash>
		#include <QString>

		#include <QDebug>

		#define StringType QString
		#define DictType QHash
		#define warningOutput qWarning ()
		#define factoryAssert Q_ASSERT
	#endif
#endif


namespace Factory {
	/*!
	  @brief Objects factory.

	  @details Used to create objects of different classes that implement interface Interface.

	  Class for objects production must be registerent before any attempt to create objects. Classes are
	  registered in factory by deffinition of variable of type AddProduction instantiated with the right
	  arguments.

	  @note It is recommended to create registering AddProduction objects at the beginning of
	  implementation file of each class.

	  @see AddProduction
	*/
	template< class Interface > class Factory {
	public:
		/*!
		  @brief Factory method for objects cteation.

		  @details Used to crate objects of registered types.

		  @param [in] i_type_id Unique identifier of type of object to be created.

		  @returns Smart pointer of type std::shared_ptr for created object. If type identifier is
		  unknown to factory, then nullptr is returned.
		*/
#ifdef FACTORY_USE_VARIADIC_TEMPLATES
		template< class ...Arguments >
		static std::shared_ptr< Interface > createObject (
			QString i_type_id,
			Arguments... io_args) {
			std::shared_ptr< Interface > object (nullptr);

			if (_produce_functions.contains(i_type_id)) {
				object = _produce_functions[i_type_id](io_args...);
			}

			return object;
		}
#else
		static std::shared_ptr< Interface >
		createObject (StringType i_type_id) {
			std::shared_ptr< Interface > object (nullptr);

			if (produceFunctions ().count (i_type_id)) {
				object = produceFunctions () [i_type_id]();
			}

			return object;
		}
#endif

	private:
		/*!
		  @brief Constructor is private in order to prevent objects creation.
		*/
		Factory();
		/*!
		  @brief Constructor is private in order to prevent objects creation.
		*/
		Factory(Factory&);

		/*!
		  @brief List of functiontions used to create objects.

		  @details Contains a list of pairs of unique identifier of class and object creation function.
		  Used in createObject

		  The field is incapsulated within a function in order to prevent problems with initialization
		  order of variables, etc.
		*/
		static DictType<
				StringType,
				decltype (&Interface::createObject) >& produceFunctions () {
			static DictType<
				StringType,
				decltype (&Interface::createObject) > _produce_functions;

			return _produce_functions;
		}

		/*!
		  @brief Friend class for producible types registration.
		*/
		template < class Interfase, class Production > friend class AddProduction;
	};


	/*!
	  @brief Class for types registration in factory.

	  @details Used to register types for objects production by Factory.

	  Template arguments are:
	  - Interface - common interface implemented by classes which instances are produced by factory.
	  Used for identification of factory for which class is registered.
	  - Production - class implementing Interface interface, objects of this class will be produced by
	  factory. This class must implement IProducible.

	  Classes are registered in factory by deffinition of variable of type AddProduction instantiated
	  with the right arguments.

	  @note It is recommended to create registering AddProduction objects at the beginning of
	  implementation file of each class.

	  @see IProducible
	*/
	template<
		class Interfase,
		class Production >
	class AddProduction {
	public:
		/*!
		  @brief Constructor.

		  @details Registers class Production in factory that produces objects implementing interface
		  Interface upon creation.
		*/
		AddProduction () {
			/*
			   Check if class with the same name was already registered. In common case it is error.
			*/
			factoryAssert (Factory< Interfase >::produceFunctions ().count (Production::typeId ()));

			if (Factory< Interfase >::produceFunctions ().count (Production::typeId ())) {
				warningOutput
					<< StringType ("Second registration of class with id ")
					<< Production::typeId ()
					<< StringType ("in factory ")
					<< typeid (Interfase).name ()
					<< StringType ("while geristering class ")
					<< typeid (Production).name ();

				if (Factory< Interfase >::produceFunctions () [Production::typeId()] == Production::createObject) {
					warningOutput << "Repeated registration of the same class";
				}
				else {
					warningOutput << "Registration of another class with dublicate id";
				}
			}

			Factory< Interfase >::produceFunctions () [Production::typeId()] =
				Production::createObject;
		}
	};


	/*!
	  @brief Interface of producible class.

	  @details This interfave must be implemented by all classes that are supposed to be used with
	  Factory.

	  None of the functions are declared virtual so it's mostly a declaration of expected interface
	  rather that an interface in comon sense.
	*/
	class IProducible {
	public:
		/*!
		  @brief Object creation function.

		  @details Called by factory upon request for object creation.

		  @returns Smat pointer std::shared_ptr for created object. T is an interface used to
		  instantiate Factory.
		*/
		template< class T > static std::shared_ptr< T > createObject ();

		/*!
		  @brief Function used to renreive unique class identifier to use in factory for objects
		  creation.

		  @returns String with unique identifier for class.
		*/
		static StringType typeId ();
	};
}

/*!
  @}
*/

#endif // FACTORY_H

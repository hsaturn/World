#include "object.h"
#include <cfileparser.hpp>

template <class T> typename Object<T>::TContainer Object<T>::mapObjects;

template <class T>
T* Object<T>::factory(CFileParser* poParser)
{
	string sName=poParser->getNextIdentifier(T::getClass()+" id");
	bool bHasRealDefinition=false;	// True if { was present
	T* poObject;

	if (true)
	{
		cout << "New object " << sName << endl;
		poObject=new T(sName);
	}
	else
	{
		cout << "reget item " << poObject->getName() << endl;
	}
	poObject->parseLongName(poParser,T::getClass());

	if (poObject->hasDefinition() && poParser->peekChar()=='{')
	{
		bHasRealDefinition=true;
		if (poObject->mbDefined)
			poParser->throw_(T::getClass()+" '"+sName+"' is defined twice.");

		poParser->getNextChar();
		while(poParser->good() && poParser->tryChar('}')==false)
		{
			string sLexeme=poParser->getNextIdentifier(T::getClass()+" entry");
			if (sLexeme=="filename")
			{
				poObject->msFileName=poParser->getNextString("file name");
			}
			else if (poObject->parseSpecific(poParser, sLexeme)==false)
			{
				poParser->throw_("Unknown "+T::getClass()+" element:("+sLexeme+")");
			}
		}

		poObject->mbDefined=true;
	}
	poObject->postFactory(poParser);
	if (bHasRealDefinition)
	{
		poObject->factoryCheck(poParser);
	}
	return poObject;
}



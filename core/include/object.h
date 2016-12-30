#ifndef _BOT_OBJECT_H_
#define _BOT_OBJECT_H_

#include <string>
#include <map>
#include <iostream>

using namespace std;

class CFileParser;

template <class T>
class Object
{
public:
	typedef map<string, T*> TContainer;
	typedef typename TContainer::iterator iterator;
	typedef typename TContainer::const_iterator const_iterator;

private:
	static TContainer mapObjects;

	string msName;
	string msLongName;
	string msFileName;

protected:
	bool mbDefined;

public:

	static void clear()
	{
	}

public:
	static T* factory(CFileParser*);

	Object() : mbDefined(false) { }
	Object(const string sName) : msName(sName), mbDefined(false) { cout << "  object " << msName << " not parsed." << endl;};
	virtual ~Object() {  }

	Object(CFileParser*, const string& sClass);

	const string& getName() const { return msName; };

	bool isDefined() const { return mbDefined; }

protected:
	virtual bool parseSpecific(CFileParser*, const string &sEntry) { return false; }
	virtual bool hasDefinition() const=0;

	/**
	 * Called only if the definition has been parsed (having a { ... } text in the config file
	 */
	virtual void factoryCheck(CFileParser*) {};

	/** Called once the object has been parsed, allowing to put more after the last } for example.
	 *  This allow also to put more short info after the long name
	 */
	virtual void postFactory(CFileParser*) {};
};

#endif

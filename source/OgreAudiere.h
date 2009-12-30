// OgreAudiere.h

#ifndef __OGRE_AUDIERE_H__
#define __OGRE_AUDIERE_H__

/****************************************************************
* Simple Audiere binding for OGRE
* by Holger Frydrych
*
* This binding implements an audiere::File derived class that
* uses the Ogre Resource system to load Ogre resources. You
* can then use such a File object with the audiere sound 
* library to load sounds from the Ogre resource system.
* The binding is placed in the public domain, do with it
* whatever you want.
****************************************************************/


#include <OgrePrerequisites.h>

class FileAccessor :
	public Ogre::ResourceGroupManager
{
public:

	Ogre::DataStreamPtr FindFile(std::string & filename);
};

Ogre::DataStreamPtr FileAccessor::FindFile ( std::string & filename )
{
	ResourceGroup * grp = getResourceGroup("General");

	if ( ! grp )
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"Cannot locate a resource group called 'General'",
			"FileAccessor::FindFile");
	}

	OGRE_LOCK_MUTEX(grp->OGRE_AUTO_MUTEX_NAME) // lock group mutex 

		ResourceLocationIndex::iterator rit = grp->resourceIndexCaseSensitive.find(filename);

	if ( rit != grp->resourceIndexCaseSensitive.end() )
	{
		// Found in the index 
		Ogre::Archive * fileArchive = rit->second;
		Ogre::DataStreamPtr data = fileArchive->open(filename);
		return data;
	}

	return Ogre::DataStreamPtr();
}

/*
namespace audiere
{
	// forward declaring File
	class File;

	// load a File from Ogre resource system.
	File* OpenOgreResourceFile(const Ogre::String& filename);
}
*/
#endif
